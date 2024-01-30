#include <msp430.h> 
#include "lcdd.h"
#include <stdlib.h>

/**
 * main.c
 */
void uart_init();
void send_uart_char(const char *print);
void clear_buffer(char rxBuf[]);
void dataArrived();
void displaymsg();
char uart_rx[100];
char lcd_first_line[16];
char lcd_second_line[16];
int pos=0;
int counter=0;
int inflag=0;
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
    P1OUT = BIT6;
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

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

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



    lcd_init(); // calling lcd function
    cursor_off;


   while(1){
   }


    /*
    send_line(1,0);
    send_string("Count ");
               send_int(x);
               send_string(" back");

    for(x;x>=0;x--)
    {
    send_line(2,7);
    send_int(x);
    delay(10);
    send_line(2,7);
    send_string("  ");
    delay(1);   //delay for refreshing lcd
    }
*/
}

void displaymsg(){
      send_line(1,2); //first row and 3 space - 16bits

      send_string(lcd_first_line);

      send_line(2,2);  // second row and 6 space - 16 bits
      send_string(lcd_second_line);

      delay_us(2);



  }

void dataArrived(){
    lcd_first_line[0] = 'l';
    lcd_first_line[1] = 'i';
    lcd_first_line[2] = 'g';
    lcd_first_line[3] = 'h';
    lcd_first_line[4] = 't';
    lcd_first_line[5] = 'l';
    lcd_first_line[6] = 'e';
    lcd_first_line[7] = 'v';
    lcd_first_line[8] = 'e';
    lcd_first_line[9] = 'l';
    lcd_first_line[10] = ':';


    if(uart_rx[2]==1){
        lcd_first_line[11] = 'b';
        lcd_first_line[12] = 'r';
        lcd_first_line[13] = 'i';
        lcd_first_line[14] = ' g';
        lcd_first_line[15] = 't';


    }

    else{
        lcd_first_line[11] = 'd';
        lcd_first_line[12] = 'a';
        lcd_first_line[13] = 'r';
        lcd_first_line[14] = 'k';
        lcd_first_line[15] = 't';
    }



    lcd_second_line[0] = 'p';
    lcd_second_line[1] = 'r';
    lcd_second_line[2] = 'o';
    lcd_second_line[3] = 'x';
    lcd_second_line[4] = 'i';
    lcd_second_line[5] = 'm';
    lcd_second_line[6] = 'i';
    lcd_second_line[7] = 't';
    lcd_second_line[8] = 'y';
    lcd_second_line[9] = ':';


    lcd_second_line[12] = (uart_rx[1] >> 16) & 0xFF;
    lcd_second_line[12] = (uart_rx[1]>>8) & 0xFF;
    lcd_second_line[12] = uart_rx[1] & 0xFF;



    for(i=0;i<11;i++){
        lcd_first_line[i]=uart_rx[i];
    }
    for(j=0;j<10;j++){
        lcd_second_line[j]=uart_rx[i];
        i++;
    }
    displaymsg();
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
/*
#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR (void){
    //RX BUFFER INTERRUPT FOR INCOMING DATA FROM THE UART
    counter++;
    if(UCA0RXBUF=='~'){

         dataArrived();
         clear_buffer(uart_rx);
     }
    uart_rx[pos] = UCA0RXBUF;
    pos++;

*/


}
