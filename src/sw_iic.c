#include <pic12f1572.h>

#include "sw_iic.h"		

#define IIC_PIN_HIGH(pin)		TRISA |= (1<<((uint8_t) pin))
#define IIC_PIN_LOW(pin)		LATA &= (~(1<<((uint8_t) pin)));\
								TRISA &= (~(1<<((uint8_t) pin)))

#define scl_high()				IIC_PIN_HIGH(SCL)
#define scl_low()				IIC_PIN_LOW(SCL)
#define sda_high()				IIC_PIN_HIGH(SDA)
#define sda_low()				IIC_PIN_LOW(SDA)
#define read_scl()				((PORTA>>((uint8_t) SCL))&0x01)
#define read_sda()				((PORTA>>((uint8_t) SDA))&0x01)

static gpio_pin_t SCL;
static gpio_pin_t SDA;

/**
 * software iic init
 * @param clk	SCL pin
 * @param dat	SDA pin
 */
void swiic_init(gpio_pin_t clk, gpio_pin_t dat)
{
	SCL = clk;
	SDA = dat;
	gpio_set_input(clk, false);
	gpio_set_input(dat, false);
	sda_low();
	scl_low();
	sda_high();
	scl_high();
}

void swiic_release_scl()
{
	scl_high();
}

void swiic_release_sda()
{
	sda_high();
}

uint8_t swiic_read_scl()
{
	return read_scl();
}

uint8_t swiic_read_sda()
{
	return read_sda();
}

void swiic_start()
{
	scl_high();
	sda_high();
	__delay_us(4);
	sda_low();
	__delay_us(8); //hold time start condition
	scl_low();
	__delay_us(8);
}

void swiic_stop()
{
	scl_low();
	sda_low();
	__delay_us(2);
	scl_high();
	__delay_us(8); //setup time stop condition
	sda_high();
	__delay_us(8);
}

bool swiic_write(uint8_t byte)
{
	bool error = false;
	for(uint8_t mask = 0x80; mask > 0; mask >>= 1) //shift bit for mask
	{
		if((byte & mask) == 0)
		{
			sda_low();
		}
		else
		{
			sda_high();
		}
		__delay_us(1); //data setup time
		scl_high(); //start clk
		__delay_us(4); //scl high time
		scl_low();
		__delay_us(2); //data hold time
	}
	sda_high(); //release SDA
	scl_high(); //clk #9 for ack
	__delay_us(2); //data setup time
	error = read_sda(); //check ack from slave
	scl_low();
	__delay_us(12); //wait time to see byte package on scope
	return error;
}

uint8_t swiic_read(ack_t ack)
{
	uint8_t rcv = 0;
	sda_high(); //release SDA
	for(uint8_t mask = 0x80; mask > 0; mask >>= 1) //shift bit for mask
	{
		scl_high(); //start clk
		__delay_us(1); //data setup time
		__delay_us(2); //scl high time
		if(read_sda()) //read bit @ scl high time 50%
		{
			rcv |= mask;
		}
		__delay_us(2); //scl high time
		scl_low();
		__delay_us(2); //data hold time
	}
	if(ack == ACK)
	{
		sda_low();
	}
	else
	{
		sda_high();
	}
	__delay_us(2); //data setup time
	scl_high(); //clk #9 for ack
	__delay_us(6);
	scl_low();
	sda_high(); //release SDA
	__delay_us(12); //wait time to see byte package on scope
	return rcv;
}

bool swiic_write_cmd(uint8_t addr, uint8_t cmd)
{
	swiic_start();
	if(swiic_write(addr))
	{
		swiic_stop();
		return false;
	}
	if(swiic_write(cmd))
	{
		swiic_stop();
		return false;
	}
	swiic_stop();
	return true;
}

bool swiic_write_byte(uint8_t addr, uint8_t reg, uint8_t byte)
{
	swiic_start();
	if(swiic_write(addr))
	{
		swiic_stop();
		return false;
	}
	if(swiic_write(reg))
	{
		swiic_stop();
		return false;
	}
	if(swiic_write(byte))
	{
		swiic_stop();
		return false;
	}
	swiic_stop();
	return true;
}

bool swiic_read_byte(uint8_t addr, uint8_t reg, uint8_t *byte)
{
	swiic_start();
	if(swiic_write(addr))
	{
		swiic_stop();
		return false;
	}
	if(swiic_write(reg))
	{
		swiic_stop();
		return false;
	}
	swiic_start();
	if(swiic_write(addr + 1))
	{
		swiic_stop();
		return false;
	}
	*byte = swiic_read(NOACK);
	return true;
}

bool swiic_write_buffer(uint8_t addr, uint8_t reg, const uint8_t *pbuf, uint8_t len)
{
	swiic_start();
	if(swiic_write(addr))
	{
		swiic_stop();
		return false;
	}
	if(swiic_write(reg))
	{
		swiic_stop();
		return false;
	}
	for(uint8_t i = 0; i < len; i++)
	{
		if(swiic_write(*pbuf++))
		{
			swiic_stop();
			return false;
		}
	}
	swiic_stop();
	return true;
}

bool swiic_read_buffer(uint8_t addr, uint8_t reg, uint8_t *pbuf, uint8_t len)
{
	swiic_start();
	if(swiic_write(addr))
	{
		swiic_stop();
		return false;
	}
	if(swiic_write(reg))
	{
		swiic_stop();
		return false;
	}
	swiic_start();
	if(swiic_write(addr + 1))
	{
		swiic_stop();
		return false;
	}
	for(uint8_t i = 0; i < len - 1; i++)
	{
		*pbuf++ = swiic_read(ACK);
	}
	*pbuf = swiic_read(NOACK);
	swiic_stop();
	return true;
}
