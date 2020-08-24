

#include <stddef.h>
#include <stdint.h>
#include <common/standard.h>
#include <kernel/device_type.h>
#include <kernel/mm.h>
#include <kernel/kernel.h>

/*
    function that returns a struct which contains 2 variables , 
    the board type (rpi1 , rpi2 , rpi3 etc) and the mmio base address
*/

struct device_data get_device_data()
{

    struct device_data deviceData;

    uint32_t reg;
    

    #ifdef AARCH64
        __asm volatile ("mrs %0, midr_el1" : "=r" (reg));
    #else
        __asm volatile ("mrc p15,0,%0,c0,c0,0" : "=r" (reg));
    #endif

    switch((reg >> 4) & 0xFFF)
    {
        case 0xB76: 
            
            #ifndef RASPI_VER
            #define RASPI_VER 1
            #endif 

            deviceData.board_type = "RPi1";
            deviceData.mmio_address = 0x20000000;
            break;
        
        case 0xC07:

            #ifndef RASPI_VER
            #define RASPI_VER 2
            #endif 

            deviceData.board_type = "RPi2";
            deviceData.mmio_address = 0x3F000000;
            break;
        
        case 0xD03:
            
            #ifndef RASPI_VER
            #define RASPI_VER 3
            #endif
            
            deviceData.board_type = "RPi3";
            deviceData.mmio_address = 0x3F000000;
            break;

        case 0xD08:

            #ifndef RASPI_VER
            #define RASPI_VER 4
            #endif

            deviceData.board_type = "RPi4";
            deviceData.mmio_address = 0xFE000000;
            break;

        default:

            deviceData.board_type = NULL;
            deviceData.mmio_address = 0x3F000000;
            break;
        
    }


    return deviceData;
}


struct device_data *get_device_data_p()
{
    struct device_data *deviceData;

    deviceData = (struct device_data *)k_malloc(sizeof(struct device_data *));

    uint32_t reg;
    

    #ifdef AARCH64
        __asm volatile ("mrs %0, midr_el1" : "=r" (reg));
    #else
        __asm volatile ("mrc p15,0,%0,c0,c0,0" : "=r" (reg));
    #endif

    switch((reg >> 4) & 0xFFF)
    {
        case 0xB76: 

            #ifndef RASPI_VER
            #define RASPI_VER 1
            #endif

            deviceData->board_type = "RPi1";
            deviceData->mmio_address = 0x20000000;
            break;
        
        case 0xC07:

            #ifndef RASPI_VER
            #define RASPI_VER 2
            #endif

            deviceData->board_type = "RPi2";
            deviceData->mmio_address = 0x3F000000;
            break;
        
        case 0xD03:

            #ifndef RASPI_VER
            #define RASPI_VER 3
            #endif

            deviceData->board_type = "RPi3";
            deviceData->mmio_address = 0x3F000000;
            break;

        case 0xD08:
            
            #ifndef RASPI_VER
            #define RASPI_VER 4
            #endif

            deviceData->board_type = "RPi4";
            deviceData->mmio_address = 0xFE000000;
            break;

        default:

            deviceData->board_type = NULL;
            deviceData->mmio_address = 0x3F000000;
            break;
        
    }


    return deviceData;
}