/*
 * DUCATI.c
 *
 *  Created on: Mar 18, 2024
 *      Author: skttr
 */

#include "test.h"

bool Get_size(ducati_typeDef *me, uint16_t size) {
	me->size = size;
	HAL_UARTEx_ReceiveToIdle_DMA(me->huart, (uint8_t*) me->buffer,LEVEL_BUFFER_SIZE);
	return true;
}

uint16_t calculateCRC(const uint8_t *data, uint8_t length)
{
	uint16_t crc = 0xFFFF;
	for (int i = 0; i < length; i++)
	{
		crc ^= data[i];
		for (int j = 0; j < 8; j++)
		{
			if (crc & 0x0001)
			{
				crc = (crc >> 1) ^ 0xA001;
			}
			else
			{
				crc = crc >> 1;
			}
		}
	}
	return crc;
}
bool ducatiSendCommand(ducati_typeDef *me, uint8_t command[],
		uint8_t size, uint32_t timeout) {

	HAL_UARTEx_ReceiveToIdle_DMA(me->huart,  me->buffer,LEVEL_BUFFER_SIZE);
	__HAL_DMA_DISABLE_IT(me->dma, DMA_IT_HT);
	bool answer = false;

	memset(me->buffer, '\0', LEVEL_BUFFER_SIZE);
	HAL_UART_Transmit(me->huart, command, size, timeout);

	uint32_t tickStart = HAL_GetTick();
	while (answer == false) {
		if (HAL_GetTick() - tickStart >= timeout) {

			return false;
		} else {
			if (me->DucatiFlag == true) {
				uint16_t packetCRC = ((uint16_t) me->buffer[me->size - 1] << 8)
						| (me->buffer[me->size - 2]);

				if (calculateCRC((uint8_t*) me->buffer, me->size - 2)
						== packetCRC) {

					return true;
				}
			}
		}
		me->DucatiFlag = false;

}
	return false;
}


uint16_t getVol(ducati_typeDef *me)
{
	uint8_t queryFrame[8] = {0x01, 0x03, 0x00, 0x03, 0x00, 0x02, 0x00, 0x00};
	uint16_t reponseCRC = calculateCRC(queryFrame, sizeof(queryFrame) - 2u);
	queryFrame[6] = (uint8_t)(reponseCRC & 0xFF);
	queryFrame[7] = (uint8_t)((reponseCRC >> 8) & 0xFF);

	ducatiSendCommand(me,queryFrame, sizeof(queryFrame), 1000);

	return ((uint16_t)me->buffer[3] << 8) | (uint16_t)me->buffer[4];
}
