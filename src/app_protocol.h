/* 
 * File:   app_protocol.h
 * Author: liruya
 *
 * Created on 2018年6月25日, 下午2:00
 */

#ifndef APP_PROTOCOL_H
#define	APP_PROTOCOL_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "app_common.h"
    
    extern void protocol_init();
    extern void protocol_receive(uint8_t rcv);
    extern void protocol_decode_frame();
    extern bool protocol_temp_para_changed();
    extern bool protocol_humi_para_changed();
    extern void protocol_send_temperature(int16_t temp);
    extern void protocol_send_humidity(int16_t humi);

#ifdef	__cplusplus
}
#endif

#endif	/* APP_PROTOCOL_H */

