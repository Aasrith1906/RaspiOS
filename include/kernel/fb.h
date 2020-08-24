#include <stdint.h>
#include <stddef.h>
#include <common/standard.h>

#ifndef _FB_H
#define _FB_H


#define COLOUR_DEPTH 24
#define BYTES_PER_PIXEL COLOUR_DEPTH/8

#define CHARS_WIDTH 8
#define CHARS_HEIGHT 8

void init_fb();
void init_structs_fb();
void fb_putc(unsigned char c);
void fb_puts(const char *s);

struct fb_info_t
{

    uint32_t width;
    uint32_t height;
    uint32_t pitch;
    void *buffer_loc;
    uint32_t buffer_size;
    uint32_t chars_width;
    uint32_t chars_height;
    uint32_t char_x;
    uint32_t char_y;

};



struct fb_info_t *fb_info;

typedef enum
{

    NULL_TAG = 0,
    FB_ALLOCATE_BUFFER = 0x00040001,
    FB_RELEASE_BUFFER = 0x00048001,
    FB_GET_PHYSICAL_DIM = 0x00040003,
    FB_SET_PHYSICAL_DIM = 0x00048003,
    FB_GET_VIRTUAL_DIM = 0x00040004,
    FB_SET_VIRTUAL_DIM = 0x00048004,
    FB_GET_BITS_PER_PIXEL = 0x00040005,
    FB_SET_BITS_PER_PIXEL = 0x00048005,
    FB_GET_BYTES_PER_ROW = 0x00040008

} fb_property_tags;


struct fb_allocate_res_t
{
    void *fb_addr;
    uint32_t fb_size;
};

struct fb_screen_size_t
{
    uint32_t width;
    uint32_t height;
};

typedef union  
{

    uint32_t fb_allocate_align;
    struct fb_allocate_res_t fb_allocate_res;
    struct fb_screen_size_t fb_screen_size;
    uint32_t fb_bits_per_pixel;
    uint32_t fb_bytes_per_row;
} value_buffer_t;


#endif