/*
 * BMP180.cpp
 *
 *  Created on: Jan 17, 2016
 *      Author: Gabriel Fernandes
 */

#include "BMP180.h"

BMP180::BMP180() {}

void BMP180::init(I2C *i2c, short _mode) {
	I2Cx = i2c;
		mode = _mode;
	step = SEND_TEMP;

	uint8_t data[2];
	I2Cx->readBytes(BMP180_ADDR, 0xAA, 2, data);
	AC1 =  (data[0] << 8) | data[1];

	I2Cx->readBytes(BMP180_ADDR, 0xAC, 2, data);
	AC2 =  (data[0] << 8) | data[1];

	I2Cx->readBytes(BMP180_ADDR, 0xAE, 2, data);
	AC3 =  (data[0] << 8) | data[1];

	I2Cx->readBytes(BMP180_ADDR, 0xB0, 2, data);
	AC4 = (data[0] << 8) | data[1];

	I2Cx->readBytes(BMP180_ADDR, 0xB2, 2, data);
	AC5 = (data[0] << 8) | data[1];

	I2Cx->readBytes(BMP180_ADDR, 0xB4, 2, data);
	AC6 =  (data[0] << 8) | data[1];

	I2Cx->readBytes(BMP180_ADDR, 0xB6, 2, data);
	B1 =  (data[0] << 8) | data[1];

	I2Cx->readBytes(BMP180_ADDR, 0xB8, 2, data);
	B2 =  (data[0] << 8) | data[1];

	I2Cx->readBytes(BMP180_ADDR, 0xBA, 2, data);
	MB =  (data[0] << 8) | data[1];

	I2Cx->readBytes(BMP180_ADDR, 0xBC, 2, data);
	MC =  (data[0] << 8) | data[1];

	I2Cx->readBytes(BMP180_ADDR, 0xBE, 2, data);
	MD =  (data[0] << 8) | data[1];

	SEND_TEMP =0;
	READ_TEMP=1;
	SEND_PRESS=2;
	READ_PRESS=3;
}

int BMP180::getDelay() {
	return delay;
}

void BMP180::getPressure(long &P) {

	//printf2("In\n");

	if (step == SEND_TEMP) {
		//printf2("SEND_TEMP\n");
		I2Cx->sendByte(BMP180_ADDR, BMP180_REG_CONTROL,
				BMP180_COMMAND_TEMPERATURE);

		timer = millis();
		step = READ_TEMP;
	} else if (step == READ_TEMP) {
		//printf2("almost READ_TEMP %i\n",(millis()));
		if ((millis() - timer) > (uint32_t)5) {
			//printf2("READ_TEMP\n");
			uint8_t data[2];
			I2Cx->readBytes(BMP180_ADDR, BMP180_REG_RESULT, 2, data);

			UT = (data[0] << 8) + data[1];

			X1 = (UT - AC6) * AC5 / pow(2, 15);
			X2 = MC * pow(2, 11) / (X1 + MD);
			B5 = X1 + X2;
			T = (B5 + 8) / pow(2, 4);

			step = SEND_PRESS;
		}
	} else if (step == SEND_PRESS) {
		//printf2("SEND_PRESS\n");
		switch (mode) {
		case 0:
			I2Cx->sendByte(BMP180_ADDR, BMP180_REG_CONTROL,
			BMP180_COMMAND_PRESSURE0);
			delay = 5;
			break;
		case 1:
			I2Cx->sendByte(BMP180_ADDR, BMP180_REG_CONTROL,
			BMP180_COMMAND_PRESSURE1);
			delay = 8;
			break;
		case 2:
			I2Cx->sendByte(BMP180_ADDR, BMP180_REG_CONTROL,
			BMP180_COMMAND_PRESSURE2);
			delay = 14;
			break;
		case 3:
			I2Cx->sendByte(BMP180_ADDR, BMP180_REG_CONTROL,
			BMP180_COMMAND_PRESSURE3);
			delay = 26;
			break;
		default:
			I2Cx->sendByte(BMP180_ADDR, BMP180_REG_CONTROL,
			BMP180_COMMAND_PRESSURE0);
			delay = 5;
			break;
		}
		timer = millis();
		step = READ_PRESS;
	} else if (step == READ_PRESS) {
		if((millis() - timer) > (uint32_t)delay) {
			//printf2("READ_PRESS\n");

			uint8_t data[3];
			I2Cx->readBytes(BMP180_ADDR, BMP180_REG_RESULT, 3,data);

			UP = ((data[0]<<16) + (data[1]<<8) + data[2]) >> (8-mode);

			B6 = B5 - 4000;
			X1 = (B2 * (B6 * B6 / pow(2,12)))/pow(2,11);
			X2 = AC2 * B6 / pow(2,11);
			X3 = X1 + X2;
			B3 = (((AC1 * 4 + X3) << mode) + 2) / 4;
			X1 = AC3 * B6 / pow(2,13);
			X2 = (B1 * (B6 * B6 / pow(2,12))) / pow(2,16);
			X3 = ((X1 + X2) + 2) / pow(2,2);
			B4 = AC4 * (unsigned long)(X3 + 32768) / pow(2,15);
			B7 = ((unsigned long)UP - B3) * (50000 >> mode);
			if(B7 < 0x80000000) {
				p = (B7 * 2) / B4;
			} else {
				p = (B7/B4)*2;
			}
			X1 = (p/pow(2,8)) * (p/pow(2,8));
			X1 = (X1 * 3038) / pow(2,16);
			X2 = (-7357 * p) / pow(2,16);
			p = p + (X1 + X2 + 3791) / pow(2,4);

			P = p;

			//printf2("Yee\n");

			step = SEND_TEMP;
		}
	}

}

void BMP180::debug() {
	//printf2(
	//		"%i \t %i \t %i \t %i \t %i \t %i \t %i \t %i \t %i \t %i \t %i \t \n",
	//		AC1, AC2, AC3, AC4, AC5, AC6, B1, B2, MB, MC, MD);
}
