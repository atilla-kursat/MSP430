#include <msp430.h> 
#include <stdlib.h>
#define ROUNDS 32
/**
 * main.c
 */

void xtea_decrypt(unsigned char *v, unsigned long *k);
void clear_buffer(char rxBuf[]);
void send_uart_char(const char *print);
void uart_init();
char uart_rx[100];
char encrypted_msg[4];
int pos=0;
int counter=0;
int inflag=0;
unsigned long key[4] = {0x12345678, 0x9ABCDEF0, 0x2468ACE0, 0x13579BDF};
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	BCSCTL1 = CALBC1_1MHZ;   // Set range
	    DCOCTL  = CALDCO_1MHZ;   // Set DCO step + modulation
	    BCSCTL2 &= ~(DIVS_3);           // SMCLK = DCO = 1MHz
	    __bis_SR_register(GIE);         // Enable interrupts.

	    TA0CTL = ID_3+TACLR+TASSEL_1+MC_1; //timer initialization
	    TA0CCR0 = 0xFFFF;
	    TA0CCTL0 = CCIE;

	    P1DIR |=  BIT0;           //P1.0 output
	    P1OUT &= ~BIT0;           //P1.0 = 0

	    clear_buffer(uart_rx);
	    uart_init();

	    while(1){

	    }


}

void send_uart_char(const char *print){
    //FUNCTION FOR SENDING STRING TO THE TX BUFFER
    unsigned int i = 0;
        while (print[i] != '\0')
        {
            while (!(IFG2 & UCA0TXIFG));
            UCA0TXBUF = print[i];
            i++;
        }
    __delay_cycles(500);
}

void uart_init(){//INITIALIZING UART FOR 115200 BAUD RATE
    //IN ACCORDANCE WITH THE MSP430G2553 DATASHEET
    UCA0CTL1 = UCSWRST;
    //-----------
    P1DIR = BIT6;
   // P1OUT = BIT6;
    //------------
    P1SEL = BIT1+BIT2;
    P1SEL2 = BIT1+BIT2;
    UCA0CTL1 |= UCSSEL_2;
    UCA0BR0 = 8;
    UCA0BR1 = 0;
    UCA0MCTL = UCBRS_6;
    UCA0CTL1 &=~UCSWRST;
    IE2 |= UCA0RXIE;
}

void xtea_decrypt(unsigned char *v, unsigned long *k) {
    unsigned long v0 = v[0] | (v[1] << 8) | (v[2] << 16) | (v[3] << 24);
    unsigned long v1 = v[4] | (v[5] << 8) | (v[6] << 16) | (v[7] << 24);
    unsigned long delta = 0x9E3779B9;
    unsigned long sum = delta * ROUNDS;
    int i;
    for (i = 0; i < ROUNDS; i++) {
        v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + k[(sum>>11) & 3]);
        sum -= delta;
        v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + k[sum & 3]);
    }
    v[0] = v0 & 0xff;
    v[1] = (v0 >> 8) & 0xff;
    v[2] = (v0 >> 16) & 0xff;
    v[3] = (v0 >> 24) & 0xff;
    v[4] = v1 & 0xff;
    v[5] = (v1 >> 8) & 0xff;
    v[6] = (v1 >> 16) & 0xff;
    v[7] = (v1 >> 24) & 0xff;
}

void clear_buffer(char rxBuf[]){
    //FUNCTION FOR CLEARING THE RX BUFFER FOR FUTURE DATA TO BE LOCATED IN THE SAME PLACE
    volatile int i;
    for(i= 99;i>=0;i--)
    {
        rxBuf[i] = '\0';
    }
    pos = 0;
}


void dataArrived(){

    xtea_decrypt(encrypted_msg, key);
    send_uart_char(encrypted_msg);
}

#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR (void){
    //RX BUFFER INTERRUPT FOR INCOMING DATA FROM THE UART
    encrypted_msg[pos] = UCA0RXBUF;
    counter++;
    pos++;
    if(counter==4){
         dataArrived();
         clear_buffer(uart_rx);
        counter = 0;
    }
}
