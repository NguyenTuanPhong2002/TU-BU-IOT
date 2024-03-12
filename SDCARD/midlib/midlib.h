/*
 * midlib.h
 *
 *  Created on: Mar 12, 2024
 *      Author: Trump
 */

#ifndef MIDLIB_H_
#define MIDLIB_H_

#include "main.h"

typedef enum
{
    THT_BUSY,
    THT_OK,
    THT_ERROR,
    THT_TIMEOUT
} THT_StatusTypeDef;

//typedef struct
//{
//	void *hspi;
//	uint16_t resetPin;
//	uint16_t nssPin;
//	uint16_t dio0;
//	uint16_t dio1;
//	uint16_t dio2;
//	uint16_t powerEnPin;
//	GPIO_TypeDef *resetPort;
//	GPIO_TypeDef *nssPort;
//	GPIO_TypeDef *dio0Port;
//	GPIO_TypeDef *dio1Port;
//	GPIO_TypeDef *dio2Port;
//	GPIO_TypeDef *powerEnPort;
//} SDCARD_HandleTypeDef;

#endif /* MIDLIB_H_ */
