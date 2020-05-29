
/*
standard functions for use
*/


#include <stdint.h>
#include <common/standard.h>


//copies data in 1 pointer to another

void *memcpy(void *dest, const void *src, size_t n)
{
    char *dp = dest;
    const char *sp = src;

    while(n--)
    {
        *dp++ = *sp++;
    }

    return dp;

}

size_t strlen(char *str)
{
    size_t size = 0;

    while(str[size])
    {
        size++;
    }

    return size;
}

char *strcpy(char *str , char *dest , size_t len)
{
    size_t i = 0;

    while(len--)
    {
        dest[i] = str[i];
        i++;    
    }

    return dest;
}

void memset(void *s , int c , size_t len)
{

    unsigned char* p = s;

    while(len--)
    {
        *p++ = (unsigned char)c;
    }

}
