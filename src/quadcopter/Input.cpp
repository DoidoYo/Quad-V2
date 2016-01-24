/*
 * Input.cpp
 *
 *  Created on: Jan 21, 2016
 *      Author: Gabriel Fernandes
 */

#include "Input.h"

uint64_t input[6];
uint64_t pulseStart[6];

int inputConnected = DISCONNECTED;

void initReceiver() {

	RCC_APB2PeriphClockCmd(
	RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

	GPIO_InitTypeDef init;
	EXTI_InitTypeDef exti;
	NVIC_InitTypeDef nvic;

	//D24 - (PA11)
	init.GPIO_Pin = GPIO_Pin_11;
	init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &init);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource11);

	exti.EXTI_Line = EXTI_Line11;
	exti.EXTI_Mode = EXTI_Mode_Interrupt;
	exti.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	exti.EXTI_LineCmd = ENABLE;
	EXTI_Init(&exti);

	nvic.NVIC_IRQChannel = EXTI15_10_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0x0F;
	nvic.NVIC_IRQChannelSubPriority = 0x0F;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);

	//D27 - (PA8)///////////////////////////////
	init.GPIO_Pin = GPIO_Pin_8;
	init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &init);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource8);

	exti.EXTI_Line = EXTI_Line8;
	exti.EXTI_Mode = EXTI_Mode_Interrupt;
	exti.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	exti.EXTI_LineCmd = ENABLE;
	EXTI_Init(&exti);

	nvic.NVIC_IRQChannel = EXTI9_5_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0x0F;
	nvic.NVIC_IRQChannelSubPriority = 0x0F;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);

	//D28 - (PB15)/////////////////////////////////
	init.GPIO_Pin = GPIO_Pin_15;
	init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &init);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource15);

	exti.EXTI_Line = EXTI_Line15;
	exti.EXTI_Mode = EXTI_Mode_Interrupt;
	exti.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	exti.EXTI_LineCmd = ENABLE;
	EXTI_Init(&exti);

	nvic.NVIC_IRQChannel = EXTI15_10_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0x0F;
	nvic.NVIC_IRQChannelSubPriority = 0x0F;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);

	//D29 - (PB14)////////////////////////////////////
	init.GPIO_Pin = GPIO_Pin_14;
	init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &init);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);

	exti.EXTI_Line = EXTI_Line14;
	exti.EXTI_Mode = EXTI_Mode_Interrupt;
	exti.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	exti.EXTI_LineCmd = ENABLE;
	EXTI_Init(&exti);

	nvic.NVIC_IRQChannel = EXTI15_10_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0x0F;
	nvic.NVIC_IRQChannelSubPriority = 0x0F;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);

	//D30 - (PB13)/////////////////////////////////////
	init.GPIO_Pin = GPIO_Pin_13;
	init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &init);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource13);

	exti.EXTI_Line = EXTI_Line13;
	exti.EXTI_Mode = EXTI_Mode_Interrupt;
	exti.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	exti.EXTI_LineCmd = ENABLE;
	EXTI_Init(&exti);

	nvic.NVIC_IRQChannel = EXTI15_10_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0x0F;
	nvic.NVIC_IRQChannelSubPriority = 0x0F;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);

	//D31 - (PB12)///////////////////////////////////////
	init.GPIO_Pin = GPIO_Pin_12;
	init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &init);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource12);

	exti.EXTI_Line = EXTI_Line12;
	exti.EXTI_Mode = EXTI_Mode_Interrupt;
	exti.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	exti.EXTI_LineCmd = ENABLE;
	EXTI_Init(&exti);

	nvic.NVIC_IRQChannel = EXTI15_10_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0x0F;
	nvic.NVIC_IRQChannelSubPriority = 0x0F;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);

}

void checkReceiverConnection() {
	inputConnected = CONNECTED;
	for (int i = 0; i < 6; i++) {
		if ((micros() - pulseStart[i]) > 21000) {
			input[i] = 0;
			inputConnected = DISCONNECTED;
		}
	}
}

extern "C" {

void EXTI9_5_IRQHandler() {

	if (EXTI_GetITStatus(EXTI_Line8) == SET) {

		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8)) {
			pulseStart[RC_AUX1] = micros();
		} else {
			input[RC_AUX1] = micros() - pulseStart[RC_AUX1];
		}

		EXTI_ClearITPendingBit(EXTI_Line8);
	}

}

void EXTI15_10_IRQHandler() {

	if (EXTI_GetITStatus(EXTI_Line11) == SET) {

		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11)) {
			pulseStart[RC_AUX2] = micros();
		} else {
			input[RC_AUX2] = micros() - pulseStart[RC_AUX2];
			//printf2("Aux2 %i\n", input[RC_AUX2]);
		}

		EXTI_ClearITPendingBit(EXTI_Line11);
	}

	if (EXTI_GetITStatus(EXTI_Line12) == SET) {

		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)) {
			pulseStart[RC_YAW] = micros();
		} else {
			input[RC_YAW] = micros() - pulseStart[RC_YAW];
			//printf2("Yaw %i\n", input[RC_YAW]);
		}

		EXTI_ClearITPendingBit(EXTI_Line12);
	}

	if (EXTI_GetITStatus(EXTI_Line13) == SET) {

		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13)) {
			pulseStart[RC_THROTTLE] = micros();
		} else {
			input[RC_THROTTLE] = micros() - pulseStart[RC_THROTTLE];
			//printf2("Throttle %i\n", input[RC_THROTTLE]);
		}

		EXTI_ClearITPendingBit(EXTI_Line13);
	}

	if (EXTI_GetITStatus(EXTI_Line14) == SET) {

		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)) {
			pulseStart[RC_PITCH] = micros();
		} else {
			input[RC_PITCH] = micros() - pulseStart[RC_PITCH];
			//printf2("Pitch %i\n", input[RC_PITCH]);
		}

		EXTI_ClearITPendingBit(EXTI_Line14);
	}

	if (EXTI_GetITStatus(EXTI_Line15) == SET) {

		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15)) {
			pulseStart[RC_ROLL] = micros();
		} else {
			input[RC_ROLL] = micros() - pulseStart[RC_ROLL];
			//printf2("Roll %i\n", input[RC_ROLL]);
		}

		EXTI_ClearITPendingBit(EXTI_Line15);
	}

}
}

