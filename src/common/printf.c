#include <common/standard.h>
#include <kernel/uart.h>
#include <kernel/mm.h>
#include <stdarg.h>


int u_printf(const char *format , ...)
{

    va_list args;
    va_start(args , format);
    
    int written = 0;

    char *tmp_str;
    double tmp_f;
    int tmp;
    char tmp_c;

    int ptr = 0;

    char *buffer;

    buffer = (char *)allocate_page(); // this is temp , will change soon when i create proper heap malloc

    while(format[ptr]!='\0')
    {

        if(format[ptr]!='%')    
        {

            if(format[ptr] == NULL)
            {
                
            }


            buffer[written] = format[ptr];
            written+=1;
            ptr+=1;

        }

        else
        {
            ptr+=1;

            switch(format[ptr])
            {
                
                case 'c':

                    tmp = va_arg(args , int);

                    if(!tmp)
                    {
                        break;
                    }

                    buffer[written] = tmp;

                    written+=1;

                    break;

                case 's': 

                    tmp_str = va_arg(args , char *);
                    
                    if(!tmp_str)
                    {
                        break;
                    }

                    size_t len = strlen(tmp_str);

                    for(size_t i = 0; i<len; ++i)
                    {
                        buffer[written] = tmp_str[i];
                        written+=1;
                    }

                    break;

                case 'd':

                    tmp = va_arg(args , int);
                    
                    if(!tmp)
                    {
                        break;
                    }
                    
                    if(tmp < 0)
                    {
                        buffer[written] = '-';
                        written+=1;

                        tmp = 0 - tmp;    
                    }
                    
                    tmp_str = convert_n(tmp , 10);

                    size_t len_ = strlen(tmp_str);

                    for(size_t i = 0; i < len_; ++i)
                    {
                        buffer[written] = tmp_str[i];
                        written++;
                    }

                    written+=1;

                    break;

                case 'f':

                    tmp_f = va_arg(args , double);

                    tmp_str = convert_n(tmp , 10);

                    size_t len__ = strlen(tmp_str);

                    for(size_t i = 0; i < len__; ++i)
                    {

                        buffer[written] = tmp_str[i];
                        written++;

                    }

                    written++;

                    break;

                case 'x':

                    buffer[written] = '0';
                    buffer[written+1] = 'x';
                    written+=2;

                    tmp = va_arg(args , int);

                    tmp_str = convert_n(tmp , 16);

                    size_t len_hex = strlen(tmp_str);

                    for(size_t i = 0; i < len_hex; ++i)
                    {
                        buffer[written] = tmp_str[i];
                        written++;
                    }

                    written++;

                default:

                    break;

            }

            ptr+=1;

            /*if(ptr > sizeof(buffer))
            {
                uart_puts("over flow , print fail !!");

                free_page(buffer);
            }*/
        }
    }


    buffer[written] = '\0';

    uart_puts((const char *)buffer);

    free_page(buffer);

    return 1;
}


char *convert_n(int num, int base) 
{ 
	static char op[]= "0123456789ABCDEF";
	static char buffer[50]; 
	char *ptr; 
	
	ptr = &buffer[49]; 
	*ptr = '\0'; 
	
	do 
	{ 
		*--ptr = op[num%base]; 
		num /= base; 
	}while(num != 0); 
	
	return(ptr); 
}