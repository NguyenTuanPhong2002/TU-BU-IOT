/*
 * rs485.h
 *
 *  Created on: Sep 11, 2023
 *      Author: ACER
 */

#ifndef RS485_RS485_H_
#define RS485_RS485_H_

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
void RS485_Transmit(RS485 *me);
void RS485_Receive(RS485 *me);

#endif /* RS485_RS485_H_ */
