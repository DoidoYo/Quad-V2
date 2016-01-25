/*
 * GPIO.h
 *
 *  Created on: Jan 19, 2016
 *      Author: Gabriel Fernandes
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "stm32f10x.h"

class GPIO {

public:
	GPIO();
	void init(GPIO_TypeDef* gpio, uint16_t pin, GPIOMode_TypeDef mode);
	void high();
	void low();
	void toggle();
	int read();

private:
	GPIO_TypeDef* _gpio;
	uint16_t _pin;
	GPIOMode_TypeDef _mode;
};


#endif /* GPIO_H_ */
