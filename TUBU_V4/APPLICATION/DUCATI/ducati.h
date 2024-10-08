/*
 * DUCATI.h
 *
 *  Created on: Mar 22, 2024
 *      Author: Trump
 */

#ifndef INC_DUCATI_H_
#define INC_DUCATI_H_

#include "main.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> // Để sử dụng hàm malloc và NULL

#define LEVEL_BUFFER_SIZE 99U

typedef struct
{
	UART_HandleTypeDef *huart;
	DMA_HandleTypeDef *dma;
	/* data */
	uint8_t buffer[LEVEL_BUFFER_SIZE];
	uint16_t size;
	bool DucatiFlag;
} ducati_typeDef;

typedef struct
{
    GPIO_TypeDef *GPIO_PORT;
    uint8_t GPIO_PIN;
}STATUS_CONTACTOR;

bool Get_size(ducati_typeDef *me, uint16_t size);

uint16_t calculateCRC(const uint8_t *data, uint8_t length);
bool ducatiSendCommand(ducati_typeDef *me, uint8_t command[],
					   uint8_t size, uint32_t timeout);

float getVol(ducati_typeDef *me);
float getCosF(ducati_typeDef *me);
float getCurrent(ducati_typeDef *me);
float getFrequency(ducati_typeDef *me);

int16_t getAvQ(ducati_typeDef *me);
int16_t getAvP(ducati_typeDef *me);
float getActivePower(ducati_typeDef *me);
float getReactivePower(ducati_typeDef *me);

//void getStatusContac(STATUS_CONTACTOR *harware[8]);
uint16_t getCoilStatus(ducati_typeDef *me);

#endif /* INC_DUCATI_H_ */
