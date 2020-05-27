
#include <stddef.h>
#include <stdint.h>

#ifndef ATAG_H
#define ATAG_H

typedef enum
{
    ATAG_CORE = 0x54410001,
    ATAG_MEM = 0x54410002,
    ATAG_VIDEOTEXT = 0x54410003,
    ATAG_RAMDISK = 0x54410004,
    ATAG_INITRD2 = 0x54420005,
    ATAG_SERIAL = 0x54410006,
    ATAG_CMDLINE = 0x54410009,
    ATAG_NONE = 0x00000000
} atag_type;


struct atag_core
{
    uint32_t flags;
    uint32_t pagesize;
    uint32_t rootdev;
};


struct atag_mem
{
    uint32_t size;
    uint32_t start;
};


struct atag_videotext
{
    uint8_t x;
    uint8_t y;
    uint16_t video_page;
    uint8_t video_mode;
    uint8_t video_cols;
    uint16_t video_ega_bx;
    uint8_t video_lines;
    uint8_t video_isvga;
    uint16_t video_points;
};

struct atag_ramdisk
{
    uint32_t flags;
    uint32_t size;
    uint32_t start;
};

struct atag_initrd2
{
    uint32_t start;
    uint32_t size;
};

struct atag_serial
{
    uint32_t low;
    uint32_t high;
};

struct atag_cmdline
{
    char cmdline[1];
};

struct atag
{
    atag_type tag_type;
    uint32_t tag_size;

    union 
    {
        struct atag_core core;
        struct atag_mem mem;
        struct atag_videotext video_text;
        struct atag_ramdisk ram_disk;
        struct atag_initrd2 initrd2;
        struct atag_serial serial;
        struct atag_cmdline cmdline;

    };
    
};


uint32_t atag_get_memsize(struct atag *tag);
uint32_t atag_get_memsize_vm(struct atag *tag);
void setup_tags(struct atag *tag);
void setup_core_tag(struct atag *tag , uint32_t pagesize);
void setup_ramdisk_tag(struct atag *tag , uint32_t size);
void setup_initrd2_tag(struct atag *tag , uint32_t size , uint32_t start);
void setup_mem_tag(struct atag *tag ,uint32_t start , uint32_t size);
void setup_cmdline_tag(struct atag *tag , const char *line);
void setup_end_tag(struct atag *tag , uint32_t size);
#endif 
