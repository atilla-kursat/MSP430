#include "msp430.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
void a5_init(void);

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
void ser_output(char *str);
void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;

  DCOCTL=CALDCO_1MHZ;
  BCSCTL1=CALBC1_1MHZ + DIVA_3;
  P1DIR |= BIT0 + BIT1 + BIT2;
  P1OUT=0x00;
  a5_init();
  P1DIR = BIT6;
  P1SEL |= BIT6;
  P2SEL = BIT1;
  TA0CTL = TASSEL_2|MC_1 ;
  TA0CCR0 = 0xFFFF;
  TA0CCR1 = 0x000A;
  TA0CCTL1 = OUTMOD_7;
  TA1CTL = TASSEL_2|MC_2 ;
  TA1CCTL1 = CAP | CCIE | CCIS_0 | CM_3 | SCS ;
  __bis_SR_register(GIE);
  __delay_cycles(1500);
  for (;;)
  {



    ADC10CTL0 |= ENC + ADC10SC;
    __bis_SR_register(CPUOFF + GIE);
    volt=(float) (ADC10MEM*3.4765); //Gerilim Formülü V = ADC10MEM * (~3,56v / 1023);


    distance = diff/58;
          dst_int = floor(distance);
          tmp_flt = distance - dst_int;
      if (tmp_flt < 0.01) {
      dst_flt = floor(tmp_flt * 1000);


                  }
           else if (tmp_flt < 0.1) {
                      dst_flt = floor(tmp_flt * 100);
                      }
           else {
                      dst_flt = floor(tmp_flt * 100);




  }
}
}
// ADC10 Kesme Vektörü
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
  __bic_SR_register_on_exit(CPUOFF);        // Clear CPUOFF bit from 0(SR)
}

void a5_init()
{
  ADC10CTL0 &= ~ENC; //enable conversion (cevrim aktif deðil)
  ADC10CTL0 = ADC10SHT_3 + ADC10ON + ADC10IE; // örnek ve tuzma zamaný (sample-and-hold time x64), adc10 açýk, adc10 enable kesme
  ADC10CTL1 = INCH_5 + ADC10DIV_7; // input chanel kanal 5, acd10 clock soure select aclk seçili, 7 ye böl
  ADC10AE0  = BIT5; //analog input enable register
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
