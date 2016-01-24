/*
 * Timer.h
 *
 *  Created on: Jan 23, 2016
 *      Author: Gabriel Fernandes
 */

#ifndef TIMER_H_
#define TIMER_H_


#include "stm32f10x.h"

void initTimer();
void delayMillis(int i);
void delayMicros(int i);
void incrementMicros();

uint64_t millis();
uint64_t micros();

extern "C" {
void SysTick_Handler();
}

#endif /* TIMER_H_ */
