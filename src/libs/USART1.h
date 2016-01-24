/*
 * USART1.h
 *
 *  Created on: Jan 18, 2016
 *      Author: Gabriel Fernandes
 */

#ifndef USART1_H_
#define USART1_H_


#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "stm32f10x.h"



 void initUSART1(int baudRate);
 void printf2(const char *format, ...);
 void USART_putc(USART_TypeDef* USARTx,char c);
 void USART_puts(USART_TypeDef* USARTx,const char *s);



#endif /* USART1_H_ */
