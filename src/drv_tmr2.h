/* 
 * File:   drv_tmr2.h
 * Author: liruya
 *
 * Created on 2018年6月23日, 下午5:12
 */

#ifndef DRV_TMR2_H
#define	DRV_TMR2_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "drv_common.h"
    
#define tmr2_interrupt_enable()     PIE1bits.TMR2IE = 1
#define tmr2_interrupt_disable()    PIE1bits.TMR2IE = 0
#define tmr2_start()                T2CONbits.TMR2ON = 1
#define tmr2_stop()                 T2CONbits.TMR2ON = 0
#define tmr2_set(val)               TMR2 = (val)
#define tmr2_get()                  (TMR2)
    
    typedef enum _tmr2_ckps{
        TMR2_CKPS_1,
        TMR2_CKPS_4,
        TMR2_CKPS_16,
        TMR2_CKPS_64
    }tmr2_ckps_t;

    typedef enum _tmr2_outps{
        TMR2_OUTPS_1,
        TMR2_OUTPS_2,
        TMR2_OUTPS_3,
        TMR2_OUTPS_4,
        TMR2_OUTPS_5,
        TMR2_OUTPS_6,
        TMR2_OUTPS_7,
        TMR2_OUTPS_8,
        TMR2_OUTPS_9,
        TMR2_OUTPS_10,
        TMR2_OUTPS_11,
        TMR2_OUTPS_12,
        TMR2_OUTPS_13,
        TMR2_OUTPS_14,
        TMR2_OUTPS_15,
        TMR2_OUTPS_16
    }tmr2_outps_t;
    
    typedef void (* tmr2_isr_cb_t)();
    
    extern void tmr2_init(tmr2_ckps_t ckps, tmr2_outps_t outps, tmr2_isr_cb_t cb);
    extern void tmr2_reload(uint8_t value);
//    extern void tmr2_set(uint8_t value);
//    extern uint8_t tmr2_get();
    extern void tmr2_isr();

#ifdef	__cplusplus
}
#endif

#endif	/* DRV_TMR2_H */

