#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>


#ifndef _UART_H
#define _UART_H


struct UART_REG_DEF
{   
    uint32_t GPIO_BASE;
    uint32_t GPPUD;
    uint32_t GPPUDCLK0;

    uint32_t UART0_BASE;
    uint32_t UART0_DR;
    uint32_t UART0_RSRECR;
    uint32_t UART0_FR;     
    uint32_t UART0_ILPR;   
    uint32_t UART0_IBRD;  
    uint32_t UART0_FBRD; 
    uint32_t UART0_LCRH;
    uint32_t UART0_CR;
    uint32_t UART0_IFLS; 
    uint32_t UART0_IMSC;
    uint32_t UART0_RIS; 
    uint32_t UART0_MIS; 
    uint32_t UART0_ICR;
    uint32_t UART0_DMACR;
    uint32_t UART0_ITCR; 
    uint32_t UART0_ITIP; 
    uint32_t UART0_ITOP; 
    uint32_t UART0_TDR; 

};

struct UART_MINI_REG_DEF
{
    uint32_t GPFSEL1;
    uint32_t GPSET0;
    uint32_t GPCLR0;
    uint32_t GPPUD;
    uint32_t GPPUDCLK0;
};

void uart_init_structs();
void uart_init();

struct UART_REG_DEF get_reg(uint32_t MMIO_ADD);

void mmio_write(uint32_t reg , uint32_t data);
uint32_t mmio_read(uint32_t reg);
void delay(int32_t count);

void uart_putc(char c);
char uart_getc();
void uart_puts(const char* s);

void mini_uart_init();
void mini_uart_send(const char* s);
void mini_uart_read();

#endif