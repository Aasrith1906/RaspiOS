#include <kernel/fb.h>
#include <kernel/mm.h>
#include <common/standard.h>

#ifndef _GPU_H
#define _GPU_H


struct pixel_t
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};  

void gpu_init(void);
void gpu_putc(char c);
void gpu_write_pixel(uint32_t x , uint32_t y , struct pixel_t *pixel);
void gpu_puts(const char *s);

#endif