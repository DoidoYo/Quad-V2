/*
 * Timer.cpp
 *
 *  Created on: Jan 23, 2016
 *      Author: Gabriel Fernandes
 */

#include "Timer.h"

uint64_t _microCounter;

void initTimer() {
	SysTick_Config(SystemCoreClock / 1000000);
}

uint64_t millis() {
	return _microCounter / 1000;
}

uint64_t micros() {
	return _microCounter;
}

void delayMillis(int DelayTime) {
	uint64_t delayTimer = millis();
	while ((millis() - delayTimer) < (uint32_t) DelayTime)
		;
}

void delayMicros(int DelayTime) {
	uint64_t delayTimer = micros();
	while ((micros() - delayTimer) < (uint32_t) DelayTime)
		;
}

void incrementMicros() {
	_microCounter++;
}

extern "C" {

void SysTick_Handler() {
	incrementMicros();
}
}
