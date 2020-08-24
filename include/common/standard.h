
#include <stddef.h>
#include <stddef.h>
#include <kernel/mm.h>
#include <stdint.h>

#ifndef _STANDARD_H
#define _STANDARD_H



void *memcpy(void *dest, const void *src, size_t n);
size_t strlen(char *str);
char *strcpy(char *str , char *dest , size_t len);
void memset(void *s , int c , size_t n);

char *convert_n(int num , int base);
int u_printf(const char *format , ...);

int printf(const char *format , ...);

int ke_printf(char *message);


#endif