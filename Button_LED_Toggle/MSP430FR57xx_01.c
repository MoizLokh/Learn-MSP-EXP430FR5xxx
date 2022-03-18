/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2012, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************
 * 
 *                       MSP430 CODE EXAMPLE DISCLAIMER
 *
 * MSP430 code examples are self-contained low-level programs that typically
 * demonstrate a single peripheral function or device feature in a highly
 * concise manner. For this the code may rely on the device's power-on default
 * register values and settings such as the clock configuration and care must
 * be taken when combining code from several examples to avoid potential side
 * effects. Also see www.ti.com/grace for a GUI- and www.ti.com/msp430ware
 * for an API functional library-approach to peripheral configuration.
 *
 * --/COPYRIGHT---*/
//******************************************************************************
//  MSP430FR57x Demo - Toggle P1.0 using software
//
//  Description: Toggle P1.0 using software.
//  ACLK = n/a, MCLK = SMCLK = TACLK = default DCO = ~625 KHz
//
//           MSP430FR5739
//         ---------------
//     /|\|               |
//      | |               |
//      --|RST            |
//        |               |
//        |           P1.0|-->LED
//
//   Priya Thanigai
//   Texas Instruments Inc.
//   August 2010
//   Built with IAR Embedded Workbench Version: 5.10 & Code Composer Studio V4.0
//******************************************************************************

#include <msp430.h>
#include "Drivers/LED.h"
#include <stdio.h>

#define BUTTON1 BIT0

void button_init(void);


int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT  

  button_init();
  LED_init();
  
  _BIS_SR(LPM0_bits+GIE); // Enter low power mode until interrupt occurs
}

void button_init(void) {
    P4DIR &= ~BUTTON1; // Set button pin as an input (0)
    P4OUT |= BUTTON1; // Set a pull up resistor for button1
    P4REN |= BUTTON1; // Enable pull up resistor for button to keep pin high until pressed
    P4IES |= BUTTON1; // Enable interrupt to trigger on falling edge (high(unpressed) to low(pressed) )
    P4IFG &= ~BUTTON1; // Clear the interrupt
    P4IE |= BUTTON1; // Enable interrupts on port 4
}

#pragma vector=PORT4_VECTOR
__interrupt void PORT4_ISR(void) {
    P4IFG &= ~BUTTON1; // Clears the interrupt flag
    P4IE &= ~BUTTON1; // Disable button interrupts on port 4

    WDTCTL = WDT_MDLY_32; // Start and set watchdog timer
    SFRIFG1  &= ~WDTIFG; // Clear interrupt for WDT
    SFRIE1  |= WDTIE; // Enable WDT interrupt

    toggle_LED(1);
    P4IES ^= BUTTON1;
}

#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR(void) {
    SFRIE1  &= ~WDTIE;
    SFRIFG1 &= ~WDTIFG;

    WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
    P4IE |= BUTTON1;    // Re-enable interrupts for the button
}
