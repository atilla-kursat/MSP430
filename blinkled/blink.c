#include <msp430.h>				


/**
 * blink.c
 */

void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;		// stop watchdog timer
	P1DIR |= BIT0;					// configure P1.0 as output

	volatile unsigned int i;		// volatile to prevent optimization
	int flag=0;                     //flag to determine whether the blink duration should be long or short
	int j;

	while(1)
	{
	    if(flag==0){
	        for( j=0;j<2;j++){
	            P1OUT ^= BIT0;				// toggle P1.0
	            for(i=10000; i>0; i--);     // delay
	    }
	    flag=1;
	    }

	    else if(flag==1){
	        for(j=0;j<2;j++){
	            P1OUT ^= BIT0;              // toggle P1.0
	            for(i=100000; i>0; i--);     // delay
	        }
	        flag=0;
	    }


	}
}
