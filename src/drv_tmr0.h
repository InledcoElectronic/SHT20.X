/* 
 * File:   drv_tmr0.h
 * Author: liruya
 *
 * Created on 2018年6月23日, 下午4:53
 */

#ifndef DRV_TMR0_H
#define	DRV_TMR0_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "drv_common.h"
    
#define tmr0_interrupt_enable()     INTCONbits.T0IE = 1
#define tmr0_interrupt_disable()    INTCONbits.T0IE = 0
#define tnr0_set(val)               (TMR0 = (val))
#define tmr0_get()                  (TMR0)
    
    typedef enum _tmr0_prescaler{
        TMR0_PS_2,
        TMR0_PS_4,
        TMR0_PS_8,
        TMR0_PS_16,
        TMR0_PS_32,
        TMR0_PS_64,
        TMR0_PS_128,
        TMR0_PS_256,
    }tmr0_prescaler_t;

    typedef void (* tmr0_isr_cb_t)();
    
    extern void tmr0_init(bool ps_enable, tmr0_prescaler_t ps, tmr0_isr_cb_t cb);
    extern void tmr0_set(uint8_t value);
//    extern uint8_t tmr0_get();
//    extern void tmr0_isr();
    
#ifdef	__cplusplus
}
#endif

#endif	/* DRV_TMR0_H */

