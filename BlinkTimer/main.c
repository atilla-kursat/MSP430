#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    P1DIR |= BIT0;                  // P1.0 set as output
    P1OUT &= ~BIT0;                 // P1.0 set LOW (Red LED)
    TA0CCTL0 |= CCIE;               // Enable Timer0_A interrupts
    TA0CTL |= TASSEL_1 + MC_1;      // ACLK, Up Mode (Counts to TA0CCR0)
    TA0CCR0 = 10000;
    TA0CTL &= ~BIT0;
    int flag=0;
    while(1){

        if((TA0CTL & BIT0) == 1)
        {

        if(flag==0){
        P1OUT ^= BIT0;              // toggle P1.0
        TA0CTL &= ~BIT0;
        TA0CCR0 = 30000;

        while((TA0CTL & BIT0) == 0);
        P1OUT ^= BIT0;
        TA0CTL &= ~BIT0;
        flag=1;
        }

        else if(flag==1){
            P1OUT ^= BIT0;              // toggle P1.0
            TA0CTL &= ~BIT0;
            TA0CCR0 = 10000;
            while((TA0CTL & BIT0) == 0);
            P1OUT ^= BIT0;
            TA0CTL &= ~BIT0;
            flag=0;
            }
        }
    }
	return 0;
}
