/*
 * sim.cpp
 *
 *  Created on: Oct 4, 2023
 *      Author: NTPhong
 * 		Email: ntphong011102@gmail.com
 */

#include <simcore.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * The function initializes the SIM module and returns the status of the initialization.
 *
 * @param huart The "huart" parameter is a pointer to the UART (Universal Asynchronous
 * Receiver/Transmitter) handle. It is used to configure and control the UART peripheral for
 * communication with the SIM7600 module.
 * @param hdma The "hdma" parameter is a pointer to a DMA_HandleTypeDef structure. This structure is
 * used to configure and control the DMA (Direct Memory Access) controller, which allows for efficient
 * data transfers between peripherals and memory without CPU intervention.
 *
 * @return a value of type SIM_StatusTypeDef.
 */
SIM_StatusTypeDef Sim::Sim_init()
{
   if (simStart() != SIM_OK)
   {
       return SIM_ERROR;
   }
   return SIM_OK;
}

/**
 * The function `HTTP_downloadFile` downloads a file from a given URL and saves it with a specified
 * filename.
 *
 * @param pURL The pURL parameter is a pointer to a character array that represents the URL of the file
 * to be downloaded.
 * @param pFilename The pFilename parameter is a pointer to a character array that represents the name
 * of the file to be downloaded.
 *
 * @return a value of type SIM_StatusTypeDef.
 */
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

/**
 * The function `SIM_getFileSize` is a C++ function that retrieves the size of a file using AT
 * commands.
 *
 * @param pFilename A pointer to a character array representing the filename of the file whose size
 * needs to be determined.
 *
 * @return a uint32_t value, which represents the file size.
 */
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

/**
 * The function SIM_getBuffer returns a pointer to a buffer of type uint32_t.
 *
 * @return A pointer to a uint32_t buffer.
 */
uint32_t* Sim::SIM_getBuffer(){
    return (uint32_t *)this->buffer;
}
