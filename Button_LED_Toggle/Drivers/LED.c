/*
 * LED.c
 *
 *  Created on: Mar. 17, 2022
 *      Author: mlokh
 */

#include "LED.h"

int leds[8] = {LED1, LED2, LED3, LED4, LED5, LED6, LED7, LED8};

void LED_init(void) {
    int i = 0;
    for (i = 7; i >= 0; i--) {
        if (i <= 3) {
            PJDIR |= leds[i];
            PJOUT &= ~leds[i];
        } else {
            P3DIR |= leds[i];
            P3OUT &= ~leds[i];
        }
    }
}

void LED_ON(unsigned char led_n) {

    if (led_n >= 1 && led_n <= 4) {
        PJOUT |= leds[--led_n];
    }
    else if (led_n >=5 && led_n <= 8) {
        P3OUT |= leds[--led_n];
    }
}

void LED_OFF(unsigned char led_n) {

    if (led_n >= 1 && led_n <= 4) {
        PJOUT &= ~leds[--led_n];
    }
    else if (led_n >=5 && led_n <= 8) {
        P3OUT &= ~leds[--led_n];
    }
}

void toggle_LED(unsigned char led_n) {

    if (led_n >= 1 && led_n <= 4) {
        PJOUT ^= leds[--led_n];
    }
    else if (led_n >=5 && led_n <= 8) {
        P3OUT ^= leds[--led_n];
    }
}



