#include "drv_gpio.h"

/**
 * set gpio as analog
 * @param pin
 */
void gpio_set_analo( gpio_pin_t pin )
{
	uint8_t mask = 1<<(pin-GPIOA_PIN0);
	ANSELA |= mask;
	TRISA |= mask;
}

/**
 * set gpio as input
 * @param pin
 * @param pullup	pullup enable
 */
void gpio_set_input( gpio_pin_t pin, bool pullup ) 
{
	uint8_t mask = 1 << ( pin - GPIOA_PIN0 );
	ANSELA &= ~mask;
	if ( pullup ) 
	{
		WPUA |= mask;
	} 
	else 
	{
		WPUA &= ~mask;
	}
	TRISA |= mask;
}

/**
 * set gpio as output
 * @param pin
 * @param level		output level
 */
void gpio_set_output( gpio_pin_t pin, bool level )
{
	uint8_t mask = 1<<(pin-GPIOA_PIN0);
	ANSELA &= ~mask;
	if ( level ) 
	{
		LATA |= mask;
	}
	else
	{
		LATA &= ~mask;
	}
	TRISA &= ~mask;
}

/**
 * RX/DT pin select
 * @param pin	RXDT_PIN_RA1 or RXDT_PIN_RA5
 */
void gpio_rxdt_select( rxdt_pin_t pin )
{
	APFCONbits.RXDTSEL = (uint8_t) pin;
}

/**
 * CWGA pin select
 * @param pin	CWGA_PIN_RA2 or CWGA_PIN_RA5
 */
 void gpio_cwga_select( cwga_pin_t pin )
{
	 APFCONbits.CWGASEL = ( uint8_t ) pin;
}
 
 /**
  * CWGB pin select
  * @param pin	CWGB_PIN_RA0 or CWGB_PIN_RA4
  */
 void gpio_cwgb_select( cwgb_pin_t pin )
 {
	APFCONbits.CWGBSEL = ( uint8_t ) pin;
 }
 
 /**
  * T1G pin select
  * @param pin	T1G_PIN_RA4 or T1G_PIN_RA3
  */
 void gpio_t1g_select( t1g_pin_t pin )
 {
	 APFCONbits.T1GSEL = ( uint8_t ) pin;
 }
 
 /**
  * TX/CK pin select
  * @param pin	TXCK_PIN_RA0 or TXCK_PIN_RA4
  */
 void gpio_txck_select( txck_pin_t pin )
 {
	 APFCONbits.TXCKSEL = ( uint8_t ) pin;
 }
 
 /**
  * PWM2 pin select
  * @param pin	PWM2_PIN_RA0 or PWM2_PIN_RA4
  */
 void gpio_pwm2_select( pwm2_pin_t pin )
 {
	 APFCONbits.P2SEL = ( uint8_t ) pin;
 }
 
 /**
  * PWM1 pin select
  * @param pin	PWM1_PIN_RA1 or PWM1_PIN_RA5
  */
 void gpio_pwm1_select( pwm1_pin_t pin )
 {
	APFCONbits.P1SEL = ( uint8_t ) pin;
 }
