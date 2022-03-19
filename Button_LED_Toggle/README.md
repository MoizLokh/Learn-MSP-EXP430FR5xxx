# Learning Objective

- Understand how to use Code Composer Studios
- Understand the MSP-EXP430FR5739 board and its pherephrials
- Learn to build and debug C code on the board

# Resources

- Good tutorial (Contains a section for UART and interrupts aswell): http://www.simplyembedded.org/tutorials/
- Interrupt: https://bennthomsen.wordpress.com/engineering-toolbox/ti-msp430-launchpad/using-the-switches/
- LED: https://scienceprog.com/running-leds-on-msp-exp430fr5739-fram-board/

# Things to note

- The LED demo 01 will not work on this board as the port being used is not connected to the LED. Instead of P1 use PJ (LED1-LED4) and P3 (LED5-LED8)
  ![image](https://user-images.githubusercontent.com/71032077/158940301-112b4f6f-1503-41d4-b6d1-7aad09f8947c.png)

## Watchdog timer

- 32-bit counter that will cause an interrupt to perform a controlled system restart when a software issue occurs.
- The user must set up, halt or clear the watchdog timer before it expires to avoid the system from reseting
- If it is not needed it can be disabled: WDTCTL = WDTPW (timer password) + WDTHOLD (1 means timer is stopped)
- Uses two bits in the SFRs for interrupt control
  - WDT interrupt flag, WDTIFG, in SFRIFG1.0
  - WDT interrupt enable, WDTIE, located in SFRIE1.0

## Clocks

- MCLK: The master clock which is used by the CPU
- SMCLK: Clk used by peripherals
- ACLK: Auxilary clock used by peripherals
