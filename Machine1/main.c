#include <msp430.h> 
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#define ROUNDS 32
/**
 * main.c
 */
void uart_init();
void send_uart_char(const char *print);
void clear_buffer(char rxBuf[]);
void xtea_encrypt(unsigned char *v, unsigned long *k);
void xtea_decrypt(unsigned char *v, unsigned long *k);
void a5_init(void);
char uart_rx[100];
char uart_tx[4];
unsigned int volt=0;
volatile int temp[2];
volatile float diff;
volatile unsigned int i=0;
int dst_int;
int dst_flt;
float tmp_flt;
char dst_char[5];
char dst_flt_char[5];
volatile float distance;
unsigned long key[4] = {0x12345678, 0x9ABCDEF0, 0x2468ACE0, 0x13579BDF};


int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    BCSCTL1 = CALBC1_1MHZ;   // Set range
    DCOCTL  = CALDCO_1MHZ + DIVA_3;   // Set DCO step + modulation
   // BCSCTL2 &= ~(DIVS_3);           // SMCLK = DCO = 1MHz
    P1DIR |= BIT0 + BIT1 + BIT2;
    P1OUT=0x00;
    a5_init();
    P1DIR = BIT6;
      P1SEL |= BIT6;
      P2SEL = BIT1;


    //TA0CTL = ID_3+TACLR+TASSEL_2+MC_1; //timer initialization
    TA0CTL = TASSEL_2|MC_1 ;
     TA0CCR0 = 0xFFFF;
     TA0CCR1 = 0x000A;
     TA0CCTL1 = OUTMOD_7;
     TA1CTL = TASSEL_2|MC_2 ;
     TA1CCTL1 = CAP | CCIE | CCIS_0 | CM_3 | SCS ;



     UCA0CTL1 |= UCSWRST+UCSSEL_2;
         UCA0BR0 = 8;
         UCA0BR1 = 0;
         UCA0MCTL = UCBRS_0;
         UCA0CTL1 &= ~UCSWRST;

         uart_tx[0] = 1;
         uart_tx[3] = 0;
    //uart_init();
    __bis_SR_register(GIE);         // Enable interrupts.
    __delay_cycles(1500);


   while(1){

       ADC10CTL0 |= ENC + ADC10SC;
          // __bis_SR_register(CPUOFF + GIE);
           volt=(float) (ADC10MEM*3.4765); //Gerilim Formülü V = ADC10MEM * (~3,56v / 1023);

           distance = diff/58;
           dst_int = floor(distance);
           if(dst_int>254){
               uart_tx[1] = 255;
           }
           else{
               uart_tx[1] = dst_int;
           }
           if (volt < 1750){
               uart_tx[2] = 0;
           }
           else{
               uart_tx[2] = 1;
           }
           xtea_encrypt(uart_tx, key);
           send_uart_char(uart_tx);



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

void xtea_encrypt(unsigned char *v, unsigned long *k) {
    unsigned long v0 = v[0] | (v[1] << 8) | (v[2] << 16) | (v[3] << 24);
    unsigned long v1 = v[4] | (v[5] << 8) | (v[6] << 16) | (v[7] << 24);
    unsigned long sum = 0;
    unsigned long delta = 0x9E3779B9;
    int i;
    for (i = 0; i < ROUNDS; i++) {
        v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + k[sum & 3]);
        sum += delta;
        v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + k[(sum>>11) & 3]);
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

void a5_init()
{
  ADC10CTL0 &= ~ENC; //enable conversion (cevrim aktif deðil)
  ADC10CTL0 = ADC10SHT_3 + ADC10ON + ADC10IE; // örnek ve tuzma zamaný (sample-and-hold time x64), adc10 açýk, adc10 enable kesme
  ADC10CTL1 = INCH_5 + ADC10DIV_7; // input chanel kanal 5, acd10 clock soure select aclk seçili, 7 ye böl
  ADC10AE0  = BIT5; //analog input enable register
}


#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void) {
 // __bic_SR_register_on_exit(CPUOFF); // Wake up the CPU
}

#pragma vector = TIMER1_A1_VECTOR
__interrupt void Timer_A(void){

        temp[i] = TA1CCR1;
        i += 1;
        TA1CCTL1 &= ~CCIFG ;
        if (i==2) {
            diff=temp[i-1]-temp[i-2];
            i=0;
        }


}
