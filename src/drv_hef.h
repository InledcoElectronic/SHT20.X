/* 
 * File:   drv_hef.h
 * Author: liruya
 *
 * Created on 2018年6月25日, 下午3:35
 */

#ifndef DRV_HEF_H
#define	DRV_HEF_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "drv_common.h"
    
    extern void hef_erase_block(uint16_t addr);
    extern void hef_write_block(uint16_t addr, const uint8_t *pbuf);
    extern void hef_write_byte(uint16_t addr, uint8_t byte);
    extern uint8_t hef_read_byte(uint16_t addr);
    extern void hef_write_buffer(uint16_t addr, const uint8_t *pbuf, uint8_t len);
    extern void hef_read_buffer(uint16_t addr, uint8_t *pbuf, uint8_t len);

#ifdef	__cplusplus
}
#endif

#endif	/* DRV_HEF_H */

