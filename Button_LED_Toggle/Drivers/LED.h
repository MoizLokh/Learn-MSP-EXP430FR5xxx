/*
 * LED.h
 *
 *  Created on: Mar. 17, 2022
 *      Author: mlokh
 */

#ifndef DRIVERS_LED_H_
#define DRIVERS_LED_H_

#include <msp430.h>

#define LED1 BIT0
#define LED2 BIT1
#define LED3 BIT2
#define LED4 BIT3
#define LED5 BIT4
#define LED6 BIT5
#define LED7 BIT6
#define LED8 BIT7




void LED_init(void);
void LED_ON(unsigned char led_n);
void LED_OFF(unsigned char led_n);

void toggle_LED(unsigned char led_n);


#endif /* DRIVERS_LED_H_ */
