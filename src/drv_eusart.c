#include "drv_eusart.h"

#ifndef	EUSART_TX_BUFFER_SIZE
#define EUSART_TX_BUFFER_SIZE   48
#endif

static uint8_t tx_buffer[EUSART_TX_BUFFER_SIZE];
static uint8_t tx_head;
static uint8_t tx_tail;
static uint8_t tx_remain;

eusart_receive_cb_t eusart_rcv_cb;

void eusart_init(rxdt_pin_t rx_pin, txck_pin_t tx_pin, uint32_t baud, eusart_receive_cb_t cb)
{
	gpio_set_input(rx_pin, false);
	gpio_set_output(tx_pin, true);
	gpio_rxdt_select(rx_pin);
	gpio_txck_select(tx_pin);

	PIE1bits.RCIE = 0;
	PIE1bits.TXIE = 0;
	BAUDCON = 0x08;
	RCSTA = 0x90;
	TXSTA = 0x24;
	SPBRG = _XTAL_FREQ/4/(baud-1);

	tx_head = 0;
	tx_tail = 0;
	tx_remain = sizeof(tx_buffer);
	eusart_rcv_cb = cb;
}

/**
 * used for printf
 * @param c
 */
void putch(char c)
{
	while(tx_remain == 0);
	if(PIE1bits.TXIE == 0)
	{
		TXREG = c;
	}
	else
	{
		PIE1bits.TXIE = 0;
		tx_buffer[tx_head++] = c;
		if(tx_head >= sizeof( tx_buffer))
		{
			tx_head = 0;
		}
		tx_remain--;
	}
	PIE1bits.TXIE = 1;
}

/**
 * eusart send one byte by transmit isr
 * @param byte		byte to send
 * @return			the byte sent
 */
uint8_t eusart_write(uint8_t byte)
{
	putch(byte);
	return byte;
}

/**
 * eusart send one byte 
 * @param byte		byte to send
 * @return			the byte sent
 */
//uint8_t eusart_write(uint8_t byte)
//{
//	while(!PIR1bits.TXIF);
//	TXREG = byte;
//}

/**
 * eusart transmit interrupt
 */
void eusart_transmit_isr()
{
	if(tx_remain < sizeof(tx_buffer))
	{
		TXREG = tx_buffer[tx_tail++];
		if(tx_tail >= sizeof( tx_buffer))
		{
			tx_tail = 0;
		}
		tx_remain++;
	}
	else
	{
		PIE1bits.TXIE = 0;
	}
}

/**
 * eusart receive interrupt
 */
void eusart_receive_isr()
{
	uint8_t rcv = RCREG;
	if(eusart_rcv_cb != NULL)
	{
		eusart_rcv_cb(rcv);
	}
}
