/*
 * I2C.h
 *
 *  Created on: Jan 17, 2016
 *      Author: Gabriel Fernandes
 */

#ifndef I2C_H_
#define I2C_H_

#include "stm32f10x.h"

class I2C {
public:
	I2C();
	void init(I2C_TypeDef *i2c, unsigned int speed);
	void sendByte(uint8_t address, uint8_t reg, uint8_t data);
	void readBytes(uint8_t address, uint8_t reg, int bytes, uint8_t* data);
	void readByte(uint8_t addr, uint8_t reg, uint8_t &data);

private:
	I2C_TypeDef *I2Cx;
};

#endif /* I2C_H_ */
