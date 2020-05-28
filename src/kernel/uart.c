
#include <kernel/uart.h>
#include <kernel/device_type.h>
#include <common/standard.h>
#include <kernel/mm.h>
#include <kernel/kernel.h>
#include <stdint.h>
#include <stddef.h>

static struct device_data deviceData;
static struct UART_REG_DEF uart_reg_data;
static struct UART_MINI_REG_DEF uart_mini_data;
static struct GPIO_REG_DEF gpio_reg;


uint32_t MMIO_ADD;


/*
    first gets the deviceData struct and then using the mmio address , calls the get_reg to get a struct with all addresses
    of the UART registers 
*/

void uart_init_structs()
{
    deviceData = get_device_data();

    if(deviceData.board_type)
    {
        MMIO_ADD = deviceData.mmio_address;
    }

    if(!MMIO_ADD)
    {
        MMIO_ADD = 0x3F000000;
    }
    
    uart_reg_data = get_reg(MMIO_ADD);

    get_reg_m(MMIO_ADD);

    get_reg_gpio(MMIO_ADD);
    
}


void m_uart_init_structs()
{

    deviceData = get_device_data();
    MMIO_ADD = 0x3F000000;

    if(!deviceData.board_type)
    {
        
        MMIO_ADD = deviceData.mmio_address;

    }


    get_reg_m(MMIO_ADD);

    get_reg_gpio(MMIO_ADD);

}

//initialises UART

void uart_init()
{
    uart_init_structs();

    mmio_write(uart_reg_data.UART0_CR ,0x00000000 );

    mmio_write(uart_reg_data.GPPUD , 0x00000000);
    delay(150);

    mmio_write(uart_reg_data.GPPUDCLK0 , (1<<14)|(1<<15));
    delay(150);

    mmio_write(uart_reg_data.GPPUDCLK0 , 0x00000000);

    mmio_write(uart_reg_data.UART0_ICR , 0x7FF);

    mmio_write(uart_reg_data.UART0_IBRD , 1);

    mmio_write(uart_reg_data.UART0_FBRD , 40);

    mmio_write(uart_reg_data.UART0_LCRH, (1 << 4) | (1 << 5) | (1 << 6));

    mmio_write(uart_reg_data.UART0_IMSC, (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) |
            (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));

    mmio_write(uart_reg_data.UART0_CR, (1 << 0) | (1 << 8) | (1 << 9));
}

//using MMIO address of the board , returns a struct with all the register addresses
//each register is of 4 bytes , hence the 4 byte increment

struct UART_REG_DEF get_reg(uint32_t mmio_address)
{
    static struct UART_REG_DEF reg_def ;

    reg_def.GPIO_BASE = mmio_address + 0x200000;
    reg_def.GPPUD = reg_def.GPIO_BASE + 0x94;
    reg_def.GPPUDCLK0 = reg_def.GPIO_BASE+ 0x98;
    
    reg_def.UART0_BASE = reg_def.GPIO_BASE + 0x1000;
    
    reg_def.UART0_DR = reg_def.UART0_BASE + 0x00;
    reg_def.UART0_RSRECR = reg_def.UART0_BASE + 0x04;
    reg_def.UART0_FR = reg_def.UART0_BASE + 0x18;
    reg_def.UART0_ILPR = reg_def.UART0_BASE + 0x20;
    reg_def.UART0_IBRD = reg_def.UART0_BASE + 0x24;
    reg_def.UART0_FBRD = reg_def.UART0_BASE + 0x28;
    reg_def.UART0_LCRH   =  reg_def.UART0_BASE + 0x2C;
    reg_def.UART0_CR     =  reg_def.UART0_BASE + 0x30;
    reg_def.UART0_IFLS   =  reg_def.UART0_BASE + 0x34;
    reg_def.UART0_IMSC   =  reg_def.UART0_BASE + 0x38;
    reg_def.UART0_RIS    =  reg_def.UART0_BASE + 0x3C;
    reg_def.UART0_MIS    =  reg_def.UART0_BASE + 0x40;
    reg_def.UART0_ICR    =  reg_def.UART0_BASE + 0x44;
    reg_def.UART0_DMACR  =  reg_def.UART0_BASE + 0x48;
    reg_def.UART0_ITCR   =  reg_def.UART0_BASE + 0x80;
    reg_def.UART0_ITIP   =  reg_def.UART0_BASE + 0x84;
    reg_def.UART0_ITOP   =  reg_def.UART0_BASE + 0x88;
    reg_def.UART0_TDR    =  reg_def.UART0_BASE + 0x8C;

    return reg_def;
}

void get_reg_gpio(uint32_t mmio_base)
{   

    gpio_reg.GPFSEL1 = mmio_base + 0x00200004;
    gpio_reg.GPSET0 = mmio_base + 0x0020001C;
    gpio_reg.GPCLR0 = mmio_base + 0x00200028;
    gpio_reg.GPPUD = mmio_base + 0x00200094;
    gpio_reg.GPPUDCLK0 = mmio_base + 0x00200098;

}

void get_reg_m(uint32_t mmio_base)
{
    uart_mini_data.AUX_ENABLES_REG = mmio_base + 0x00215004;
    uart_mini_data.AUX_IO_REG = mmio_base + 0x00215040;
    uart_mini_data.AUX_IER_REG = mmio_base + 0x00215044;
    uart_mini_data.AUX_IIR_REG = mmio_base + 0x00215048;
    uart_mini_data.AUX_LCR_REG = mmio_base + 0x0021504C;
    uart_mini_data.AUX_MCR_REG = mmio_base + 0x00215050;
    uart_mini_data.AUX_LSR_REG = mmio_base + 0x00215054;
    uart_mini_data.AUX_MSR_REG = mmio_base + 0x00215058;
    uart_mini_data.AUX_SCRATCH = mmio_base + 0x0021505C;
    uart_mini_data.AUX_CNTL_REG = mmio_base + 0x00215060;
    uart_mini_data.AUX_STAT_REG = mmio_base + 0x00215064;
    uart_mini_data.AUX_BAUD_REG = mmio_base + 0x00215068;
 
}

//writes a value to a register

void mmio_write(uint32_t reg , uint32_t data)
{
    *(volatile uint32_t*)reg = data;
}

//reads a value from a mmio register
uint32_t mmio_read(uint32_t reg)
{
    return *(volatile uint32_t*)reg;
}

//sends a message to the co-processor to delay

void delay(int32_t count)
{
    __asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n" : "=r"(count): [count]"0"(count) : "cc");
}

//writes a char to the data register
void uart_putc(char c)
{
    while(mmio_read(uart_reg_data.UART0_FR) & (1<<4)){}
    mmio_write(uart_reg_data.UART0_DR , c);
}

//reads the topmost char from the data register
char uart_getc()
{
    while(mmio_read(uart_reg_data.UART0_FR) & (1<<4)){}
    return mmio_read(uart_reg_data.UART0_DR);
}


//writes a string to the data register
void uart_puts(const char* s)
{
    for(size_t i = 0; s[i]!='\0'; i++)
    {
        uart_putc((char)s[i]);
    }
}


//mini uart (uart1) setup



//GPIO 14 and GPIO 15 alt 5 funcs are 

void mini_uart_init()
{

    uint32_t selector , baudrate , baudrate_reg_val;

    baudrate = 115200;

    selector = get32(gpio_reg.GPFSEL1);

    selector &= ~(7<<12); // clean GPIO 14
    selector |= 2<<12; // ALT 5 func for GPIO 14
    
    selector &= ~(7<<15); // clean GPIO 15
    selector |= 2<<15; // ALT 5 func for GPIO 15

    put32(gpio_reg.GPFSEL1 , selector);

    put32(gpio_reg.GPPUD , 0);

    delay(150);

    put32(gpio_reg.GPPUDCLK0 , (1<<14)|(1<<15));

    delay(150);

    put32(gpio_reg.GPPUDCLK0 , 0);

    put32(uart_mini_data.AUX_ENABLES_REG , 1); //enable UART1

    put32(uart_mini_data.AUX_CNTL_REG , 0); // disable recv and trans temp

    put32(uart_mini_data.AUX_IER_REG , 0); //disable interrupts

    put32(uart_mini_data.AUX_LCR_REG , 3); //enable 8bit mode

    put32(uart_mini_data.AUX_MCR_REG , 0); //RTS line always high

    baudrate_reg_val = (250000/((baudrate/1000)*8)) - 1;

    put32(uart_mini_data.AUX_BAUD_REG , 270); // baud rate 115200

    put32(uart_mini_data.AUX_CNTL_REG , 3); // enable trans and recv

}

void mini_uart_send_c(char c)
{

    while(1)
    {
        if(get32(uart_mini_data.AUX_LSR_REG)&0x20)
        {
            break;
        }
    }

    put32(uart_mini_data.AUX_IO_REG , c);

}

char mini_uart_read()
{   

    while(1)
    {
        if(get32(uart_mini_data.AUX_LSR_REG)&0x20)
        {
            break;
        }
    }


    return (get32(uart_mini_data.AUX_LSR_REG)&0xFF);

}


void mini_uart_send(const char *s)
{   

    for(size_t i = 0; s[i]!='\0'; i++)
    {
        mini_uart_send_c(s[i]);
    }

}