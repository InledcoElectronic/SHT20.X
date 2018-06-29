#include "drv_tmr2.h"

tmr2_isr_cb_t tmr2_isr_cb;

void tmr2_init(tmr2_ckps_t ckps, tmr2_outps_t outps, tmr2_isr_cb_t cb)
{
	T2CONbits.T2CKPS = (uint8_t) ckps;
	T2CONbits.T2OUTPS = (uint8_t) outps;
	tmr2_isr_cb = cb;
}

void tmr2_reload(uint8_t value)
{
	PR2 = value;
}

//void tmr2_set(uint8_t value)
//{
//	TMR2 = value;
//}
//
//uint8_t tmr2_get()
//{
//	return TMR2;
//}

void tmr2_isr()
{
	if(PIR1bits.TMR2IF)
	{
		PIR1bits.TMR2IF = 0;
		if(tmr2_isr_cb != NULL)
		{
			tmr2_isr_cb();
		}
	}
}
