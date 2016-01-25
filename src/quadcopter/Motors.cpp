/*
 * Motors.cpp
 *
 *  Created on: Jan 22, 2016
 *      Author: Gabriel Fernandes
 */

#include "Motors.h"

/*
 * motor 1 FR = 3 - (PB0) TIM3_CH3
 * motor 2 FL = 4 - (PA7) TIM3_CH2
 * motor 3 BL = 5 - (PA6) TIM3_CH1
 * motor 4 BR = 10 - (PA1) TIME2_CH2
 */

int motors[4];

void motorsInit() {

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef init;
	TIM_TimeBaseInitTypeDef time;

	//motor 1 /////////////////////////////////////////
	init.GPIO_Pin = GPIO_Pin_0;
	init.GPIO_Mode = GPIO_Mode_AF_PP;
	init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &init);

	//motor 2 /////////////////////////////////////////
	init.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOA, &init);

	//motor 3 /////////////////////////////////////////
	init.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOA, &init);

	//motor 4 /////////////////////////////////////////
	init.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOA, &init);

	/////////////////////
	TIM_TimeBaseStructInit(&time);
	time.TIM_Period = 4000 - 1;
	time.TIM_Prescaler = (uint16_t) (SystemCoreClock / 1000000) - 1;
	time.TIM_ClockDivision = 0;
	time.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &time);
	TIM_TimeBaseInit(TIM3, &time);

	TIM_CtrlPWMOutputs(TIM2, ENABLE);
	TIM_CtrlPWMOutputs(TIM3, ENABLE);

	TIM_Cmd(TIM2, ENABLE);
	TIM_Cmd(TIM3, ENABLE);

	TIM_OCInitTypeDef pwm;
	TIM_OCStructInit(&pwm);
	pwm.TIM_OCMode = TIM_OCMode_PWM1;
	pwm.TIM_OutputState = TIM_OutputState_Enable;
	pwm.TIM_OCPolarity = TIM_OCPolarity_High;
	pwm.TIM_Pulse = 1000;

	TIM_OC3Init(TIM3, &pwm);
	TIM_OC2Init(TIM3, &pwm);
	TIM_OC1Init(TIM3, &pwm);
	TIM_OC2Init(TIM2, &pwm);

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);

}

void motorsSetAll(int i) {
	TIM3->CCR3 = i;

	TIM3->CCR2 = i;

	TIM3->CCR1 = i;

	TIM2->CCR2 = i;
}

void motorsUpdate() {

	TIM3->CCR3 = motors[M_BR];

	TIM3->CCR2 = motors[M_BL];

	TIM3->CCR1 = motors[M_FL];

	TIM2->CCR2 = motors[M_FR];

}

