/*
 * DUCATI.c
 *
 *  Created on: Mar 22, 2024
 *      Author: Trump
 */

#include "ducati.h"

bool Get_size(ducati_typeDef *me, uint16_t size)
{
	me->size = size;
	HAL_UARTEx_ReceiveToIdle_DMA(me->huart, (uint8_t *)me->buffer, LEVEL_BUFFER_SIZE);
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

/**
 * The RS485SendCommand function sends a command over UART, waits for a response, and checks the
 * response for correctness based on a CRC calculation.
 * 
 * @param me `me` is a pointer to a structure of type `ducati_typeDef`. This structure likely contains
 * information and configurations related to the Ducati device, such as UART settings, buffer sizes,
 * and flags.
 * @param command The `command` parameter is an array of uint8_t type, which represents the command to
 * be sent over the RS485 communication.
 * @param size The `size` parameter in the `RS485SendCommand` function represents the size of the
 * `command` array that is being transmitted over UART. It indicates the number of bytes to be sent as
 * part of the command.
 * @param timeout The `timeout` parameter in the `RS485SendCommand` function is the maximum time in
 * milliseconds that the function will wait for a response before timing out and returning `false`.
 * 
 * @return The function `RS485SendCommand` returns a boolean value - `true` if the received packet's
 * CRC matches the calculated CRC, and `false` if the timeout is reached or the CRC does not match.
 */
bool ducatiSendCommand(ducati_typeDef *me, uint8_t command[],
					   uint8_t size, uint32_t timeout)
{

	HAL_UARTEx_ReceiveToIdle_DMA(me->huart, me->buffer, LEVEL_BUFFER_SIZE);
	__HAL_DMA_DISABLE_IT(me->dma, DMA_IT_HT);
	bool answer = false;

	memset(me->buffer, '\0', LEVEL_BUFFER_SIZE);
	HAL_UART_Transmit(me->huart, command, size, timeout);

	uint32_t tickStart = HAL_GetTick();
	while (answer == false)
	{
		if (HAL_GetTick() - tickStart >= timeout)
		{

			return false;
		}
		else
		{
			if (me->DucatiFlag == true)
			{
				uint16_t packetCRC = ((uint16_t)me->buffer[me->size - 1] << 8) | (me->buffer[me->size - 2]);

				if (calculateCRC((uint8_t *)me->buffer, me->size - 2) == packetCRC)
				{

					return true;
				}
			}
		}
		me->DucatiFlag = false;
	}
	return false;
}

float getVol(ducati_typeDef *me)
{
	uint8_t queryFrame[8] = {0x01, 0x03, 0x00, 0x03, 0x00, 0x02, 0x00, 0x00};
	uint16_t reponseCRC = calculateCRC(queryFrame, sizeof(queryFrame) - 2u);
	queryFrame[6] = (uint8_t)(reponseCRC & 0xFF);
	queryFrame[7] = (uint8_t)((reponseCRC >> 8) & 0xFF);

	ducatiSendCommand(me, queryFrame, sizeof(queryFrame), 1000);

	return ((uint16_t)me->buffer[3] << 8) | (uint16_t)me->buffer[4];
}

float getCosF(ducati_typeDef *me)
{
	uint8_t queryFrame[8] = {0x01, 0x03, 0x01, 0x61, 0x00, 0x02, 0x00, 0x00};
	uint16_t reponseCRC = calculateCRC(queryFrame, sizeof(queryFrame) - 2u);
	queryFrame[6] = (uint8_t)(reponseCRC & 0xFF);
	queryFrame[7] = (uint8_t)((reponseCRC >> 8) & 0xFF);

	ducatiSendCommand(me, queryFrame, sizeof(queryFrame), 1000);

	return ((uint16_t)me->buffer[3] << 8) | (uint16_t)me->buffer[4];
}

float getCurrent(ducati_typeDef *me)
{
	uint8_t queryFrame[8] = {0x01, 0x03, 0x00, 0x11, 0x00, 0x02, 0x00, 0x00};
	uint16_t reponseCRC = calculateCRC(queryFrame, sizeof(queryFrame) - 2u);
	queryFrame[6] = (uint8_t)(reponseCRC & 0xFF);
	queryFrame[7] = (uint8_t)((reponseCRC >> 8) & 0xFF);

	ducatiSendCommand(me, queryFrame, sizeof(queryFrame), 1000);

	return ((uint16_t)me->buffer[3] << 8) | (uint16_t)me->buffer[4];
}

float getFrequency(ducati_typeDef *me)
{
	uint8_t queryFrame[8] = {0x01, 0x03, 0x00, 0x01, 0x00, 0x02, 0x00, 0x00};
	uint16_t reponseCRC = calculateCRC(queryFrame, sizeof(queryFrame) - 2u);
	queryFrame[6] = (uint8_t)(reponseCRC & 0xFF);
	queryFrame[7] = (uint8_t)((reponseCRC >> 8) & 0xFF);

	ducatiSendCommand(me, queryFrame, sizeof(queryFrame), 1000);

	return ((uint16_t)me->buffer[3] << 8) | (uint16_t)me->buffer[4];
}

int16_t getAvQ(ducati_typeDef *me)
{
	uint8_t queryFrame[8] = {0x01, 0x03, 0x00, 0x53, 0x00, 0x02, 0x00, 0x00};
	uint16_t reponseCRC = calculateCRC(queryFrame, sizeof(queryFrame) - 2u);
	queryFrame[6] = (uint8_t)(reponseCRC & 0xFF);
	queryFrame[7] = (uint8_t)((reponseCRC >> 8) & 0xFF);

	ducatiSendCommand(me, queryFrame, sizeof(queryFrame), 1000);

	return ((uint16_t)me->buffer[3] << 8) | (uint16_t)me->buffer[4];
}

int16_t getAvP(ducati_typeDef *me)
{
	uint8_t queryFrame[8] = {0x01, 0x03, 0x00, 0x23, 0x00, 0x02, 0x00, 0x00};
	uint16_t reponseCRC = calculateCRC(queryFrame, sizeof(queryFrame) - 2u);
	queryFrame[6] = (uint8_t)(reponseCRC & 0xFF);
	queryFrame[7] = (uint8_t)((reponseCRC >> 8) & 0xFF);

	ducatiSendCommand(me, queryFrame, sizeof(queryFrame), 1000);

	return ((uint16_t)me->buffer[3] << 8) | (uint16_t)me->buffer[4];
}

float getActivePower(ducati_typeDef *me)
{
	uint8_t queryFrame[8] = {0x01, 0x03, 0x00, 0x21, 0x00, 0x02, 0x00, 0x00};
	uint16_t reponseCRC = calculateCRC(queryFrame, sizeof(queryFrame) - 2u);
	queryFrame[6] = (uint8_t)(reponseCRC & 0xFF);
	queryFrame[7] = (uint8_t)((reponseCRC >> 8) & 0xFF);

	ducatiSendCommand(me, queryFrame, sizeof(queryFrame), 1000);

	return ((uint16_t)me->buffer[3] << 8) | (uint16_t)me->buffer[4];
}

float getReactivePower(ducati_typeDef *me)
{
	uint8_t queryFrame[8] = {0x01, 0x03, 0x00, 0x51, 0x00, 0x02, 0x00, 0x00};
	uint16_t reponseCRC = calculateCRC(queryFrame, sizeof(queryFrame) - 2u);
	queryFrame[6] = (uint8_t)(reponseCRC & 0xFF);
	queryFrame[7] = (uint8_t)((reponseCRC >> 8) & 0xFF);

	ducatiSendCommand(me, queryFrame, sizeof(queryFrame), 1000);

	return ((uint16_t)me->buffer[3] << 8) | (uint16_t)me->buffer[4];
}


//void getStatusContac(STATUS_CONTACTOR *harware[8])
//{
//	bool StatusIn[8];
//    StatusIn[0] = HAL_GPIO_ReadPin(harware[0]->GPIO_PORT, harware[0]->GPIO_PIN);
//    StatusIn[1] = HAL_GPIO_ReadPin(harware[1]->GPIO_PORT, harware[1]->GPIO_PIN);
//    StatusIn[2] = HAL_GPIO_ReadPin(harware[2]->GPIO_PORT, harware[2]->GPIO_PIN);
//    StatusIn[3] = HAL_GPIO_ReadPin(harware[3]->GPIO_PORT, harware[3]->GPIO_PIN);
//    StatusIn[4] = HAL_GPIO_ReadPin(harware[4]->GPIO_PORT, harware[4]->GPIO_PIN);
//    StatusIn[5] = HAL_GPIO_ReadPin(harware[5]->GPIO_PORT, harware[5]->GPIO_PIN);
//    StatusIn[6] = HAL_GPIO_ReadPin(harware[6]->GPIO_PORT, harware[6]->GPIO_PIN);
//    StatusIn[7] = HAL_GPIO_ReadPin(harware[7]->GPIO_PORT, harware[7]->GPIO_PIN);
//}

uint16_t getCoilStatus(ducati_typeDef *me)
{
	uint8_t queryFrame[8] = {0x01, 0x01, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00};
	uint16_t reponseCRC = calculateCRC(queryFrame, sizeof(queryFrame) - 2u);
	queryFrame[6] = (uint8_t)(reponseCRC & 0xFF);
	queryFrame[7] = (uint8_t)((reponseCRC >> 8) & 0xFF);

	ducatiSendCommand(me, queryFrame, sizeof(queryFrame), 1000);

	return ((uint16_t)me->buffer[3] << 8) | (uint16_t)me->buffer[4];
}
