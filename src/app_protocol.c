#include "app_protocol.h"

#define RECV_BUFFER_SIZE		40
#define DATA_BUFFER_SIZE		(RECV_BUFFER_SIZE-6)

#define TEMPERATURE_VERSION		1
#define HUMIDITY_VERSION		1
#define TEMPERATURE_ARGS_LENGTH	7
#define HUMIDITY_ARGS_LENGTH	7
#define SENSOR_TYPE_TEMPERATURE	2
#define SENSOR_TYPE_HUMIDITY	3
#define FRAME_HEADER			0x68
#define CMD_GET_TEMPERATURE		0x10	
#define CMD_GET_HUMIDITY		0x11
#define CMD_SET_TEMPERATURE		0x20
#define CMD_SET_HUMIDITY		0x21

#define HEF_TEMPERATURE_ADDR	0x7E0
#define HEF_HUMIDITY_ADDR		0x7F0

typedef struct {
	bool notify_enable;
	bool linkage_enable;
	int8_t threshold;
	bool night_mode_enable;
	uint16_t night_start;
	uint16_t night_end;
	int8_t night_threshold;
} temperature_para_t;

typedef struct {
	bool notify_enable;
	bool linkage_enable;
	int8_t threshold;
	bool night_mode_enable;
	uint16_t night_start;
	uint16_t night_end;
	int8_t night_threshold;
} humidity_para_t;

typedef union{
	struct{
		uint8_t header;
		uint8_t cmd;
		uint8_t notify_enable;
		uint8_t linkage_enable;
		uint8_t version;
		uint8_t length;
		uint8_t args[DATA_BUFFER_SIZE];
	};
	uint8_t array[RECV_BUFFER_SIZE];
}recv_frame_t;

static temperature_para_t temp_para;
static humidity_para_t humi_para;

static recv_frame_t recv_frame;
static uint8_t index;
static uint8_t checksum;
static bool temp_para_changed;
static bool humi_para_changed;

void protocol_init()
{
	hef_read_buffer(HEF_TEMPERATURE_ADDR, (uint8_t *) & temp_para, sizeof(temp_para));
//	hef_read_buffer(HEF_HUMIDITY_ADDR, (uint8_t *) & humi_para, sizeof(humi_para));
	if(temp_para.notify_enable > 1)
	{
		temp_para.notify_enable = false;
	}
	if(temp_para.linkage_enable > 1)
	{
		temp_para.linkage_enable = false;
	}
	if(temp_para.threshold < 15 || temp_para.threshold > 35)
	{
		temp_para.threshold = 25;
	}
	if(temp_para.night_mode_enable > 1)
	{
		temp_para.night_mode_enable = false;
	}
	if(temp_para.night_start > 1439)
	{
		temp_para.night_start = 1140;
	}
	if(temp_para.night_end > 1439)
	{
		temp_para.night_end = 420;
	}
	if(temp_para.night_threshold < 15 || temp_para.night_threshold > 35)
	{
		temp_para.night_threshold = 25;
	}
}

static void protocol_save_temperature_para()
{
	hef_write_buffer(HEF_TEMPERATURE_ADDR, (uint8_t *) & temp_para, sizeof(temp_para));
}

static void protocol_save_humidity_para()
{
	hef_write_buffer(HEF_HUMIDITY_ADDR, (uint8_t *) & humi_para, sizeof(humi_para));
}

void protocol_send_temperature(int16_t temp)
{
	uint8_t xor = 0;
	xor ^= eusart_write(FRAME_HEADER);
	xor ^= eusart_write(CMD_GET_TEMPERATURE);
	xor ^= eusart_write(SENSOR_TYPE_TEMPERATURE);
	xor ^= eusart_write(temp & 0xFF);
	xor ^= eusart_write(temp >> 8);
	xor ^= eusart_write(temp_para.notify_enable);
	xor ^= eusart_write(temp_para.linkage_enable);
	xor ^= eusart_write(TEMPERATURE_VERSION);
	xor ^= eusart_write(TEMPERATURE_ARGS_LENGTH);
	xor ^= eusart_write(temp_para.threshold);
	xor ^= eusart_write(temp_para.night_mode_enable);
	xor ^= eusart_write(temp_para.night_start & 0xFF);
	xor ^= eusart_write(temp_para.night_start >> 8);
	xor ^= eusart_write(temp_para.night_end & 0xFF);
	xor ^= eusart_write(temp_para.night_end >> 8);
	xor ^= eusart_write(temp_para.night_threshold);
	eusart_write(xor);
}

void protocol_send_humidity(int16_t humi)
{
	uint8_t xor = 0;
	xor ^= eusart_write(FRAME_HEADER);
	xor ^= eusart_write(CMD_GET_HUMIDITY);
	xor ^= eusart_write(SENSOR_TYPE_HUMIDITY);
	xor ^= eusart_write(humi & 0xFF);
	xor ^= eusart_write(humi >> 8);
	xor ^= eusart_write(humi_para.notify_enable);
	xor ^= eusart_write(humi_para.linkage_enable);
	xor ^= eusart_write(HUMIDITY_VERSION);
	xor ^= eusart_write(HUMIDITY_ARGS_LENGTH);
	xor ^= eusart_write(humi_para.threshold);
	xor ^= eusart_write(humi_para.night_mode_enable);
	xor ^= eusart_write(humi_para.night_start & 0xFF);
	xor ^= eusart_write(humi_para.night_start >> 8);
	xor ^= eusart_write(humi_para.night_end & 0xFF);
	xor ^= eusart_write(humi_para.night_end >> 8);
	xor ^= eusart_write(humi_para.night_threshold);
	eusart_write(xor);
}

void protocol_receive(uint8_t rcv)
{
	recv_frame.array[index++] = rcv;
	checksum ^= rcv;
	tmr2_stop();
	tmr2_set(0);
	tmr2_start();
}

/**
 * FRM_HDR CMD ntfy linkage version args_len args... xor
 */
void protocol_decode_frame()
{
	tmr2_stop();
	tmr2_set(0);
	if(recv_frame.header != FRAME_HEADER || checksum != 0 || index < 7 || recv_frame.length + 7 != index )
	{
		index = 0;
		return;
	}
	if(recv_frame.cmd == CMD_SET_TEMPERATURE 
		&& recv_frame.version == TEMPERATURE_VERSION
		&& recv_frame.length == TEMPERATURE_ARGS_LENGTH)
	{
		temp_para.notify_enable = recv_frame.notify_enable;
		temp_para.linkage_enable = recv_frame.linkage_enable;
		temp_para.threshold = recv_frame.args[0];
		temp_para.night_mode_enable = recv_frame.args[1];
		temp_para.night_start = (recv_frame.args[3]<<8)|recv_frame.args[2];
		temp_para.night_end = (recv_frame.args[5]<<8)|recv_frame.args[4];
		temp_para.night_threshold = recv_frame.args[6];
		temp_para_changed = true;
		protocol_save_temperature_para();
	}
//	else if(recv_frame.cmd == CMD_SET_HUMIDITY 
//		&& recv_frame.version == HUMIDITY_VERSION
//		&& recv_frame.length == HUMIDITY_ARGS_LENGTH)
//	{
//		humi_para.notify_enable = recv_frame.notify_enable;
//		humi_para.linkage_enable = recv_frame.linkage_enable;
//		humi_para.threshold = recv_frame.args[0];
//		humi_para.night_mode_enable = recv_frame.args[1];
//		humi_para.night_start = (recv_frame.args[3] << 8) | recv_frame.args[2];
//		humi_para.night_end = (recv_frame.args[5] << 8) | recv_frame.args[4];
//		humi_para.night_threshold = recv_frame.args[6];
//		humi_para_changed = true;
//		protocol_save_humidity_para();
//	}
	index = 0;
}

bool protocol_temp_para_changed()
{
	if(temp_para_changed)
	{
		temp_para_changed = false;
		return true;
	}
	return false;
}

bool protocol_humi_para_changed()
{
	if(humi_para_changed)
	{
		humi_para_changed = false;
		return true;
	}
	return false;
}
