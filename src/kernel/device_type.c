

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
        __asm volatile ("mrc p15,0,%0,c0,c0,0" : "=r" (reg));
    #else
        asm volatile ("mrs %0, midr_el1" : "=r" (reg));
    #endif

    switch((reg >> 4) & 0xFFF)
    {
        case 0xB76: 

            deviceData.board_type = "RPi1";
            deviceData.mmio_address = 0x20000000;
            break;
        
        case 0xC07:

            deviceData.board_type = "RPi2";
            deviceData.mmio_address = 0x3F000000;
            break;
        
        case 0xD03:

            deviceData.board_type = "RPi3";
            deviceData.mmio_address = 0x3F000000;
            break;

        case 0xD08:

            deviceData.board_type = "RPi4";
            deviceData.mmio_address = 0xFE000000;
            break;

        default:

            deviceData.board_type = "????";
            deviceData.mmio_address = 0x3F000000;
            break;
        
    }


    return deviceData;
}


struct device_data *get_device_data_p()
{
    struct device_data *deviceData;

    deviceData = (struct device_data *)allocate_page();

    uint32_t reg;
    

    #ifdef AARCH64
        asm volatile ("mrc p15,0,%0,c0,c0,0" : "=r" (reg));
    #else
        asm volatile ("mrs %0, midr_el1" : "=r" (reg));
    #endif

    switch((reg >> 4) & 0xFFF)
    {
        case 0xB76: 

            deviceData->board_type = "RPi1";
            deviceData->mmio_address = 0x20000000;
            break;
        
        case 0xC07:

            deviceData->board_type = "RPi2";
            deviceData->mmio_address = 0x3F000000;
            break;
        
        case 0xD03:

            deviceData->board_type = "RPi3";
            deviceData->mmio_address = 0x3F000000;
            break;

        case 0xD08:

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