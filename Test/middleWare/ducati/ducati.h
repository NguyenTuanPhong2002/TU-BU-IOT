/*
 * ducati.h
 *
 *  Created on: Oct 4, 2023
 *      Author: Trump2003
 */
#ifndef Ducati_H_
#define Ducati_H_

#include "stm32f4xx_hal.h"
#include "rs485.h"

// Query struct
typedef struct
{
	uint8_t slaveAddress;
	uint8_t FunC;
	uint8_t DataStartRegisterH;
	uint8_t DataStartRegisterL;
	uint8_t DataRegsH;
	uint8_t DataRegsL;
	uint16_t crc;
} Ducati_Query_t;




uint16_t crcCalculation(uint8_t* rs485_data, int in_dx);

void Ducati_transmit(UART_HandleTypeDef *huart ,Ducati_Query_t Query);

void Ducati_Master_Receive( int length);
#endif /* Ducati_H_ */
