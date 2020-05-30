
#include <kernel/mm.h>
#include <kernel/atag.h>
#include <common/standard.h>
#include <kernel/uart.h>
#include <kernel/kernel.h>

extern uint8_t __end;

uint32_t num_pages;

static struct page_t *all_pages_array;

struct list_pages list_free_pages;

struct heap_segment *heap_segement_start;

void init_list(struct list_pages *page_list)
{
    page_list->head_page = page_list->tail_page = (void *)0;

    page_list->list_size = 0;
}

void append_page(struct list_pages *page_list , struct page_t *page)
{
    page->next_page = NULL;
    page_list->tail_page->next_page = page;
    page->prev_page = page_list->tail_page;
    
    page_list->tail_page = page;

    if(page_list->head_page == NULL)
    {
        page_list->head_page = page;
        page->prev_page = NULL;
    }

    page_list->list_size +=1;

}

struct page_t *get_free_page(struct list_pages *free_pages)
{

    struct page_t *free_page;

    free_page = free_pages->head_page;

    free_pages->head_page = free_pages->head_page->next_page;

    free_pages->head_page->prev_page = NULL;

    if(free_pages->head_page == NULL)
    {
        free_pages->tail_page = NULL;
    }

    return free_page;
}




void *allocate_page(void)
{
    struct page_t *page;

    void *page_mem;

    if(list_free_pages.list_size==0)
    {
        return 0;
    }

    page = get_free_page(&list_free_pages);

    page->page_flags.allocated = 1;
    page->page_flags.kernel_page = 1;

    
    page_mem  = (void *)((page - all_pages_array)*PAGE_SIZE); // gets mem address to the page

    memset(page_mem , 0 , PAGE_SIZE); //setting page mem to 0

    return page_mem;

}


void free_page(void *ptr)
{
    struct page_t *page;

    page = all_pages_array + ((uint32_t) ptr/ PAGE_SIZE); 

    page->page_flags.allocated = 0; 
    page->page_flags.kernel_page = 0;

    append_page(&list_free_pages , page);
}

//initialising memory management and paging

#ifndef AARCH64

void memory_init(struct atag *tag)
{
    uint32_t MEMSIZE = atag_get_memsize_vm(tag);

    uint32_t page_array_len , kernel_pages, tmp , heap_start;

    
    /*
        uncomment line below if running on a real rpi,
        qemu does not generate atags and simulate the actual
        bootloader hence we cannot use the atag_get_memsize function
    */

   //uint32_t MEMSIZE = atag_get_memsize(tag);


   num_pages = MEMSIZE/PAGE_SIZE;

   page_array_len = sizeof(struct page_t) * num_pages;   

   all_pages_array = (struct page_t *)&__end;

   erase_data(all_pages_array , page_array_len);

   kernel_pages = (uint32_t)&__end/PAGE_SIZE; 
   
/* 
   since __end is allocated after kernel in linker file , page_size divided by the location of __end while
   give us all the num of kernel_pages
*/

    for( tmp = 0; tmp < kernel_pages; tmp++)
    {

        all_pages_array[tmp].virtual_addr_mapped = tmp*PAGE_SIZE;
        all_pages_array[tmp].page_flags.allocated = 1;
        all_pages_array[tmp].page_flags.kernel_page = 1;
    }

    for(; tmp < page_array_len; tmp++)
    {

        all_pages_array[tmp].page_flags.allocated = 0;

        append_page(&list_free_pages , &all_pages_array[tmp]);

    }

    heap_start = page_array_len + (uint32_t)&__end;

    init_heap_s(heap_start);
}   
#else

void memory_init()
{
    uint32_t MEMSIZE = 1024 * 1024 * 256;

    uint32_t page_array_len , kernel_pages, tmp , heap_start;

    
    /*
        uncomment line below if running on a real rpi,
        qemu does not generate atags and simulate the actual
        bootloader hence we cannot use the atag_get_memsize function
    */

   //uint32_t MEMSIZE = atag_get_memsize(tag);


   num_pages = MEMSIZE/PAGE_SIZE;

   page_array_len = sizeof(struct page_t) * num_pages;   

   all_pages_array = (struct page_t *)&__end;

   erase_data(all_pages_array , page_array_len);

   kernel_pages = (uint32_t)&__end/PAGE_SIZE; 
   
/* 
   since __end is allocated after kernel in linker file , page_size divided by the location of __end while
   give us all the num of kernel_pages
*/

    for( tmp = 0; tmp < kernel_pages; tmp++)
    {

        all_pages_array[tmp].virtual_addr_mapped = tmp*PAGE_SIZE;
        all_pages_array[tmp].page_flags.allocated = 1;
        all_pages_array[tmp].page_flags.kernel_page = 1;
    }

    for(; tmp < page_array_len; tmp++)
    {

        all_pages_array[tmp].page_flags.allocated = 0;

        append_page(&list_free_pages , &all_pages_array[tmp]);

    }

    heap_start = page_array_len + (uint32_t)&__end;

    init_heap_s(heap_start);
}   

#endif

void init_heap_s(uint32_t heap_start)
{
    heap_segement_start = (struct heap_segment *)heap_start;
    
    memset(heap_segement_start , 0 , sizeof(struct heap_segment));

    heap_segement_start->size = HEAP_SIZE;

}

// below functions don't work just yet fml

/*

void k_free(void *mem)
{

    size_t num_pages = sizeof(mem)/PAGE_SIZE;

    void **tmp;

    *tmp = mem;

    for(size_t i = 0; i<num_pages; ++i)
    {
        free_page(*tmp);

    }

}

void *k_malloc(uint32_t pages)
{
    size_t num_pages_allocated = 0;

    struct page_t *tmp_page , *start_page;

    void *mem;
    
    start_page = get_free_page(&list_free_pages);

    while(num_pages_allocated != pages)
    {
        tmp_page = get_free_page(&list_free_pages);

        if(!tmp_page)
        {

            uart_puts("error allocating page");

            k_free(mem);

            return (void *)NULL;

        }

        tmp_page->page_flags.allocated = 1;
        tmp_page->page_flags.kernel_page = 1;

        mem = (void *)((tmp_page-start_page)*PAGE_SIZE);

        num_pages_allocated+=1;
    }

}*/



void erase_data(struct page_t *page , uint32_t len)
{

    memset(page , 0 , (size_t) len);

}
