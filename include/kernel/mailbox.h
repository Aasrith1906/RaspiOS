#include <stdint.h>
#include <kernel/fb.h>

#ifndef _MAILBOX_H_


#define _MAILBOX_H_

#define FRAMEBUFFER_CHANNEL 1
#define PROPERTY_CHANNEL 8

#define MB_OFFSET 0xB880


typedef enum
{

    REQUEST = 0x00000000,
    RESPONSE_SUCCESS = 0x80000000,
    RESPONSE_ERROR = 0x80000001

}buffer_req_res;




struct mail0_struct_t
{

    uint32_t mb_base;
    struct mail_message_t *mail0_read;
    struct mail_status_t *mail0_status;
    struct mail_message_t *mail0_write;

} ;

struct mail_status_t
{
    
    uint32_t reserved:30;
    uint8_t empty:1;
    uint8_t full:1;

} ;

struct prop_m_buff_t
{
    uint32_t size;
    buffer_req_res req_code;
    uint32_t tags[1];

} ;

struct mail_message_t{

    uint8_t channel:4;
    uint32_t data:28;

};

struct property_message_tag_t
{
    fb_property_tags tag;
    value_buffer_t value_buff;
};


void init_mb_sturct(void);
struct mail_message_t *mailbox_read(int channel);
void mailbox_send(struct mail_message_t *msg , int channel);
uint32_t get_vbuff_len(struct property_message_tag_t *tag);
void mailbox_send_prop_messages(struct property_message_tag_t *tags);

void mailbox_init(void);

#endif