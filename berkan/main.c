#include <msp430.h> 
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;
    P1DIR |= BIT0;
    P1OUT &= ~BIT0;
    TA0CCTL0 |= CCIE;
    TA0CTL |= TASSEL_1 + MC_1;
    TA0CCR0 = 9999;
    TA0CTL &= ~BIT0;
    int var=0;
    while(1){
        if((TA0CTL & BIT0) == 1)
        {
        if(var==0){
        P1OUT ^= BIT0;
        TA0CTL &= ~BIT0;
        TA0CCR0 = 9999;
        while((TA0CTL & BIT0) == 0);
        P1OUT ^= BIT0;
        TA0CTL &= ~BIT0;
        var=1;
        }
        else if(var==1){
            P1OUT ^= BIT0;
            TA0CTL &= ~BIT0;
            TA0CCR0 = 9999;
            while((TA0CTL & BIT0) == 0);
            P1OUT ^= BIT0;
            TA0CTL &= ~BIT0;
            var=0;
            }
        }
    }
    return 0;
}

