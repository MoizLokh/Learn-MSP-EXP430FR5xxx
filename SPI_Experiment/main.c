#include <msp430.h> 


/**
 * main.c
 */

int count = 0;

void init_SPI(void);


int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	init_SPI();
	PJDIR |= BIT0;
	PJOUT &= ~BIT0;

	__enable_interrupt();
	for(;;);
}


/**
 * Using the UCA0 peripheral for SPI in 3 pin mode
 * Baud rate - 9600
 */
void init_SPI(void) {

    //-- Initializing all eUSCI registers

    UCA0CTLW0 |= UCSWRST;       // Enter reset mode
    UCA0CTLW0 |= UCSSEL__SMCLK; // Set clk to 1 MHz
    UCA0CTLW0 |= UCSYNC;        // Synchronous mode (SPI)
    UCA0CTLW0 |= UCMST;         // Master mode


//    //-- Setup Baud rate
//
//    UCA0BRW |= 0x6;
//    UCA0MCTLW |= UCOS16 + 0x2080; // config for baud 9600 with 1Mhz clk. OCOS16 - 1, UCBRF = 0x80 (Lower Byte) and UCBRS = 0x20 (Upper Byte)

    //-- Port configuration

    /* P2.1 for UCA0SOMI */
    P2SEL1 |= BIT1;
    P2SEL0 &= ~BIT1;

    /* P2.0 for UCA0SIMO */
    P2SEL1 |= BIT0;
    P2SEL0 &= ~BIT0;

    /* P1.5 for UCA0CLK */
    P1SEL1 |= BIT5;
    P1SEL0 &= ~BIT5;


    UCA0CTLW0 &= ~UCSWRST;       // Move out of reset mode before enabling interrupts

    //-- Enable Interrupts

    UCA0IE |= UCTXIE;           // Enable TX interrupts
    UCA0IFG |= UCTXIFG;         // Clear interrupt flag

}

#pragma vector = USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void) {
    PJOUT ^= BIT0;
    switch(__even_in_range(UCA0IV,18)) {
    case 0x00:  // No interrupt
        break;
    case 0x02:  // UCRXIFG
        break;
    case 0x04:  // UCTXIFG
        count++;
        if (count < 10000) UCA0TXBUF= 0x09;
        else {
            count = 0;
            UCA0IFG &= ~UCTXIFG;
        }
        break;
    default:
        break;
    }
}
