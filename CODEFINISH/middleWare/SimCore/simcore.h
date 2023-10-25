/*
 * sim.h
 *
 *  Created on: Oct 4, 2023
 *      Author: NTPhong
 * 		Email: ntphong011102@gmail.com
 */

#ifndef SIMCORE_SIMCORE_H_
#define SIMCORE_SIMCORE_H_

#include "sim7600.h"

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
    SIM_StatusTypeDef Sim_init();
    SIM_StatusTypeDef HTTP_downloadFile(const char *pURL, const char *pFilename);
    uint32_t SIM_getFileSize(const char *pFilename);
    uint32_t *SIM_getBuffer();
};



#endif /* SIMCORE_SIMCORE_H_ */
