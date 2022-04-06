# Learning Objective

- Learn about SPI serial communication
- Able to handle transmition and receive

# Resources

- Video series on UART in MSP430 (https://www.youtube.com/watch?v=2J8_dpnaBOk)
- https://www.argenox.com/library/msp430/msp430-spi-peripheral-chapter-9/

# Things to note

The SPI protocol has 3 data lines when communicating between devices

1. SCLK - Clock that is shared from the master to the slave device
2. SIMO or MOSI - Slave in / Master out
3. SOMI or MISO - Slave out / Master in
4. STE - Slave transmit enable (used in 4 pin mode to select between multiple slave devices. Controled by Master)

The eUSCI_A and B we used for UART also support SPI communication protocol. SPI mode is set when the UCSYNC bit is set

## Procedure to setup SPI peripheral

1. Set UCSWRST in the UCxCTL1 register. Sets it to reset state.
2. Initialize all eUSCI registers
   - UCCKPH and UCCKPL need to be set depending on the slave device (When the rising edge of the clk should be?)
3. Configure the ports (The pin should be in SPI mode instead of GPI0 which is the default)
4. Ensure that UCxSOMI (Master mode) have settled to final voltage levels
5. Clear the UCSWRST bit - Out of reset mode
6. Enable interrupts with UCRXIE or UCTXIE

## Interrupts for TX and RX

1. The UCTXIFG interrupt flag is set by the tranmitter to indicate taht UCxTXBUF is ready to accept values. Flag automatically resets when written into UCxTXBUF.
2. The eUSCI interrupt flags are prioritized and combined to a single interrupt vector. The interrupt vector register UCxIV is used to determind what caused the interrupt.

The TX or RX interrupts can be enabled by setting the UCRXIE and UCTXIE field in the UCAxIE register.

Note: Always enable iterrupts for SPI **after** removing SPI from reset mode. Failure to do so will cause the interrupt enable bit to reset after setting it.
