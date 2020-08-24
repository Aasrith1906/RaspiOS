
#include <kernel/mailbox.h>
#include <kernel/mm.h>
#include <kernel/device_type.h>
#include <kernel/kernel.h>
#include <common/standard.h>


struct mail0_struct_t *mail_struct;
struct device_data *d; 


uint32_t get_vbuff_len(struct property_message_tag_t *tag)
{

    switch(tag->tag)
    {
        case FB_ALLOCATE_BUFFER:
        case FB_GET_PHYSICAL_DIM:
        case FB_SET_PHYSICAL_DIM:
        case FB_GET_VIRTUAL_DIM:
        case FB_SET_VIRTUAL_DIM:
            return 8;
        case FB_GET_BITS_PER_PIXEL:
        case FB_SET_BITS_PER_PIXEL:
        case FB_GET_BYTES_PER_ROW:
            return 4;
        case FB_RELEASE_BUFFER:
        default:
            return 0;
    }

}

void init_mb_struct()
{

        d = get_device_data_p();

        uint32_t base = d->mmio_address;

        mail_struct = (struct mail0_struct_t *)k_malloc(sizeof(struct mail0_struct_t *));

        mail_struct->mb_base = base + MB_OFFSET;
        mail_struct->mail0_read = (struct  mail_message_t*)(mail_struct->mb_base + 0x00);
        mail_struct->mail0_status = (struct mail_status_t *)(mail_struct->mb_base + 0x18);
        mail_struct->mail0_write = (struct mail_message_t  *)(mail_struct->mb_base + 0x20);

}

void mailbox_init(void)
{
    init_mb_struct();
}

struct mail_message_t *mailbox_read(int channel)
{
    
    struct mail_message_t *mes;
    struct mail_status_t *stat;

    do
    {
        
        do
        {
            
            stat = mail_struct->mail0_status;

        } while (stat->empty==1);
        
        mes = mail_struct->mail0_read;

    } while (mes->channel != channel);

    return mes;
    
}

void mailbox_send(struct mail_message_t *msg , int channel)
{   

    struct mail_status_t *stat;

    do
    {
        stat = mail_struct->mail0_status;

    } while (stat->empty == 1);
    
    mail_struct->mail0_write = msg;
}

void mailbox_send_prop_messages(struct property_message_tag_t *tags)
{
    
        struct prop_m_buff_t *msg;
        struct mail_message_t mail;
        uint32_t buff = 0 ,len , buffpos;

        for(int i = 0; tags[i].tag != NULL_TAG; i++)
        {
            buff += get_vbuff_len(&tags[i]) + 3*sizeof(uint32_t);
        }

        buff+=3*sizeof(uint32_t);

        buff+= (buff % 16) ? 16 - (buff%16) : 0;

        msg = k_malloc(buff);

        if(!msg)
            ke_printf("error allocating memory");
            return;
        
        msg->size = buff;
        msg->req_code = REQUEST;

        for(int i = 0 , buffpos = 0; tags[i].tag != NULL_TAG; i++)
        {
            len = get_vbuff_len(&tags[i]);
            msg->tags[buffpos++] = tags[i].tag;
            msg->tags[buffpos++] = len;
            msg->tags[buffpos++] = 0;

            memcpy(msg->tags + buffpos, &tags[i].value_buff , len);
            buffpos += len/4;

        }

        msg->tags[buffpos] = 0;

        mail.data = ((uint32_t)msg) >> 4;
        
        mailbox_send(&mail , PROPERTY_CHANNEL);

        struct mail_message_t *recv_mail;

        recv_mail = mailbox_read(PROPERTY_CHANNEL);

        if(msg->req_code == REQUEST || msg->req_code == RESPONSE_ERROR)
        {
            k_free(msg);
            return;
        }

        for(int i = 0, buffpos = 0; tags[i].tag != NULL_TAG; i++)
        {
            
            len = get_vbuff_len(&tags[i]);
            buffpos+=3;
            memcpy(&tags[i].value_buff , msg->tags+buffpos , len);
            buffpos += len/4;
        }

        k_free(msg);

        return;
}