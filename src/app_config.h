/* 
 * File:   app_config.h
 * Author: liruya
 *
 * Created on 2018年6月22日, 下午3:53
 */

#ifndef APP_CONFIG_H
#define	APP_CONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include "drv_common.h"
#include "drv_osc.h"
#include "drv_gpio.h"

    /*
     *  custom modify config
     */
    
    // PIC12F1572 Configuration Bit Settings
    // 'C' source line config statements

    // CONFIG1
#pragma config FOSC = INTOSC    //  (INTOSC oscillator; I/O function on CLKIN pin)
#pragma config WDTE = ON        // Watchdog Timer Enable (WDT enabled)
#pragma config PWRTE = ON       // Power-up Timer Enable (PWRT enabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable (Brown-out Reset disabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)

    // CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN = OFF      // PLL Enable (4x PLL disabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LPBOREN = OFF    // Low Power Brown-out Reset enable bit (LPBOR is disabled)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled)
    
    // #pragma config statements should precede project file includes.
    // Use project enums instead of #define for ON and OFF.

//==============================================================================
// project config start
//==============================================================================
#define _XTAL_FREQ          16000000
//#define APP_DEBUG_ENABLED
    
#define EUSART_ENABLED
#define SWIIC_ENABLED
//#define TMR0_ENABLED
//#define TMR1_ENABLED
#define TMR2_ENABLED
#define HEF_ENABLED
    
//==============================================================================    
// project config end
//==============================================================================    
    
#ifndef	_XTAL_FREQ
#error	"Please define _XTAL_FREQ."
#endif

#if defined(APP_DEBUG_ENABLED) || defined(EUSART_ENABLED)
#include "drv_eusart.h"
#endif
    
#ifdef  SWIIC_ENABLED
#include "sw_iic.h"
#endif
    
#ifdef TMR0_ENABLED
#include "drv_tmr0.h"
#endif
    
#ifdef TMR1_ENABLED
#include "drv_tmr1.h"
#endif
    
#ifdef TMR2_ENABLED
#include "drv_tmr2.h"
#endif
    
#ifdef HEF_ENABLED
#include "drv_hef.h"
#endif
    
#ifdef	__cplusplus
}
#endif

#endif	/* APP_CONFIG_H */

