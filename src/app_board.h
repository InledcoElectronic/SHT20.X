/* 
 * File:   app_board.h
 * Author: liruya
 *
 * Created on 2018年6月23日, 下午4:38
 */

#ifndef APP_BOARD_H
#define	APP_BOARD_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <pic12f1572.h>
#include <pic.h>
#include "app_config.h"

#define SCL_PIN         GPIOA_PIN4
#define SDA_PIN         GPIOA_PIN5
#define TX_PIN          TXCK_PIN_RA0
#define RX_PIN          RXDT_PIN_RA1

#ifdef	__cplusplus
}
#endif

#endif	/* APP_BOARD_H */

