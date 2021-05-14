#include <kernel/atag.h>
 
#define DRAM_BASE 0x10000000
#define INITRD_LOAD_ADDRESS DRAM_BASE + 0x800000



uint32_t atag_get_memsize(struct atag *tag)
{
    while(tag->tag_type != ATAG_NONE)
    {
        if(tag->tag_type == ATAG_MEM)
        {
            return tag->mem.size;
        }
        tag = (uint32_t *)tag + tag->tag_size;
    }

    return 0;
}

uint32_t atag_get_memsize_vm(struct atag *tag)
{
    return 1024 * 1024 * 256;
}

void setup_core_tag(struct atag *tag , uint32_t pagesize)
{
    while(tag->tag_type!=ATAG_NONE)
    {
        if(tag->tag_type == ATAG_CORE)
        {

            tag->core.pagesize = pagesize;
            tag->core.flags = 1;
            tag->core.rootdev = 0;
        }

        tag = (uint32_t *)tag + tag->tag_size; 
    }
}

void setup_mem_tag(struct atag *tag , uint32_t start , uint32_t size)
{

    while(tag->tag_type!=ATAG_NONE)
    {
        if(tag->tag_type == ATAG_MEM)
        {
            tag->mem.start = start;
            tag->mem.size = size;
        }

        tag = (uint32_t *)tag + tag->tag_size;
    }

}


void setup_ramdisk_tag(struct atag *tag , uint32_t size)
{

    while(tag->tag_type != ATAG_NONE)
    {
        if(tag->tag_type == ATAG_RAMDISK)
        {
            tag->ram_disk.flags = 0;
            tag->ram_disk.size = size;
            tag->ram_disk.start = 0;

        }

        tag = (uint32_t *)tag + tag->tag_size;
    }

}

void setup_initrd2_tag(struct atag *tag , uint32_t size , uint32_t start)
{
    while(tag->tag_type != ATAG_NONE)
    {
        if(tag->tag_type == ATAG_INITRD2)
        {
            tag->initrd2.size = size;
            tag->initrd2.start = start;
        }

         tag = (uint32_t *)tag + tag->tag_size;
    }
}

void setup_cmdline_tag(struct atag *tag , const char *line)
{
    //to be implemented later , don't know what to do yet lol
}

void setup_end_tag(struct atag *tag  , uint32_t size)
{
    tag->tag_size = size;
}

void setup_tags(struct atag *tag)
{
    setup_core_tag(tag , 4096); //pagesize 4Kb

    setup_mem_tag(tag , DRAM_BASE, 0x10000000); //256 mb from DRAM_BASE

    setup_ramdisk_tag(tag , 4096); // 4mb ramdisk

    setup_initrd2_tag(tag , INITRD_LOAD_ADDRESS , 0x100000); // 1mb compressed placed in 8mb

    setup_cmdline_tag(tag , ""); // no clue yet ffs

    setup_end_tag(tag , 0);

}
