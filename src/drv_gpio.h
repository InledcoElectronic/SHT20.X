/* 
 * File:   drv_gpio.h
 * Author: liruya
 *
 * Created on 2018年6月22日, 下午2:36
 */

#ifndef DRV_GPIO_H
#define	DRV_GPIO_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <pic12f1572.h>

    typedef enum _gpio_pin {
        GPIOA_PIN0,
        GPIOA_PIN1,
        GPIOA_PIN2,
        GPIOA_PIN3,
        GPIOA_PIN4,
        GPIOA_PIN5
    } gpio_pin_t;

    typedef enum _rxdt_pin {
        RXDT_PIN_RA1,
        RXDT_PIN_RA5
    } rxdt_pin_t;

    typedef enum _cwga_pin {
        CWGA_PIN_RA2,
        CWGA_PIN_RA5
    } cwga_pin_t;

    typedef enum _cwgb_pin {
        CWGB_PIN_RA0,
        CWGB_PIN_RA4
    } cwgb_pin_t;

    typedef enum _t1g_pin {
        T1G_PIN_RA4,
        T1G_PIN_RA3
    } t1g_pin_t;

    typedef enum _txck_pin {
        TXCK_PIN_RA0,
        TXCK_PIN_RA4
    } txck_pin_t;

    typedef enum _pwm2_pin {
        PWM2_PIN_RA0,
        PWM2_PIN_RA4
    } pwm2_pin_t;

    typedef enum _pwm1_pin {
        PWM1_PIN_RA1,
        PWM1_PIN_RA5
    } pwm1_pin_t;

    extern void gpio_set_analog(gpio_pin_t pin);
    extern void gpio_set_input(gpio_pin_t pin, bool pullup);
    extern void gpio_set_output(gpio_pin_t pin, bool level);
    extern void gpio_rxdt_select(rxdt_pin_t pin);
    extern void gpio_cwga_select(cwga_pin_t pin);
    extern void gpio_cwgb_select(cwgb_pin_t pin);
    extern void gpio_t1g_select(t1g_pin_t pin);
    extern void gpio_txck_select(txck_pin_t pin);
    extern void gpio_pwm2_select(pwm2_pin_t pin);
    extern void gpio_pwm1_select(pwm1_pin_t pin);

#ifdef	__cplusplus
}
#endif

#endif	/* DRV_GPIO_H */

