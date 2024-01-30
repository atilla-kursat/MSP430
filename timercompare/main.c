#include<msp430.h>

int main(void){

  WDTCTL = WDTPW + WDTHOLD;                // Stop WDT

  BCSCTL1 = CALBC1_1MHZ;
  BCSCTL2 = DIVA_3;

  P1DIR |= 0x01;                           // P1.0 output


  TA0CCTL0 |= CCIE;                            // CCR0 interrupt enabled

  TA0CCR0 = 1000;

  TA0CTL = TASSEL_1 | MC_1 | ID_3;                 // SMCLK, contmode

   _BIS_SR(GIE);                // Enter LPM0 w/ interrupt

}

// Timer A0 interrupt service routine

#pragma vector=TIMER0_A0_VECTOR

__interrupt void Timer_A (void){
    P1OUT ^= BIT0; // toggle LED
    if(TA0CCR0>=32000){
        TA0CCR0=1000;

    }
    else{
        TA0CCR0 = TA0CCR0 * 2;
    }
}
