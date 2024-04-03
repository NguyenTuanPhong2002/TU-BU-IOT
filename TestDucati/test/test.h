/*
 * DUCATI.h
 *
 *  Created on: Mar 18, 2024
 *      Author: skttr
 */

#ifndef INC_DUCATI_H_
#define INC_DUCATI_H_

#include "main.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>// Để sử dụng hàm malloc và NULL

#define LEVEL_BUFFER_SIZE 99U

typedef struct {
	UART_HandleTypeDef *huart;
	DMA_HandleTypeDef *dma;
	/* data */
	uint8_t buffer[LEVEL_BUFFER_SIZE];
	uint16_t size;
	bool DucatiFlag;
} ducati_typeDef;

bool Get_size(ducati_typeDef *me, uint16_t size);

uint16_t calculateCRC(const uint8_t *data, uint8_t length);
bool ducatiSendCommand(ducati_typeDef *me, uint8_t command[],
		uint8_t size, uint32_t timeout);

uint16_t getVol(ducati_typeDef *me);

#endif /* INC_DUCATI_H_ */
