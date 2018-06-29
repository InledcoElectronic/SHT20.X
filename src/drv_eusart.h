/* 
 * File:   drv_eusart.h
 * Author: liruya
 *
 * Created on 2018年6月22日, 下午3:02
 */

#ifndef DRV_EUSART_H
#define	DRV_EUSART_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include "drv_common.h"
#include "drv_gpio.h"

#define eusart_receive_interrupt_enable()       PIE1bits.RCIE = 1
#define eusart_receive_interrupt_disable()      PIE1bits.RCIE = 0
    
    typedef void (* eusart_receive_cb_t)( uint8_t rcv );

    extern void eusart_init( rxdt_pin_t rx_pin, txck_pin_t tx_pin, uint32_t baud, eusart_receive_cb_t cb );
    extern uint8_t eusart_write(uint8_t byte);
    extern void eusart_transmit_isr();
    extern void eusart_receive_isr();

#ifdef	__cplusplus
}
#endif

#endif	/* DRV_EUSART_H */

