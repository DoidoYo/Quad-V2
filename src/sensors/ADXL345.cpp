/*
 * ADXL345.cpp
 *
 *  Created on: Jan 24, 2016
 *      Author: Gabriel Fernandes
 */

#include "ADXL345.h"

ADXL345::ADXL345()
{
}

void ADXL345::init(I2C* I2Cx)
{
	i2c = I2Cx;

	i2c->sendByte(ADXL345_ADDRESS, ADXL345_REG_POWER_CTL, 0x08);

	//set range to 8+-
	uint8_t data;
	i2c->readByte(ADXL345_ADDRESS, ADXL345_REG_DATA_FORMAT, data);
	data &= ~0x0F;
	data |= ADXL345_RANGE_8_G;
	data |= 0x08;
	i2c->sendByte(ADXL345_ADDRESS, ADXL345_REG_DATA_FORMAT, data);

	//set datarate
	i2c->sendByte(ADXL345_ADDRESS, ADXL345_REG_BW_RATE, ADXL345_DATARATE_200_HZ);

	calibrated = 0;

	delayMillis(250);
}

void ADXL345::read(vector &v)
{
	uint8_t data[6];
	i2c->readBytes(ADXL345_ADDRESS, ADXL345_REG_DATAX0, 6, data);

	v.x = (float) ((int16_t) (data[0] | (data[1] << 8)));
	v.y = (float) ((int16_t) (data[2] | (data[3] << 8)));
	v.z = (float) ((int16_t) (data[4] | (data[5] << 8)));

	v.x /= 256;
	v.y /= 256;
	v.z /= 256;

	if (calibrated)
	{
		v.x -= zeroX;
		v.y -= zeroY;
		v.z -= zeroZ;
	}

}

void ADXL345::calibrate(GPIO light)
{

	zeroX = 0;
	zeroY = 0;
	zeroZ = 0;

	vector v;
	for (int i = 0; i < 1000; i++)
	{
		if (i % 15 == 0)
			light.toggle();

		read(v);
		zeroX += v.x;
		zeroY += v.y;
		zeroZ += (1-v.z);

		delayMillis(5);
	}

	light.low();

	zeroX /= 1000;
	zeroY /= 1000;
	zeroZ /= 1000;

	calibrated = 1;

}

