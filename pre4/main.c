#include <msp430.h>
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                     // Stop Watchdogtimer
    P1DIR |= BIT0;                                // P1.0 output
    P1OUT &= ~BIT0; // turn off the LED

    P1REN |= BIT3; // configure P1.2 as input Switch connection
    P1IE |= BIT3; // enable P1.2 interrupt
    P1IES |= BIT3; // configure the edge of the interrupt
    P1IFG &= ~BIT3; // clear the interrupt flag
    __bis_SR_register(GIE);
    while (1)
    {
    }
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1_IntHandler(void)
{
    P1OUT ^= BIT0; // toggle LED
    P1IES ^= BIT3; // toggle the edge of the interrupt
    P1IFG &= ~BIT3; // clear interrupt flag
}
