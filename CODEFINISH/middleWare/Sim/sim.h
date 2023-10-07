/*
 * sim.h
 *
 *  Created on: Oct 4, 2023
 *      Author: NTPhong
 * 		Email: ntphong01112002@gmail.com
 */

#ifndef SIM_SIM_H_
#define SIM_SIM_H_

#include "sim7600.h"

#include "main.h"
#include "stm32f4xx_hal_uart.h"

#define SIM_URC_SMS "+CMTI: \"SM\","
#define SIM_URC_UNDER_VOLTAGE "UNDER-VOLTAGE WARNNING"
#define SIM_URC_SIMCARD_UNPLUGIN "+SIMCARD: NOT AVAILABLE"
#define SIM_URC_NETWORK_REPORT "+CNSMOD:"
#define SIM_URC_URC_SIGNAL_REPORT "+CSQ:"
#define SIM_URC_OK "OK"
#define SIM_URC_ERROR "ERROR"

class Sim : public Sim7600
{
private:
    /* data */
    char buffer[500];
public:
    SIM_StatusTypeDef Sim_init(UART_HandleTypeDef *huart, DMA_HandleTypeDef *hdma);
    SIM_StatusTypeDef HTTP_downloadFile(const char *pURL, const char *pFilename);
    uint32_t SIM_getFileSize(const char *pFilename);
    uint32_t* SIM_getBuffer();
};

#endif /* SIM_SIM_H_ */
