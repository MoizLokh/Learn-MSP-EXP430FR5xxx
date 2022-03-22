# Learning Objective

- Learn about UART serial communication
- Understand how to use the eUSCI UART peripheral on the MSP430
- Able to handle UART TX and RX interrupts

# Resources

- Video series on UART in MSP430 (https://www.youtube.com/watch?v=p_YOsh7BSDE)

# Things to note

## Procedure to setup UART peripheral

1. Set the UCSWRST bit in UCAxCTLW0. This puts eUSCI_Ax (UART peripheral) into reset.
2. Initialize all eUSCI_Ax configuration
3. Set baud rate
4. Configure ports
5. Clear UCSWRST (to take eUSCI_Ax peripheral out of reset)
6. Enable interrupts using the UCRXIE (RX interrupt enable bit) and UCTXIE (TX interrupt enable bit) in UCAxIE configuration register
7. Transmit by sending data through UCATXBUFx (8 bits) and Receive from UCARXBUFx (8 bits)

## eUSCI_Ax registers (x is either 0 or 1 as there are two UART peripheral)

- UCAxCTLW0, UART control register word 0
  - UCSSELx - Select the CLK source, UCLK by default (00b)
  - UCSYNC - Either Async (0) or Sync mode (1), By default (0)
  - UCMODEx - UART mode by default (00b)
  - UART Frame, Parity bit, character length, stop bit etc - all set by default (0)
    - UCPEN
    - UCPAR
    - UCMSB
    - UC7BIT
    - UCSPB
  - UCSWRST - Puts UART into reset while we configure (1 - enable reset)

## Port configuration for UART

Initially the GPIO ports are set to be I/O. But we can use the port select to change them to alternate functions such as UART.

If you want to send data to the computer we use eUSCI_A1 and those pins are P2.1 (UCA0TXD) and P2.0 (UCA0RXD).

- P2SEL1 - is 00 by default (I/O) we set this to 11
  - P2SEL1 |= BIT1 + BIT0
- P2SEL0 - is cleared to 00
  - P2SEL0 &= ~(BIT1 + BIT0)

## Baud rate configuration

- UCAxBRW - Baud rate control word for eUSCI_Ax
  - Value that divides the CLK to form the baud rate
  - If CLK is 1 MHz then UCAxBRW value = 1MHz/requiredBaudRate
- UCAxMCTLW - The UCAxBRW might not be accurate due to rounding therefore we modulate the CLK to account for it
  - UCOS16 bit is either set to 1 (oversampling mode) or 0
  - UBRFx field
  - UCBRSx field

## Interrupts for TX and RX

The TX or RX interrupts can be enabled by setting the UCRXIE and UCTXIE field in the UCAxIE register.

Note: Always enable iterrupts for UART **after** removing UART from reset mode. Failure to do so will cause the interrupt enable bit to reset after setting it.
