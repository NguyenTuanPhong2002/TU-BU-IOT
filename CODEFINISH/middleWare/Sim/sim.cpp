/*
 * sim.cpp
 *
 *  Created on: Oct 4, 2023
 *      Author: NTPhong
 * 		Email: ntphong01112002@gmail.com
 */

#include <sim.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

SIM_StatusTypeDef Sim::Sim_init(UART_HandleTypeDef *huart, DMA_HandleTypeDef *hdma)
{
    Sim7600(huart, hdma);
    if (simStart() != SIM_OK)
    {
        return SIM_ERROR;
    }
    return SIM_OK;
}

SIM_StatusTypeDef Sim::HTTP_downloadFile(const char *pURL, const char *pFilename)
{
    if (httpDownloadFile((char *)pURL, (char *)pFilename) != SIM_ERROR)
    {
        return SIM_OK;
    }
    else
    {
        return SIM_ERROR;
    }
}

uint32_t Sim::SIM_getFileSize(const char *pFilename)
{
    enum Enum
    {
        COMMAND_SIZE = 100
    };

    char command[COMMAND_SIZE] = {0};

    snprintf(command, sizeof(command), "AT+FSATTRI=%s", pFilename);

    sendATcommand("AT+FSCD=F:", SIM_URC_OK, 3000);
    sendATcommand("AT+FSLS=2", SIM_URC_OK, 3000);

    if (sendATcommand(command, SIM_URC_OK, 3000) == SIM_OK)
    {
        char *sPtr = strstr(this->buffer, "+FSATTRI: ");

        if (sPtr != NULL)
        {
            uint32_t filesize = atol(sPtr + 10);
            return filesize;
        }
    }

    return 0u;
}

uint32_t* Sim::SIM_getBuffer(){
    return (uint32_t *)this->buffer;
}