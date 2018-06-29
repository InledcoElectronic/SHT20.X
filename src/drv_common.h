/* 
 * File:   drv_common.h
 * Author: liruya
 *
 * Created on 2018年6月22日, 下午2:34
 */

#ifndef DRV_COMMON_H
#define	DRV_COMMON_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include <pic12f1572.h>
#include <pic.h>

#include "app_config.h"
    
#define global_interrupt_enable()       INTCONbits.GIE = 1
#define global_interrupt_disable()      INTCONbits.GIE = 0
#define peripheral_interrupt_enable()   INTCONbits.PEIE = 1
#define peripheral_interrupt_disable()  INTCONbits.PEIE = 0

#ifdef	__cplusplus
}
#endif

#endif	/* DRV_COMMO_H */

