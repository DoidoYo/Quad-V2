/*
 * GPIO.cpp
 *
 *  Created on: Jan 19, 2016
 *      Author: Gabriel Fernandes
 */


#include "GPIO.h"

GPIO::GPIO() {}

void GPIO::init(GPIO_TypeDef* gpio, uint16_t pin, GPIOMode_TypeDef mode) {
	_gpio = gpio;
		_pin = pin;
		_mode = mode;

		if(_gpio == GPIOA) {
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		} else if (_gpio == GPIOB) {
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		} else if (_gpio == GPIOC) {
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
		} else if (_gpio == GPIOD) {
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
		} else if (_gpio == GPIOE) {
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
		} else if (_gpio == GPIOF) {
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
		}

		GPIO_InitTypeDef init;
		init.GPIO_Mode = _mode;
		init.GPIO_Pin = _pin;
		init.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(_gpio, &init);
}

void GPIO::high() {
	GPIO_SetBits(_gpio, _pin);
}

void GPIO::low() {
	GPIO_ResetBits(_gpio, _pin);
}

int GPIO::read() {
	return GPIO_ReadInputDataBit(_gpio, _pin);
}
