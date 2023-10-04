/*
 * sim7600.h
 *
 *  Created on: Oct 3, 2023
 *      Author: NTPhong
 * 		Email: ntphong01112002@gmail.com
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

class Sim7600
{
private:
    GPIO_TypeDef *RST_PORT;
    uint8_t RST_PIN;

    GPIO_TypeDef *PWR_PORT;
    uint8_t PWR_PIN;
    UART_HandleTypeDef *huart;
    DMA_HandleTypeDef *hdma;
    uint8_t rxBuffer[SIM_BUFFER_SIZE];
    uint8_t txBuffer[SIM_BUFFER_SIZE];
    uint8_t rxFlag;

public:
    SIM_StatusTypeDef initDMA();
    SIM_StatusTypeDef sendATcommand(const char *ATCommand, const char *Response, uint32_t Timeout);
    SIM_StatusTypeDef simStart();
    uint8_t getsingle();
    SIM_StatusTypeDef isReady();
    SIM_StatusTypeDef rxEventCallback();
    void reset();
    SIM_StatusTypeDef sendSMS(const char *phoneNumber, const char *message);
    char *realTime();
    char *readSMS(uint8_t index);
    SIM_StatusTypeDef httpStart();
    SIM_StatusTypeDef httpSetUrl(const char *url);
    SIM_StatusTypeDef httpStop();
    SIM_StatusTypeDef httpReadResponse();
    SIM_StatusTypeDef httpPost(const char *data, int timeout);
    SIM_StatusTypeDef httpDownloadFile(char *url, char *filename);
};

#endif /* SIM7600_H_ */
