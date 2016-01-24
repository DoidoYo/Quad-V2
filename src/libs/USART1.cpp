/*
 * USART1.cpp
 *
 *  Created on: Jan 24, 2016
 *      Author: Gabriel Fernandes
 */


#include "USART1.h"

void initUSART1(int baudRate) {
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStructure;
//enable bus clocks
	RCC_APB2PeriphClockCmd(
			RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,
			ENABLE);
//Set USART1 Tx (PA.09) as AF push-pull
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
//Set USART1 Rx (PA.10) as input floating
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	USART_ClockStructInit(&USART_ClockInitStructure);
	USART_ClockInit(USART1, &USART_ClockInitStructure);
	USART_InitStructure.USART_BaudRate = baudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_HardwareFlowControl =
			USART_HardwareFlowControl_None;
//Write USART1 parameters
	USART_Init(USART1, &USART_InitStructure);
//Enable USART1
	USART_Cmd(USART1, ENABLE);

}

void printf2(const char *format, ...) {
	va_list list;
	va_start(list, format);
	int len = vsnprintf(0, 0, format, list);
	char *s;
	s = (char *) malloc(len + 1);
	vsprintf(s, format, list);
	USART_puts(USART1, s);
	free(s);
	va_end(list);
	return;
}
void USART_putc(USART_TypeDef* USARTx, char c) {
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
		;
	USART_SendData(USART1, c);
}

void USART_puts(USART_TypeDef* USARTx, const char *s) {
	int i;
	for (i = 0; s[i] != 0; i++)
		USART_putc(USARTx, s[i]);
}

