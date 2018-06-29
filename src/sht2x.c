#include "sht2x.h"

#define	SHT2X_ADDR_W					0x80
#define	SHT2X_ADDR_R					0x81
#define CMD_MEASURE_HOLD_MASK			0xE0
#define CMD_MEASURE_POLL_MASK			0xF0
#define CMD_MEASURE_TEMPERATURE_HOLD	0xE3
#define CMD_MEASURE_HUMIDITY_HOLD		0xE5
#define CMD_MEASURE_TEMPERATURE_POLL	0xF3
#define CMD_MEASURE_HUMIDITY_POLL		0xF5
#define CMD_WRITE_REGISTER				0xE6
#define CMD_READ_REGISTER				0xE7
#define CMD_RESET						0xFE
#define CUSTOM_REG_MASK					0x38
#define TEMPERATURE_MEASURE_PERIOD		100			//ms	14bit
#define HUMIDITY_MEASURE_PERIOD			50			//ms	12bit

#define POLYNOMIAL						0x0131

#define BASE_WEIGHT						3
#define BASE_ELEMENT					((1<<BASE_WEIGHT)-1)


static sht2x_value_t sht2x_measure_t;
static sht2x_value_t sht2x_measure_rh;

/**
 * sht2x use default config: T-14bit  RH-12bit
 * @param scl
 * @param sda
 */
void sht2x_init(gpio_pin_t scl, gpio_pin_t sda)
{
	swiic_init(scl, sda);
}

static uint8_t crc8_check(uint8_t *pbuf, uint8_t len)
{
	uint8_t crc = 0;
	for(uint8_t i = 0; i < len; i++)
	{
		crc ^= pbuf[i];
		for(uint8_t j = 0; j < 8; j++)
		{
			if(crc & 0x80)
			{
				crc = (crc << 1) ^ POLYNOMIAL;
			}
			else
			{
				crc <<= 1;
			}
		}
	}
	return crc;
}

/**
 * sht2x software reset
 * @return reset success or not
 */
bool sht2x_reset()
{
	if(swiic_write_cmd(SHT2X_ADDR_W, CMD_RESET))
	{
		__delay_ms(15);
		return true;
	}
	return false;
}

/**
 * read sht2x user register
 * @param preg
 * @return		read user register success or not
 */
bool sht2x_read_user_register(uint8_t *preg)
{
	return swiic_read_byte(SHT2X_ADDR_W, CMD_READ_REGISTER, preg);
}

/**
 * write sht2x user register 
 * @param value	
 * @return		write user register success or not
 */
bool sht2x_write_user_register(uint8_t value)
{
	return swiic_write_byte(SHT2X_ADDR_W, CMD_WRITE_REGISTER, value);
}

/**
 * sht2x measure poll
 * @param type		MEASURE_TYPE_TEMPERATURE or MEASURE_TYPE_HUMIDITY
 * @param pdata		sensor value and checksum
 * @return			measure success or not
 */
static bool sht2x_measure_poll(measure_type_t type, sht2x_value_t *pdata)
{
	uint8_t error_count = 0;
	uint8_t cmd = CMD_MEASURE_TEMPERATURE_POLL;
	if(type == MEASURE_TYPE_HUMIDITY)
	{
		cmd = CMD_MEASURE_HUMIDITY_POLL;
	}
	swiic_start();
	if(swiic_write(SHT2X_ADDR_W))
	{
		swiic_stop();
		return false;
	}
	if(swiic_write(cmd))
	{
		swiic_stop();
		return false;
	}
	do
	{
		swiic_start();
		__delay_ms(10);
		error_count++;
		if(error_count > 20)
		{
			swiic_stop();
			return false;
		}
	}
	while(swiic_write(SHT2X_ADDR_R));
	pdata->array[0] = swiic_read(ACK);
	pdata->array[1] = swiic_read(ACK);
	pdata->array[2] = swiic_read(NOACK);
	swiic_stop();
	return crc8_check(pdata->array, 3) == 0;
}

/**
 * measure sht2x hold
 * @param type		MEASURE_TYPE_TEMPERATURE or MEASURE_TYPE_HUMIDITY
 * @param pdata		sensor value and checksum
 * @return			measure success or not
 */
static bool sht2x_measure_hold(measure_type_t type, sht2x_value_t *pdata)
{
	uint8_t cnt = 0;
	uint8_t cmd = CMD_MEASURE_TEMPERATURE_HOLD;
	if(type == MEASURE_TYPE_HUMIDITY)
	{
		cmd = CMD_MEASURE_HUMIDITY_HOLD;
	}
	swiic_start();
	if(swiic_write(SHT2X_ADDR_W))
	{
		swiic_stop();
		return false;
	}
	if(swiic_write(cmd))
	{
		swiic_stop();
		return false;
	}
	swiic_start();
	if(swiic_write(SHT2X_ADDR_R))
	{
		swiic_stop();
		return false;
	}
	swiic_release_scl();
	do {
		__delay_ms(2);
		cnt++;
		if(cnt > 100)
		{
			swiic_stop();
			return false;
		}
	} while(swiic_read_scl() == 0);
	pdata->array[0] = swiic_read(ACK);
	pdata->array[1] = swiic_read(ACK);
	pdata->array[2] = swiic_read(NOACK);
	swiic_stop();
	return crc8_check(pdata->array, 3) == 0;
}

/**
 * get sht2x measured temp value and convert to temp
 * T = -46.85 + 175.72 * value/2^16
 * @param ptemp
 * @return true if success
 */
static bool sht2x_get_temperature_once(int16_t *ptemp)
{
	if(sht2x_measure_hold(MEASURE_TYPE_TEMPERATURE, &sht2x_measure_t))
	{
		uint16_t value = (sht2x_measure_t.valueH<<8)|sht2x_measure_t.valueL;
		value >>= 2;
		*ptemp = (int16_t) ((1757.2 * value) / 16384u - 468.5);
		return true;
	}
	return false;
}

/**
 * get sht2x measured humi value and convert to humi
 * RH = -6 + 125 * value/2^16
 * @param phumi
 * @return	true if success
 */
static bool sht2x_get_humidity_once(int16_t *phumi)
{
	if(sht2x_measure_hold(MEASURE_TYPE_HUMIDITY, &sht2x_measure_rh))
	{
		uint16_t value = (sht2x_measure_rh.valueH << 8) | sht2x_measure_rh.valueL;
		;
		value >>= 2;
		*phumi = ((625u * value) >> 13) - 60;
		return true;
	}
	return false;
}

int16_t sht2x_get_temperature()
{
	static int16_t temperature = 0;
	int16_t temp;
	int32_t value;
	if(sht2x_get_temperature_once(&temp))
	{
		value = temperature * BASE_ELEMENT + temp;
		temperature = value >> BASE_WEIGHT;
	}
	return temperature;
}

int16_t sht2x_get_humidity()
{
	static int16_t humidity = 0;
	int16_t humi;
	int32_t value;
	if(sht2x_get_humidity_once(&humi))
	{
		value = humidity * BASE_ELEMENT + humi;
		humidity = value >> BASE_WEIGHT;
	}
	return humidity;
}