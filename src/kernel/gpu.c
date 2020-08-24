#include <kernel/fb.h>
#include <kernel/mm.h>
#include <common/standard.h>
#include <common/chars_pixels.h>
#include <kernel/gpu.h>


void gpu_init(void)
{
    init_fb();

    struct pixel_t white = {0xFF , 0xFF , 0xFF};

}



void gpu_write_pixel(uint32_t x , uint32_t y , struct pixel_t *pixel)
{
    if(!fb_info->buffer_loc)
    {
        ke_printf("error fb not initialized");
        return;
    }

    uint8_t *location = (uint32_t)fb_info->buffer_loc + y*fb_info->pitch + x*BYTES_PER_PIXEL;

    memcpy(location , pixel , BYTES_PER_PIXEL);

}

void gpu_putc(char c)
{
   
    struct pixel_t white = {0xFF , 0xFF , 0xFF};
    struct pixel_t black = {0x00 , 0x00 , 0x00};

    uint8_t w,h;
    uint8_t mask;
    size_t i;
    uint8_t *bmp = font(c);

    uint32_t num_rows = fb_info->height/fb_info->chars_height;

    if(fb_info->char_y >= num_rows)
    {
        for(i = 0; i < num_rows-1; i++)
        {
            // scroll kinda lol

            memcpy((uint32_t)fb_info->buffer_loc + fb_info->pitch*i*CHARS_HEIGHT , 
            (uint32_t)fb_info->buffer_loc + fb_info->pitch*(i+1)*CHARS_HEIGHT , 
            fb_info->pitch * CHARS_HEIGHT );

        }

        memset((void *)((uint32_t)fb_info->buffer_loc + fb_info->pitch*i*CHARS_HEIGHT), 0 , 
        fb_info->pitch*CHARS_HEIGHT); //erase the last row

        fb_info->char_y--;
    }

    if(c == '\n')
    {
        fb_info->char_x = 0;
        fb_info->char_y++;

        return;
    }

    for(w = 0; w < CHARS_WIDTH; w++)
    {
        for(h = 0 ; h < CHARS_HEIGHT; h++)
        {
            mask = 1 << (w);

            if(bmp[h] & mask)
            {
                gpu_write_pixel(fb_info->char_x*CHARS_WIDTH + w , 
                fb_info->char_y*CHARS_HEIGHT + h , &white);
            }
            else
            {
                gpu_write_pixel(fb_info->char_x*CHARS_WIDTH + w , fb_info->char_y*CHARS_HEIGHT+h , &black);
            }
        }
    }
    
    fb_info->char_x++;

    if(fb_info->char_x > fb_info->chars_width)
    {
        fb_info->char_x = 0;
        fb_info->char_y++;
    }
}

void gpu_puts(const char *s)
{
    int len = strlen(s);

    for(size_t i = 0; i < len; i++)
    {
        gpu_putc(s[i]);
    }
}