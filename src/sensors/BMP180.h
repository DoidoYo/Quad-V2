/*
 * BMP180.h
 *
 *  Created on: Jan 17, 2016
 *      Author: Gabriel Fernandes
 */

#include "stm32f10x.h"
#include "libs/I2C.h"
#include "libs/Timer.h"
#include "math.h"

#define BMP180_ADDR 0x77 // 7-bit address

#define	BMP180_REG_CONTROL 0xF4
#define	BMP180_REG_RESULT 0xF6
#define BMP180_REG_ID 0xD0

#define	BMP180_COMMAND_TEMPERATURE 0x2E
#define	BMP180_COMMAND_PRESSURE0 0x34
#define	BMP180_COMMAND_PRESSURE1 0x74
#define	BMP180_COMMAND_PRESSURE2 0xB4
#define	BMP180_COMMAND_PRESSURE3 0xF4

class BMP180 {

public:
	BMP180();
	void init(I2C *i2c, short _mode);
	void startPressure();
	void getPressure(long &P);
	int getDelay();
	void debug();

	//void readPressureTask();
	//long Pressure;


private:
	I2C *I2Cx;

	uint32_t timer;

	short mode, step;

	int16_t AC1,AC2,AC3,B1,B2,MB,MC,MD;
	uint16_t AC4,AC5,AC6;

	long X1, X2, X3,B3,B5, T, B6,p, UT, UP;
	unsigned long B4,B7;

	//double c5,c6,mc,md,x0,x1,x2,y0,y1,y2,p0,p1,p2, T;
	int delay;

	int SEND_TEMP, READ_TEMP, SEND_PRESS, READ_PRESS;

};
