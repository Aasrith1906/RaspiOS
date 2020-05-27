
#include <kernel/uart.h>
#include <kernel/device_type.h>
#include <common/standard.h>
#include <kernel/mm.h>

static struct device_data deviceData;
static struct UART_REG_DEF uart_reg_data;
static struct UART_MINI_REG_DEF uart_mini_data;

uint32_t MMIO_ADD;


/*
    first gets the deviceData struct and then using the mmio address , calls the get_reg to get a struct with all addresses
    of the UART registers 
*/

void uart_init_structs()
{
    deviceData = get_device_data();
    MMIO_ADD = 0x3F000000;

    if(deviceData.board_type)
    {
        MMIO_ADD = deviceData.mmio_address;
    }
    
    uart_reg_data = get_reg(MMIO_ADD);

    get_reg_m(MMIO_ADD);
    
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

struct UART_MINI_REG_DEF get_reg_m(uint32_t mmio_base)
{   

    uart_mini_data.GPFSEL1 = mmio_base + 0x00200004;
    uart_mini_data.GPSET0 = mmio_base + 0x0020001C;
    uart_mini_data.GPCLR0 = mmio_base + 0x00200028;
    uart_mini_data.GPPUD = mmio_base + 0x00200094;
    uart_mini_data.GPPUDCLK0 = mmio_base + 0x00200098;

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

void mini_uart_init()
{

    uint32_t selector;

    selector = mmio_read(uart_mini_data.GPFSEL1);

    selector &= ~(7<<12);
    selector |= 2<<12;
    selector |= 2<<15;

    mmio_write(uart_mini_data.GPFSEL1 , selector);

    mmio_write(uart_mini_data.GPPUD , 0);

    delay(150);

    mmio_write(uart_mini_data.GPPUDCLK0 , (1<<14)|(1<<15));

    delay(150);

    mmio_write(uart_mini_data.GPPUDCLK0 , 0);

}