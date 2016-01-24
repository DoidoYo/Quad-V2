/*
 * Input.h
 *
 *  Created on: Jan 19, 2016
 *      Author: Gabriel Fernandes
 */

#ifndef INPUT_H_
#define INPUT_H_

#include "stm32f10x.h"
#include "libs/Timer.h"

#define RC_THROTTLE 1
#define RC_YAW 0
#define RC_ROLL 3
#define RC_PITCH 2
#define RC_AUX1 4
#define RC_AUX2 5

#define DISCONNECTED 0
#define CONNECTED 1

void inputInit();
void inputCheck();

extern uint64_t input[6];
extern int inputConnected;

extern "C" {
void EXTI9_5_IRQHandler();
void EXTI15_10_IRQHandler();
}

#endif /* INPUT_H_ */
