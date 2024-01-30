#include <msp430.h>
/**
* main.c
*/
int main(void)
{
WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer
volatile unsigned int mynumber = 0x1A2B, query;
query = mynumber & (BIT1 | BIT8);
return 0;
}
