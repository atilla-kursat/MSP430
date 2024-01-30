#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

	P1DIR = 0x01;  //enabling LED
	P1REN = 0x08;  //enabling resistor to sense button presses
	P1OUT = 0x08;  //configuring P1.3 as button press sensor

	while(1){
	    if((0x08 & P1IN) == 0 ){  //when a button is pressed MCU will wait for its release to change the LED status.
	        while((0x08 & P1IN) == 0);
	        P1OUT ^= 0x01;
	    }


	}
	return 0;
}
