#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
  DCOCTL = 0; // Select lowest DCOx and MODx settings
  BCSCTL1 = CALBC1_1MHZ; // Set DCO to 1MHz
  DCOCTL = CALDCO_1MHZ;

  P1SEL = BIT1 + BIT2; // P1.1 = RXD, P1.2=TXD
  P1SEL2 = BIT1 + BIT2; // P1.1 = RXD, P1.2=TXD
  UCA0CTL1 |= UCSSEL_2; // Use SMCLK
  UCA0BR0 = 104; // Set baud rate to 9600 with 1MHz clock (Data Sheet 15.3.13)
  UCA0BR1 = 0; // Set baud rate to 9600 with 1MHz clock
  UCA0MCTL = UCBRS0; // Modulation UCBRSx = 1
  UCA0CTL1 &= ~UCSWRST; // Initialize USCI state machine
  IE2 |= UCA0RXIE; // Enable USCI_A0 RX interrupt

  __bis_SR_register(LPM0_bits + GIE); // Enter LPM0, interrupts enabled

  return 0;
}

// Echo back RXed character, confirm TX buffer is ready first
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
  while (!(IFG2 & UCA0TXIFG)); // Wait for TX buffer to be ready
  UCA0TXBUF = UCA0RXBUF; // Echo back character
}
