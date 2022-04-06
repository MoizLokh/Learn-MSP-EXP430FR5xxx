#include <msp430.h> 

void init_timer(void);
void init_stoptimer(void);

/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	PJDIR |= BIT0;
	PJOUT &= ~BIT0;

	init_timer();
	init_stoptimer();

	_BIS_SR(LPM0_bits + GIE);
	for(;;);
	return 0;
}

void init_stoptimer() {
    //-- Timer_A Main timer 4s
    TA1CCR0 = 64000 - 1;

    TA1CCTL0 |= CCIE;

    //-- ACLK (32 khz) / 2, MC - UP, Timer Interrupt enabled
    TA1CTL |= TASSEL_1 + MC_1 + ID_1;
}

void init_timer(void) {

    //-- Port configuration --

    //-- P1.4 TB0.1 Output
    P1DIR |= BIT4;
    P1SEL1 &= ~BIT4;
    P1SEL0 |= BIT4;
    P1OUT |= BIT4;

    //-- Timer_B Main timer 2s
    TB0CCR0 = 64000 - 1;

    //-- Timer_B compare register - 1s
    TB0CCR1 = 32000;

    //-- OUTMOD - Toggle/set
    TB0CCTL1 |= OUTMOD_6;

    //-- ACLK (32 khz), MC - Continuous
    TB0CTL |= TBSSEL_1 + MC_2;
}

#pragma vector = TIMER1_A0_VECTOR;
__interrupt void TIMER1_A0_ISR (void) {

    PJOUT ^= BIT0;

    //-- Hault Timer B
    TB0CTL &=  ~MC_2;

    //-- Hault Timer A
    TA1CTL &= ~MC_1;

    //-- Clear flag
    TA1CTL &= ~TBIFG;
}
