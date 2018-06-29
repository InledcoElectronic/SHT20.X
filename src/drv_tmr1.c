#include "drv_tmr1.h"

tmr1_isr_cb_t tmr1_isr_cb;
tmr1_gate_isr_cb_t tmr1_gate_isr_cb;

void tmr1_init(tmr1_cs_t cs, tmr1_ckps_t ckps, tmr1_isr_cb_t cb)
{
	T1CONbits.TMR1CS = (uint8_t) cs;
	T1CONbits.T1CKPS = (uint8_t) ckps;
	tmr1_isr_cb = cb;
}

//void tmr1_set(uint16_t value)
//{
//	TMR1 = value;
//}
//
//uint16_t tmr1_get()
//{
//	return TMR1;
//}

/**
 * tmr1 interrupt
 */
void tmr1_isr()
{
	if(PIR1bits.TMR1IF)
	{
		PIR1bits.TMR1IF = 0;
		if(tmr1_isr_cb != NULL)
		{
			tmr1_isr_cb();
		}
	}
}

/**
 * tmr1 gate config
 * @param pol	门控源极性
 * @param tm	翻转模式	门控源 翻转触发计数 触发->保持->触发->保持...
 * @param spm	单脉冲模式	需置位T1GGO开启捕捉,捕捉触发后自动清零
 * @param gss	门控源
 * @param cb
 */
void tmr1_gate_init(bool pol, bool tm, bool spm, tmr1_gss_t gss, tmr1_gate_isr_cb_t cb)
{
	T1GCONbits.T1GPOL = pol;
	T1GCONbits.T1GTM = tm;
	T1GCONbits.T1GSPM = spm;
	T1GCONbits.T1GSS = (uint8_t) gss;
	tmr1_gate_isr_cb = cb;
}

/**
 * tmr1 gate interrupt
 */
void tmr1_gate_isr()
{
	if(PIR1bits.TMR1GIF)
	{
		PIR1bits.TMR1GIF = 0;
		if(tmr1_gate_isr_cb != NULL)
		{
			tmr1_gate_isr_cb();
		}
	}
}
