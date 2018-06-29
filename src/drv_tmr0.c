#include "drv_tmr0.h"

tmr0_isr_cb_t tmr0_isr_cb;

void tmr0_init(bool ps_enable, tmr0_prescaler_t ps, tmr0_isr_cb_t cb)
{
	OPTION_REGbits.PSA = (ps_enable == 0) ? 0 : 1;
	OPTION_REGbits.PS = (uint8_t) ps;
	tmr0_isr_cb = cb;
}

//void tmr0_set(uint8_t value)
//{
//	TMR0 = value;
//}
//
//uint8_t tmr0_get()
//{
//	return TMR0;
//}

void tmr0_isr()
{
	if(INTCONbits.T0IF)
	{
		INTCONbits.T0IF = 0;
		if(tmr0_isr_cb != NULL)
		{
			tmr0_isr_cb();
		}
	}
}
