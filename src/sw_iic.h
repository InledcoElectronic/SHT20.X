/* 
 * File:   drcv_iic.h
 * Author: liruya
 *
 * Created on 2018年6月22日, 下午4:17
 */

#ifndef SW_IIC_H
#define	SW_IIC_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "drv_common.h"
#include "drv_gpio.h"
    
    typedef enum _ack {
        ACK,
        NOACK
    } ack_t;

    extern void swiic_init(gpio_pin_t scl, gpio_pin_t sda);
    extern void swiic_release_scl();
    extern void swiic_release_sda();
    extern uint8_t swiic_read_scl();
    extern uint8_t swiic_read_sda();
    extern void swiic_start();
    extern void swiic_stop();
    extern bool swiic_write(uint8_t byte);
    extern uint8_t swiic_read(ack_t ack);
    extern bool swiic_write_cmd(uint8_t addr, uint8_t cmd);
    extern bool swiic_write_byte(uint8_t addr, uint8_t reg, uint8_t byte);
    extern bool swiic_read_byte(uint8_t addr, uint8_t reg, uint8_t *byte);
    extern bool swiic_write_buffer(uint8_t addr, uint8_t reg, const uint8_t *pbuf, uint8_t len);
    extern bool swiic_read_buffer(uint8_t addr, uint8_t reg, uint8_t *pbuf, uint8_t len);

#ifdef	__cplusplus
}
#endif

#endif	/* SW_IIC_H */

