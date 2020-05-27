#ifndef _KERNEL_H
#define _KERNEL_H

#define AARCH64

extern void put32(uint32_t data , uint32_t reg);
extern uint32_t get32(uint32_t reg);
extern void delay_m(size_t t);

#endif  