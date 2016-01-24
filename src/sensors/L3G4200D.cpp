/*
 * L3G4300D.cpp
 *
 *  Created on: Jan 18, 2016
 *      Author: Gabriel Fernandes
 */

#include "L3G4200D.h"

L3G4200D::L3G4200D() {
}

void L3G4200D::init(I2C *i2c) {
	I2Cx = i2c;

	I2Cx->sendByte(105, 0x20, 0x0F);
	I2Cx->sendByte(105, 0x23, 0x90);

	calibrated = 0;

	delayMillis(250);
}

void L3G4200D::calibrate() {

	vector v;

	zeroX = 0;
	zeroY = 0;
	zeroZ = 0;

	for (int i = 0; i < 1000; i++) {
		read(v);
		zeroX += (float) v.x;
		zeroY += (float) v.y;
		zeroZ += (float) v.z;

		delayMillis(5);
	}

	zeroX /= 1000;
	zeroY /= 1000;
	zeroZ /= 1000;

	calibrated = 1;

	//printf2("Zeroes: %i \t %i \t %i \n", (int)zeroX,(int)zeroY,(int)zeroZ);

}

void L3G4200D::read(vector &out) {
	uint8_t data[6];

	I2Cx->readBytes(105, 168, 6, data);

	out.x = (float) (((int16_t) ((data[1] << 8) | data[0])));
	out.y = (float) (((int16_t) ((data[3] << 8) | data[2])));
	out.z = (float) (((int16_t) ((data[5] << 8) | data[4])));

	out.x /= 57.14286;
	out.y /= 57.14286;
	out.z /= 57.14286;

	if (calibrated) {
		out.x -= zeroX;;
		out.y -= zeroY;
		out.z -= zeroZ;
	}
}

