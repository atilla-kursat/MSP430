
#include "msp430.h"
void main(void)
{
    WDTCTL =  WDT_ADLY_1000;  //using ACLK with 1000ms
    P1DIR |=  BIT0;           //P1.0 output
    P1OUT = ~BIT0;           //P1.0 turned off


    while(1){
        if((IFG1 & BIT0) == WDTIFG){//checks if interrupt is generated by WDT
            P1OUT ^= BIT0;//toggle LED
            IFG1 = ~BIT0;//clear interrupt
        }
    }
}


