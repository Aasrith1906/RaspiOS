#include <kernel/kernel.h>
#include <common/standard.h>
#include <kernel/device_type.h>
#include <kernel/uart.h>
#include <kernel/atag.h>
#include <kernel/mm.h>



#ifdef AARCH64
void kernel_main(uint64_t dtb_ptr32, uint64_t x1, uint64_t x2, uint64_t x3)
#else
void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags)
#endif
{

    #ifndef AARCH64
    (void)r0;
    (void)r1;
    #else
    (void)x1;
    (void)x2;
    (void)x3;
    #endif

    
    
    #ifndef AARCH64
    setup_tags((struct atag *)atags);

    memory_init((struct atag *)atags);

    uart_init();

    struct device_data *d;
    d = get_device_data_p();

    char *s;

    s = d->board_type;

    uart_puts((const char *)s);
    
    while(1)
    {
        uart_putc(uart_getc());
    }

    #else

    setup_tags((struct atag *)dtb_ptr32);
    memory_init((struct atag *)dtb_ptr32);

    mini_uart_init();

    mini_uart_send("test");

    while(1)
    {
        mini_uart_send_c(mini_uart_read());
    }

    #endif  


}