/* 
 * File:   sht2x.h
 * Author: liruya
 *
 * Created on 2018年6月23日, 上午9:40
 */

#ifndef SHT2X_H
#define	SHT2X_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "sw_iic.h"

    typedef enum _measure_type {
        MEASURE_TYPE_TEMPERATURE = 3,
        MEASURE_TYPE_HUMIDITY = 5
    } measure_type_t;

    typedef union _sht2x_value{
        struct{
            uint8_t valueH;
            uint8_t valueL;
            uint8_t checksum;
        };
        uint8_t array[3];
    }sht2x_value_t;

    extern void sht2x_init(gpio_pin_t scl, gpio_pin_t sda);
    extern bool sht2x_reset();
    extern bool sht2x_read_user_register(uint8_t *preg);
    extern bool sht2x_write_user_register(uint8_t value);
    extern int16_t sht2x_get_temperature();
    extern int16_t sht2x_get_humidity();

#ifdef	__cplusplus
}
#endif

#endif	/* SHT2X_H */

