

#ifndef MM_H
#define MM_H

#include <common/standard.h>
#include <kernel/atag.h>

// size of each page
#define PAGE_SIZE 4096

//size of the heap
#define HEAP_SIZE 1024*1024



// page_t_flags holds the meta data of each page:

struct page_t_flags
{
    uint32_t allocated:1; // if the page is allocated
    uint32_t kernel_page:1; //if it is a kernel page (part of the kernel)
    uint32_t reserved:30; //data
};


// struct used to create a list of pages


struct list_pages
{
    struct page_t *head_page;
    struct page_t *tail_page;

    uint32_t list_size;
};

//individual page node struct

struct page_t
{
     struct page_t_flags page_flags;
     uint32_t virtual_addr_mapped;

     struct page_t *next_page;
     struct page_t *prev_page;
};


//heap list structure

struct heap_segment
{
    uint32_t is_allocated:1;
    uint32_t size;

    struct heap_segment *next;
    struct heap_segment *prev;
};  


// function to initialize memory in the RPi
void memory_init(struct atag *tag);

// function used to init the list of pages
void init_list(struct list_pages *pages_list);

//function to append pages to list
void append_page(struct list_pages *pages_list , struct page_t *page);

//function to erase all data in a page
void erase_data(struct page_t *page , uint32_t len);    

//function to get a free page
struct page_t *get_free_page(struct list_pages *free_pages);

// returns a pointer to mem
void *allocate_page(void);

//allocates pages and returns as contiguous memory;
void *allocate_pages(size_t size);

// function to free a page , ptr is the pointer returned
void free_page(void *ptr);  

//init heap segment

void init_heap_s(uint32_t heap_start);

//kernel malloc , use to dynamically allocate memory , returns pointer to location in memory

void kmalloc(uint32_t bytes);

//frees dynamically allocated memory

void free(void *ptr);

#endif 
