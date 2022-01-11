

#ifndef __I2C_H
#define __I2C_H
#include<reg52.h> 
#include "delay.h"
sbit SCL=P3^7;
sbit SDA=P3^6;




void I2CStart()
{
	SCL = 0;
	SDA = 1;
	Delay_us(50);
	SCL = 1;
	Delay_us(50);
	SDA = 0;
	Delay_us(50);
	SCL = 0;
}
 
void I2CStop()
{
	SCL = 0;
	Delay_us(50);
	SDA = 0;
	Delay_us(50);
	SCL = 1;
	Delay_us(50);
	SDA = 1;
}
 void I2CAck()
{
	
	SDA = 0;
	Delay_us(50);
	SCL = 1;
	Delay_us(50);
	SCL = 0;
	SDA = 1;
}



void I2CNak()
{
	SDA = 1;
	Delay_us(50);
	SCL = 1;
	Delay_us(50);
	SCL = 0;
	SDA = 1;
}

void I2CSend(unsigned char Data)
{
	 unsigned char i;
	 for (i = 0; i < 8; i++) 
	{
		SDA=Data & 0x80;
		SCL = 1;
	 	SCL = 0;
		Data<<=1;
	}
	 SDA = 1;
	 Delay_us(10);
	 SCL = 1;
	 Delay_us(10);
	 SCL = 0;
}
 
unsigned char I2CRead()
{
	unsigned char i, Data=0;
	for (i = 0; i < 8; i++) 
	{
		SCL = 1;
		Data|=SDA;
		if(i<7)
			Data<<=1;
		SCL = 0;
	}
	return Data;
}

void I2C_send_byte(unsigned char adr, unsigned char Dat)
{
	I2CStart();
	I2CSend(0xD0);
	I2CSend(adr);
	I2CSend(Dat);
	I2CStop();
}

#endif