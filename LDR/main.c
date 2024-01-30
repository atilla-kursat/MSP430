#include "msp430.h"

void a5_init(void);


unsigned int volt=0;

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;

  DCOCTL=CALDCO_1MHZ;
  BCSCTL1=CALBC1_1MHZ + DIVA_3;

 P1DIR |= BIT0 + BIT1 + BIT2;


  P1OUT=0x00;

  P1DIR = BIT6;
   P1SEL |= BIT6;
  a5_init();

  __delay_cycles(500);
  for (;;)
  {

    ADC10CTL0 |= ENC + ADC10SC;
    __bis_SR_register(CPUOFF + GIE);

    volt=(float) (ADC10MEM*3.4765); //Gerilim Formülü V = ADC10MEM * (~3,56v / 1023)



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
  ADC10CTL1 = INCH_5 + ADC10SSEL_1 + ADC10DIV_7; // input chanel kanal 5, acd10 clock soure select aclk seçili, 7 ye böl
  ADC10AE0  = BIT5; //analog input enable register
}
