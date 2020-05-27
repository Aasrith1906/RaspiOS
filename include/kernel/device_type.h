
#ifndef _DEVICE_TYPE_H
#define _DEVICE_TYPE_H


#include <stdint.h>




struct device_data
{
    char *board_type;
    uint32_t mmio_address;
};

struct device_data get_device_data();
struct device_data *get_device_data_p();

#endif