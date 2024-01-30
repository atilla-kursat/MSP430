#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	volatile int kursat,akcay,i;
	kursat=51;
	for( i=0;i<4;i++){
	kursat=kursat<<1;
	}
	akcay= kursat & 0x30;
	
	return 0;
}
