#include <msp430.h> 


void init_UART(void);
void putString_noInterrupts(char* message);

/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

	init_UART();

	PJDIR |= BIT0;
	PJOUT &= ~BIT0;

	char message[] = "Hello World\r\n";
	//putString_noInterrupts(message);

	_enable_interrupts();
	for(;;);
	return 0;
}

void init_UART(void) {
    UCA0CTLW0 |= UCSWRST + UCSSEL__SMCLK;   // Initially set UART A1 to reset and select SMCLK 1Mhz clk

    UCA0BRW = 0x6;
    UCA0MCTLW |= UCOS16 + 0x2080; // config for baud 9600 with 1Mhz clk. OCOS16 - 1, UCBRF = 0x80 (Lower Byte) and UCBRS = 0x20 (Upper Byte)

    P2SEL1 |= BIT1 + BIT0;       // Pin 5 and 6 are UCA1 TXD and RXD respectively
    P2SEL0 &= ~(BIT1 + BIT0);

    PM5CTL0 &= ~LOCKLPM5;

    UCA0CTLW0 &= ~UCSWRST;  // Clear UART reset

    /* Interrupts */
    UCA0IE |= UCRXIE; // Enable Interrupts for RX
    UCA0IFG &= ~UCRXIFG; // Clear Interrupt flag for RX

}

#pragma vector = USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void) {
    char c = 0;
    switch(__even_in_range(UCA0IV,18)) {
    case 0x00: // No interrupts
        break;
    case 0x02: // Vector 2: UCRXIFG
        c = UCA0RXBUF;
        UCA0TXBUF = c;

        if (c == 'm') PJOUT ^= BIT0;
        break;
    case 0x03: // Vector 4: UCTXIFG
        break;
    default:
        break;
    }
}

void putString_noInterrupts(char* message) {
    int i = 0;
    int x = 0;
    for (i = 0; i < sizeof(message); i++) {
        UCA0TXBUF = message[i]; // Put character in buffer

        for (x = 0; x < 10000; x++); // Delay
    }
}
