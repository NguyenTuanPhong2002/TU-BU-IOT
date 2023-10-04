/*
 * sim7600.h
 *
 *  Created on: Oct 3, 2023
 *      Author: NTPhong
 */

#ifndef SIM7600_H_
#define SIM7600_H_

#include "main.h"
#include "stm32f4xx_hal_uart.h"

#define SIM_BUFFER_SIZE 500

typedef enum
{
    SIM_OK,
    SIM_ERROR,
    SIM_TIMEOUT,
    SIM_BUSY,
} SIM_StatusTypeDef;

class sim7600
{
private:
    UART_HandleTypeDef *huart;
    DMA_HandleTypeDef *hdma;
    uint8_t rxBuffer[SIM_BUFFER_SIZE];
    uint8_t txBuffer[SIM_BUFFER_SIZE];
    uint8_t rxFlag;
public:
    SIM_StatusTypeDef sendATcommand(const char *ATCommand, const char *Response, uint32_t Timeout);
    SIM_StatusTypeDef simStart();

};

#endif /* SIM7600_H_ */
