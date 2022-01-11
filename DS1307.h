#ifndef __DS1307_H
#define __DS1307_H
#include<reg52.h>
#include "I2C.h"

extern unsigned int hour=0, minute=0, second=0;
unsigned int get_hour()
{
	unsigned int a;
	I2CStart();
	I2CSend(0xD0);
	I2CSend(0x02);
	I2CStart();
	I2CSend(0xD1);
	a = I2CRead();
	I2CNak();
	I2CStop();
	return a;
}

unsigned int get_minute()
{
	unsigned int a;
	I2CStart();
	I2CSend(0xD0);
	I2CSend(0x01);
	I2CStart();
	I2CSend(0xD1);
	a = I2CRead();
	I2CNak();
	I2CStop();
	return a;
}

unsigned int get_second()
{
	unsigned int a;
	I2CStart();
	I2CSend(0xD0);
	I2CSend(0x00);
	I2CStart();
	I2CSend(0xD1);
	a = I2CRead();
	I2CNak();
	I2CStop();
	return a;
}

void get_time()
{
	I2CStart();
	I2CSend(0xD0);
	I2CSend(0x00);
	I2CStart();
	I2CSend(0xD1);
	second = I2CRead();
	I2CAck();
	minute = I2CRead();
	I2CAck();
	hour = I2CRead();
	I2CNak();
	I2CStop();
}

#endif