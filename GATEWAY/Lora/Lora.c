/*
 * Lora.c
 *
 *  Created on: Mar 11, 2024
 *      Author: Trump
 */

#include "lora.h"

#include "../RFM/rfm.h"

THT_StatusTypeDef LORA_init(LORA_HandleTypeDef *const me)
{
	if (me == NULL)
	{
		return THT_ERROR;
	}
	return (*me->init)(me);
}

THT_StatusTypeDef LORA_transmit(LORA_HandleTypeDef *const me, const uint8_t *pBuffer, uint16_t size, uint32_t timeout)
{
	if (me == NULL || pBuffer == NULL)
	{
		return THT_ERROR;
	}

	return (*me->transmit)(me, pBuffer, size, timeout);
}

THT_StatusTypeDef LORA_receivePolling(LORA_HandleTypeDef *const me, uint8_t *pBuffer, uint16_t size, uint32_t timeout)
{
	if (me == NULL || pBuffer == NULL)
	{
		return THT_ERROR;
	}

	return (*me->receive)(me, pBuffer, size, timeout);
}

THT_StatusTypeDef LORA_startReceiveIT(LORA_HandleTypeDef *const me)
{
	if (me == NULL)
	{
		return THT_ERROR;
	}

	return (*me->startReceiveIT)(me);
}

THT_StatusTypeDef LORA_receiveIT(LORA_HandleTypeDef *const me, uint8_t *pBuffer, uint16_t size)
{
	if (me == NULL || pBuffer == NULL)
	{
		return THT_ERROR;
	}

	return (*me->receiveIT)(me, pBuffer, size);
}

int16_t LORA_getRSSI(LORA_HandleTypeDef *const me)
{
	if (me == NULL)
	{
		return THT_ERROR;
	}
	return (*me->getRSSI)(me);
}
