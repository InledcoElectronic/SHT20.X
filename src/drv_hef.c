#include "drv_hef.h"

#define FLASH_SIZE			2048
#define HEF_ADDR_START		0x780
#define BLOCK_SIZE			16
#define BLOCK_MASK			0x7F0

void hef_erase_block(uint16_t addr)
{
	bool state = INTCONbits.GIE;
	INTCONbits.GIE = 0;
	PMADR = addr;
	PMCON1bits.CFGS = 0;
	PMCON1bits.FREE = 1;
	PMCON1bits.WREN = 1;
	PMCON2 = 0x55;
	PMCON2 = 0xAA;
	PMCON1bits.WR = 1;
	NOP();
	NOP();
	PMCON1bits.WREN = 0;
	INTCONbits.GIE = state;
}

/**
 * 
 * @param addr	auto align to block start
 * @param pbuf
 */
void hef_write_block(uint16_t addr, const uint8_t *pbuf)
{
	hef_erase_block(addr);
	addr &= BLOCK_MASK;
	bool state = INTCONbits.GIE;
	INTCONbits.GIE = 0;
	PMCON1bits.CFGS = 0;
	PMCON1bits.WREN = 1;
	PMCON1bits.LWLO = 1;
	for(uint8_t i = 0; i < BLOCK_SIZE; i++)
	{
		PMADR = addr;
		PMDATL = pbuf[i];
		if(i== (BLOCK_SIZE-1))
		{
			PMCON1bits.LWLO = 0;
		}
		PMCON2 = 0x55;
		PMCON2 = 0xAA;
		NOP();
		NOP();
		addr++;
	}
	PMCON1bits.WREN = 0;
	INTCONbits.GIE = state;
}

/**
 * 
 * @param addr	auto align to block start
 * @param pbuf
 * @param len	buffer size, must less than BLOCK_SIZE
 */
void hef_write_buffer(uint16_t addr, const uint8_t *pbuf, uint8_t len)
{
	if( len > BLOCK_SIZE )
	{
		return;
	}
	hef_erase_block(addr);
	addr &= BLOCK_MASK;
	bool state = INTCONbits.GIE;
	INTCONbits.GIE = 0;
	PMCON1bits.CFGS = 0;
	PMCON1bits.WREN = 1;
	PMCON1bits.LWLO = 1;
	for(uint8_t i = 0; i < len; i++)
	{
		PMADR = addr;
		PMDATL = pbuf[i];
		if(i == (len - 1))
		{
			PMCON1bits.LWLO = 0;
		}
		PMCON2 = 0x55;
		PMCON2 = 0xAA;
		NOP();
		NOP();
		addr++;
	}
	PMCON1bits.WREN = 0;
	INTCONbits.GIE = state;
}

void hef_read_buffer(uint16_t addr, uint8_t *pbuf, uint8_t len)
{
	for(uint8_t i = 0; i < len; i++)
	{
		pbuf[i] = hef_read_byte(addr++);
	}
}

uint8_t hef_read_byte(uint16_t addr)
{
	bool state = INTCONbits.GIE;
	INTCONbits.GIE = 0;
	PMADR = addr;
	PMCON1bits.CFGS = 0;
	PMCON1bits.RD = 1;
	NOP();
	NOP();
	INTCONbits.GIE = state;
	return PMDATL;
}

void hef_write_byte(uint16_t addr, uint8_t byte)
{
	if(addr >= FLASH_SIZE || addr < HEF_ADDR_START)
	{
		return;
	}
	uint8_t buf[BLOCK_SIZE];
	uint16_t start = addr&BLOCK_MASK;
	uint8_t offset = addr-start;
	for(uint8_t i = 0; i < BLOCK_SIZE; i++)
	{
		buf[i] = hef_read_byte(start++);
	}
	buf[offset] = byte;
	hef_write_block(start, buf);
}
