#include <kernel/mm.h>
#include <kernel/device_type.h>
#include <common/standard.h>
#include <kernel/mailbox.h>
#include <stddef.h>
#include <stdint.h>





void init_fb()
{
    struct property_message_tag_t tags[5];
    
    tags[0].tag = FB_SET_PHYSICAL_DIM;

    tags[0].value_buff.fb_screen_size.width = 1024;
    tags[0].value_buff.fb_screen_size.height = 768;

    tags[1].tag = FB_SET_VIRTUAL_DIM;
    
    tags[1].value_buff.fb_screen_size.width = 1024;
    tags[1].value_buff.fb_screen_size.height = 768;
    
    tags[2].tag = FB_SET_BITS_PER_PIXEL;
    tags[2].value_buff.fb_bits_per_pixel = COLOUR_DEPTH;

    tags[3].tag = NULL_TAG;

    mailbox_send_prop_messages(tags);

    fb_info = (struct fb_info_t *)allocate_page();


    fb_info->width =  tags[0].value_buff.fb_screen_size.width;
    fb_info->height = tags[0].value_buff.fb_screen_size.height;
    fb_info->chars_width = fb_info->width/CHARS_WIDTH;
    fb_info->chars_height = fb_info->height/CHARS_HEIGHT;
    fb_info->char_x = 0;
    fb_info->char_y = 0;
    
    fb_info->pitch = fb_info->width*BYTES_PER_PIXEL;

    fb_info->buffer_loc = tags[0].value_buff.fb_allocate_res.fb_addr;
    fb_info->buffer_size = tags[0].value_buff.fb_allocate_res.fb_size;
}