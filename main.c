#include<reg52.h>
#include "I2C.h"
#include "delay.h"
#include "DS1307.h"
sbit D1 = P2^2;
sbit D2 = P2^3;
sbit D3 = P2^4;
sbit D4 = P2^5;
sbit D5 = P2^6;
sbit D6 = P2^7;
sbit set = P1^2;
sbit next = P1^3;
sbit pause = P1^4;
const unsigned char st[]={0x01,0xC7,0x22,0x82,0xC4,0x88,0x08,0xC3,0x00,0xC0};
const unsigned char rt[]={0x80,0xB6,0x44,0x14,0x32,0x11,0x01,0xB4,0x00,0x30};
const unsigned char Pm = 0x60;
const unsigned char Am = 0x20;
const unsigned char Mm = 0x4A;


void disp_sec()
{
	unsigned int s_1,s_2;
	s_1 = 0x0f&second;
	s_2 = 0x0f&(second>>4);
	P0 = st[s_1]; D1 = 0; Delay_ms(2); D1 = 1; P0 = rt[s_2]; D2 = 0; Delay_ms(2); D2 = 1;
}

void disp_minute()
{
	unsigned int s_1,s_2;
	s_1 = 0x0f&minute;
	s_2 = 0x0f&(minute>>4);
	P0 = st[s_1]; D3 = 0; Delay_ms(2); D3 = 1; P0 = rt[s_2]; D4 = 0; Delay_ms(2); D4 = 1;
}

void disp_hour()
{
	unsigned int s_1,s_2;
	s_1 = 0x0f&hour;
	s_2 = 0x0f&(hour>>4);
	P0 = st[s_1]; D5 = 0; Delay_ms(2); D5 = 1; P0 = rt[s_2]; D6 = 0; Delay_ms(2); D6 = 1;
}

void set_time()
{
	I2CStart();
	I2CSend(0xD0);
	I2CSend(0x00);
	I2CSend(0x01);
	I2CSend(0x01);
	I2CSend(0x16);
	I2CSend(0x01);
	I2CSend(0x01);
	I2CSend(0x01);
	I2CSend(0x01);
	I2CStop();
}
int BCDToDecimal(char bcdByte)
{
       char a,b,dec;
    a=(((bcdByte & 0xF0) >> 4) * 10);
    b=(bcdByte & 0x0F);
    dec=a+b;
    return dec;
}

char DecimalToBCD (int decimalByte)
{
  char a,b,bcd;
  a=((decimalByte / 10) << 4);
  b= (decimalByte % 10);
  bcd=a|b;
  return bcd;
}

unsigned int modifyBit( unsigned int n, unsigned int p, unsigned int b)
{
	unsigned int mask = 1<<p;
	return ((n & (~mask)) | (b<<p));
}

void pause_ini()
{
	unsigned int g=1,i=0;
	while(pause == 0)
	{
		if(g==1)
		{
			for(i=0;i<12;i++)
			{
				disp_sec();
				disp_minute();
			}
			disp_hour();
			if(set == 0)
			{
				unsigned int a,q,w;
				a = BCDToDecimal(hour&0x1f);
				a = a+1;
				if(a>12){a=1;}
				q = DecimalToBCD(a);
				a = (hour>>5)&0x01;
				w = modifyBit(q,5,a);
				a = modifyBit(w,6,1);
				hour = a;
				I2C_send_byte(0x02,a);
				Delay_ms(15);
			}
			if(next == 0)
			{
				g=2;
				Delay_ms(15);
			}
		}
		if(g==2)
		{
			for(i=0;i<12;i++)
			{
				disp_sec();
				disp_hour();
			}
			disp_minute();
			if(set == 0)
			{
				unsigned int a;
				a = BCDToDecimal(minute);
				a = a+1;
				if(a>59){a=0;}
				a = DecimalToBCD(a);
				minute = a;
				I2C_send_byte(0x01,a);
				Delay_ms(15);
			}
			if(next == 0)
			{
				g=3;
				Delay_ms(15);
			}
		}
		if(g==3)
		{
			for(i=0;i<12;i++)
			{
				disp_hour();
				disp_minute();
			}
			disp_sec();
			if(set == 0)
			{
				unsigned int a,q,w;
				a = (hour>>5)&0x01;
				if(a == 0x01){a = 0x00;}
				else if(a == 0x00) {a = 0x01;}
				q = hour;
				w = modifyBit(q, 5, a);
				hour = w;
				I2C_send_byte(0x02,w);
				Delay_ms(15);
			}
			if(next == 0)
			{
				Delay_ms(15);
				g=1;
			}
		}
	}
}

void main()
{
	set_time();
	P0 = 0xff;
	pause = 1;
	next = 1;
	set = 1;
	while(1)
	{
		get_time();
		disp_hour();
		disp_minute();
		disp_sec();
	}
}