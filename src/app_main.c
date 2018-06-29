/*
 * File:   app_main.c
 * Author: liruya
 *
 * Created on 2018年6月25日, 上午10:00
 */


#include "app_common.h"
#include "app_board.h"
#include "sht2x.h"
#include "app_protocol.h"

void app_init()
{
	osc_init();
	
	eusart_init(RX_PIN, TX_PIN, 9600, protocol_receive);
	sht2x_init(SCL_PIN, SDA_PIN);
	
	//(249+1)/(16MHz/4/16/5) = 5ms
	tmr2_init(TMR2_CKPS_16, TMR2_OUTPS_5, protocol_decode_frame);
	tmr2_reload(249);
	
	protocol_init();
	
	for(uint8_t i = 0; i < 20; i++)
	{
		sht2x_get_temperature();
		__delay_ms(10);
//		sht2x_get_humidity();
//		__delay_ms(10);
		CLRWDT();
	}
}

void interrupt app_isr()
{
//	if(PIE1bits.RCIE && PIR1bits.RCIF)
//	{
//		eusart_receive_isr();
//	}
//	else 
		if(PIE1bits.TMR2IE && PIR1bits.TMR2IF)
	{
		tmr2_isr();
	}
//	else if(PIE1bits.TXIE && PIR1bits.TXIF)
//	{
//		eusart_transmit_isr();
//	}
}

void app_process()
{
	static int16_t mLastTemp = 0;
	static int16_t mLastHumi = 0;
	int16_t mTemp = 0;
	int16_t mHumi = 0;
	
	CLRWDT();
	mTemp = sht2x_get_temperature();
	if(mTemp != mLastTemp || protocol_temp_para_changed() )
	{
		mLastTemp = mTemp;
		protocol_send_temperature(mTemp);
	}
	__delay_ms(500);

//	CLRWDT();
//	mHumi = sht2x_get_humidity();
//	if(mHumi != mLastHumi || protocol_humi_para_changed() )
//	{
//		mLastHumi = mHumi;
//		protocol_send_humidity(mHumi);
//	}
//	__delay_ms(500);
}

void main(void)
{
	
	app_init();
	global_interrupt_enable();
	peripheral_interrupt_enable();
	
	eusart_write(0x55);
	while(1)
	{
		CLRWDT();
		
		app_process();
	}
}
