#include "msp430G2553.h"


/**
 * main.c
 */
#define G_LED BIT6
#define R_LED BIT0

int i,j,delay;

void delay_time(int);
void delay_long(int);

void main(void){
WDTCTL = WDTPW + WDTHOLD; // disable watch dog timer
DCOCTL = CALDCO_16MHZ;    // set internal oscillator at 16MHz
BCSCTL1 = CALBC1_16MHZ;   // set internal oscillator at 16MHz

P1OUT = 0x00;
P2OUT = 0x00;
P1DIR = 0xFF; // Set all as outputs
P2DIR = 0xFF; // Set all as outputs

    for(;;) {


    P1OUT |= G_LED;      // Enable = 1
    delay_long(delay);
    P1OUT &= ~(G_LED);   // Disable = 0
    delay_long(delay);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------
//-----------------  Functions & Procedures  -----------  Functions & Procedures  -----------  Functions & Procedures  -------------
//----------------------------------------------------------------------------------------------------------------------------------

void delay_time (int delay){      //----------------------------------------- Delay---------------------

     do delay--;
     while (delay != 0);
}

void delay_long (int delay){            //--------------------------------------- Delay long------------------

            int j;
            j = 100;
         do{
            delay_time(5000);
         j--;
         }while (j != 0);
}
