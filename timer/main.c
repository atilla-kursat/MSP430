#include <msp430.h>

int main() {

    /*** Watchdog timer and clock Set-Up ***/
    WDTCTL = WDTPW + WDTHOLD;       // Stop watchdog timer

    P1DIR |= BIT0;                  // P1.0 set as output
    P1OUT &= ~BIT0;                 // P1.0 set LOW

    TA0CCR0 |= 30000;                    // Counter value
    TA0CCTL0 |= CCIE;               // Enable Timer0_A interrupts
    TA0CTL |= TASSEL_1 + MC_1;      // ACLK, Up Mode

    while(1){
        if(TA0CTL & BIT0 == 1){
            P1OUT ^= BIT0;
            TA0CTL &= ~BIT0;
        }

    }
}


