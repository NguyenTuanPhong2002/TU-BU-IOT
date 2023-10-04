/*
 * rs485.cpp
 *
 *  Created on: Sep 11, 2023
 *      Author: Trump2003
 */
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_uart.h"

#ifndef RS485_RS485_H_
#define RS485_RS485_H_
/**
 * @brief 						
 * 
 */
typedef struct
{
	GPIO_TypeDef *RS485_Power_Port;
	uint16_t RS485_Power_Pin;
	GPIO_TypeDef *RS485_DE_Port;
	uint16_t RS485_DE_Pin;
	GPIO_TypeDef *RS485_RE_Port;
	uint16_t RS485_RE_Pin;
	UART_HandleTypeDef *huart;
	char TxBuffer[256];
	char RxBuffer[256];

} RS485;
/**
 * @brief 
 * 
 * @param me 
 */
void RS485_Transmit(RS485 *me);
void RS485_Receive(RS485 *me);
#endif /* RS485_RS485_H_ */
