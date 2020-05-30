#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>


char *convert_n(int num ,int base)
{
    static const char op[] = "0123456789ABCDEF";

    char tmp[50];
    int p = 0;

    do
    {
        tmp[p] = op[num%base];
        num /= base;

    } while (num!=0);

    return &tmp;
}


int main()
{
    char *str;

    int i = 1;


    str = convert_n(i , 10);

    printf("%s \n" , str);
}