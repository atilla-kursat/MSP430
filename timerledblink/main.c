#include <msp430.h> 

int flag=0;
/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    P1DIR |= BIT0;                  // P1.0 set as output
    P1OUT &= ~BIT0;                 // P1.0 set LOW (Red LED)
    TA0CCTL0 |= CCIE;               // Enable Timer0_A interrupts
    TA0CTL |= TASSEL_1 + MC_1;      // ACLK, Up Mode (Counts to TA0CCR0)
    TA0CCR0 = 10000;
    TA0CTL &= ~BIT0;
    _BIS_SR(GIE);

    while(1);

}


#pragma vector=TIMER0_A0_VECTOR

__interrupt void Timer_A (void){
    if(flag==0){
            P1OUT ^= BIT0;              // toggle P1.0
            TA0CCR0 = 30000;
            flag++;
    }
    else if(flag==1){

        P1OUT ^= BIT0;
        flag++;
    }

    else if(flag==2){
        P1OUT ^= BIT0;              // toggle P1.0
        TA0CCR0 = 10000;
        flag++;

    }


    else if(flag==3){

        P1OUT ^= BIT0;
        flag=0;
    }


            }

