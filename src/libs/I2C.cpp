/*
 * I2C.cpp
 *
 *  Created on: Jan 17, 2016
 *      Author: Gabriel Fernandes
 */

#include "I2C.h"

I2C::I2C() {}

void I2C::init(I2C_TypeDef *i2c, unsigned int speed) {
	I2Cx = i2c;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

		GPIO_InitTypeDef gpioInit;
		I2C_InitTypeDef i2cInit;

		if (I2Cx == I2C1) {
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

			gpioInit.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
			gpioInit.GPIO_Mode = GPIO_Mode_AF_OD;
			gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOB, &gpioInit);
		} else if (I2Cx == I2C2) {
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);

			gpioInit.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
			gpioInit.GPIO_Mode = GPIO_Mode_AF_OD;
			gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOB, &gpioInit);
		}

		//configure I2C
		i2cInit.I2C_Mode = I2C_Mode_I2C;
		i2cInit.I2C_DutyCycle = I2C_DutyCycle_16_9;
		i2cInit.I2C_OwnAddress1 = 0x00;
		i2cInit.I2C_Ack = I2C_Ack_Enable;
		i2cInit.I2C_ClockSpeed = speed;
		I2C_Init(I2Cx, &i2cInit);
		//enable I2C
		I2C_Cmd(I2Cx, ENABLE);
}

void I2C::sendByte(uint8_t address, uint8_t reg, uint8_t data) {
	I2C_AcknowledgeConfig(I2Cx, ENABLE);

	while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
		;

	I2C_GenerateSTART(I2Cx, ENABLE);
	while (!I2C_GetFlagStatus(I2Cx, I2C_FLAG_SB))
		;

	I2C_Send7bitAddress(I2Cx, address << 1, I2C_Direction_Transmitter);
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
		;

	I2C_SendData(I2Cx, reg);
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		;

	I2C_SendData(I2Cx, data);
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		;

	//I2C_AcknowledgeConfig(I2Cx, DISABLE);
	//I2C_NACKPositionConfig(I2Cx, I2C_NACKPosition_Current);

	I2C_GenerateSTOP(I2Cx, ENABLE);
	while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF))
		;
}

void I2C::readByte(uint8_t addr, uint8_t reg, uint8_t &data) {
	I2C_AcknowledgeConfig(I2Cx, ENABLE);

	while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
		;

	I2C_GenerateSTART(I2Cx, ENABLE);
	while (!I2C_GetFlagStatus(I2C1, I2C_FLAG_SB))
		;

	I2C_Send7bitAddress(I2Cx, addr << 1, I2C_Direction_Transmitter);
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
		;

	I2C_SendData(I2Cx, reg);
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		;

	I2C_GenerateSTART(I2Cx, ENABLE);
	while (!I2C_GetFlagStatus(I2Cx, I2C_FLAG_SB))
		;

	I2C_Send7bitAddress(I2Cx, addr << 1, I2C_Direction_Receiver);
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
		;

	I2C_AcknowledgeConfig(I2Cx, DISABLE);
	I2C_NACKPositionConfig(I2Cx, I2C_NACKPosition_Current);

	//wait for data to be received
	while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED))
		;
	//read data
	data = I2C_ReceiveData(I2Cx);

	I2C_GenerateSTOP(I2Cx, ENABLE);
	while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF))
		;
}

void I2C::readBytes(uint8_t address, uint8_t sRegister, int bytes,
		uint8_t* data) {
	I2C_AcknowledgeConfig(I2C1, ENABLE);

	while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
		;

	I2C_GenerateSTART(I2C1, ENABLE);
	while (!I2C_GetFlagStatus(I2C1, I2C_FLAG_SB))
		;

	I2C_Send7bitAddress(I2C1, address << 1, I2C_Direction_Transmitter);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
		;

	I2C_SendData(I2C1, sRegister);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		;

	I2C_GenerateSTART(I2C1, ENABLE);
	while (!I2C_GetFlagStatus(I2C1, I2C_FLAG_SB))
		;

	I2C_Send7bitAddress(I2C1, address << 1, I2C_Direction_Receiver);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
		;

	int i;
	for (i = 0; i < bytes; i++) {
		//wait for data to be received
		while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))
			;
		//read data
		data[i] = I2C_ReceiveData(I2C1);

		if (i == (bytes - 2)) {
			I2C_AcknowledgeConfig(I2C1, DISABLE);
			I2C_NACKPositionConfig(I2C1, I2C_NACKPosition_Current);
		}

	}

	I2C_GenerateSTOP(I2C1, ENABLE);
	while (I2C_GetFlagStatus(I2C1, I2C_FLAG_STOPF))
		;
}
