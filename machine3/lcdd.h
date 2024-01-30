#include <msp430g2553.h>
#include <stdio.h>
#define DR P2OUT = P2OUT | BIT4 // define RS high
#define CWR P2OUT = P2OUT & (~BIT4) // define RS low
#define READ P2OUT = P2OUT | BIT5 // define Read signal R/W = 1 for reading
#define WRITE P2OUT = P2OUT & (~BIT5) // define Write signal R/W = 0 for writing
#define ENABLE_HIGH P1OUT = P1OUT | BIT6 // define Enable high signal
#define ENABLE_LOW P1OUT = P1OUT & (~BIT6) // define Enable Low signal

#define lcd_clear send_command(0x01)
#define frist_line send_command(0x80)
#define second_line send_command(0xC0)
#define cursor_off send_command(0x0C)

unsigned int i;
unsigned int j;
char positionString[6]; //to be used in itoa function

void delay(unsigned int k) //watch you freq.
{
for(j=0;j<=k;j++)
{
for(i=0;i<100;i++);
}
}


void delay_us(unsigned int us)
{
    while (us)
    {
        __delay_cycles(1000000); // 1 for 1 Mhz set 16 for 16 MHz
        us--;
    }
}



void data_write(void)
{
ENABLE_HIGH;
delay(1);
ENABLE_LOW;
}
void data_read(void)
{
ENABLE_LOW;
delay(1);
ENABLE_HIGH;
}

void check_busy(void)
{
P2DIR &= ~(BIT3); // make P2.3 as input
while((P2IN&BIT3)==1)
{
data_read();
}
P2DIR |= BIT3; // make P2.3 as output
}

void send_command(unsigned char cmd)
{
check_busy();
WRITE;
CWR;
P2OUT = (P2OUT & 0xF0)|((cmd>>4) & 0x0F); // send higher nibble
data_write(); // give enable trigger
P2OUT = (P2OUT & 0xF0)|(cmd & 0x0F); // send lower nibble
data_write(); // give enable trigger
}

void send_data(unsigned char data)
{
check_busy();
WRITE;
DR;
P2OUT = (P2OUT & 0xF0)|((data>>4) & 0x0F); // send higher nibble
data_write(); // give enable trigger
P2OUT = (P2OUT & 0xF0)|(data & 0x0F); // send lower nibble
data_write(); // give enable trigger
}



void send_string(char *s)
{
while(*s)
{
send_data(*s);
s++;
}
}


char *itoa(unsigned long num, char *str, int radix) {
char sign = 0;
char temp[17]; //an int can only be 16 bits long
//at radix 2 (binary) the string
//is at most 16 + 1 null long.
int temp_loc = 0;
int digit;
int str_loc = 0;
//save sign for radix 10 conversion
if (radix == 10 && num < 0) {
sign = 1;
num = -num;
}
//construct a backward string of the number.
do {
digit = (unsigned long)num % radix;
if (digit < 10)
temp[temp_loc++] = digit + '0';
else
temp[temp_loc++] = digit - 10 + 'A';
num = (((unsigned long)num) / radix);
} while ((unsigned long)num > 0);
//now add the sign for radix 10
if (radix == 10 && sign) {
 temp[temp_loc] = '-';
 } else {
 temp_loc--;
 }
 //now reverse the string.
 while ( temp_loc >=0 ) {// while there are still chars
 str[str_loc++] = temp[temp_loc--];
 }
 str[str_loc] = 0; // add null termination.
 return str;
}


void send_int(long num)
{
itoa(num, positionString, 10);
send_string(positionString);
}



void send_float(double num)// float deðikenleri lcd ekrana yazmak için yeni foksiyon
{

send_int((long)num);// int deðeri yazdýrmak
send_string(".");//lcd . iþaretini koyuyoruz
long a_num=((long)num*100); // virgülü kýsmý 2 basamagý kurtarýp tam sayý yapýyoruz.

num=num*100; // gelen int deðeri 100 ile çapým num_a çýkarýnca kalan kýsým virgulý kýsm olucak onuda lcd ekrana yazýyoruz.
send_int(num-a_num);

}


void send_line(int satir,int sutun) // satýr sutun bilgisi için gerekli yeni foksiyon
{

    if(satir == 1)
    {
      send_command(0x80 + sutun); // Komut tablosuna göre 80-8F arasýnda deðer gönderiliyor.
    }

    else if(satir == 2)
    {
      send_command(0xC0 + sutun);  // C0 - CF arasý deðer gönderiliyor.
    }
    delay(5);
}

void lcd_init(void)
{
P2DIR |= 0xFF;
P2OUT &= 0x00;
P1DIR |= BIT6;

send_command(0x33);
send_command(0x32);
send_command(0x28); // 4 bit mode
send_command(0x0E); // clear the screen
send_command(0x01); // display on cursor on
send_command(0x06); // increment cursor
send_command(0x80); // row 1 column 1
}
