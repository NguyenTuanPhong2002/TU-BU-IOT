/*
 * application.h
 *
 *  Created on: Oct 26, 2023
 *      Author: NTPhong
 * 		Email: ntphong011102@gmail.com
 */

#ifndef APPLICATION_APPLICATION_H_
#define APPLICATION_APPLICATION_H_

#include "main.h"
#include "stm32f4xx_hal_uart.h"

#if 1
#include <stdint.h>
#endif

typedef enum
{
	APP_OK,
	APP_ERROR,
	APP_BUSY,
	APP_TIMEOUT
} APP_StatusTypeDef;


void APP_copyStringUntilToken(const char *src, char des[], uint16_t size, const char *token);

#endif /* APPLICATION_APPLICATION_H_ */
