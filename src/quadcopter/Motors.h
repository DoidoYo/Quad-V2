/*
 * Motors.h
 *
 *  Created on: Jan 22, 2016
 *      Author: Gabriel Fernandes
 */

#ifndef MOTORS_H_
#define MOTORS_H_

#include "stm32f10x.h"

#define M_FR 0
#define M_FL 1
#define M_BL 2
#define M_BR 3

extern int motors[4];

void motorsInit();
void motorsSetAll(int i);
void motorsUpdate();


#endif /* MOTORS_H_ */
