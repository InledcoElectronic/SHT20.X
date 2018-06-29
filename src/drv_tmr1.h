/* 
 * File:   drv_tmr1.h
 * Author: liruya
 *
 * Created on 2018年6月23日, 下午5:12
 */

#ifndef DRV_TMR1_H
#define	DRV_TMR1_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "drv_common.h"

#define tmr1_interrupt_enable       PIE1bits.TMR1IE = 1
#define tmr1_interrupt_disable      PIE1bits.TMR1IE = 0
#define tmr1_gate_interrupt_enable  PIE1bits.TMR1GIE = 1
#define tmr1_gate_interrupt_disable PIE1bits.TMR1GIE = 0
#define tmr1_start()                T1CONbits.TMR1ON = 1
#define tmr1_stop()                 T1CONbits.TMR1ON = 0
#define tmr1_set(val)               (TMR1 = (val))
#define tmr1_get()                  (TMR1)
#define tmr1_gate_enable()          T1GCONbits.TMR1GE = 1
#define tmr1_gate_disable()         T1GCONbits.TMR1GE = 0
#define tmr1_gate_go                T1GCONbits.T1GGO = 1

    typedef enum _tmr1_cs {
        TMR1_CS_FOSC_4,
        TMR1_CS_FOSC,
        TMR1_CS_T1CKI,
        TMR1_CS_LFINTOSC
    } tmr1_cs_t;
    
    typedef enum _tmr1_ckps{
        TMR1_CKPS_1,
        TMR1_CKPS_2,
        TMR1_CKPS_4,
        TMR1_CKPS_8
    }tmr1_ckps_t;
    
    typedef enum _tmr1_gss{
        TMR1_GSS_T1G,
        TMR1_GSS_C1OUT,
        TMR1_GSS_T0OVF,
        TMR1_GSS_RESERVED
    }tmr1_gss_t;

    typedef void (* tmr1_isr_cb_t)();
    typedef void (* tmr1_gate_isr_cb_t)();

    extern void tmr1_init(tmr1_cs_t cs, tmr1_ckps_t ckps, tmr1_isr_cb_t cb);
//    extern void tmr1_set(uint16_t value);
//    extern uint16_t tmr1_get();
    extern void tmr1_isr();
    
    extern void tmr1_gate_init(bool pol, bool tm, bool spm, tmr1_gss_t gss, tmr1_gate_isr_cb_t cb);
    extern void tmr1_gate_isr();


#ifdef	__cplusplus
}
#endif

#endif	/* DRV_TMR1_H */

