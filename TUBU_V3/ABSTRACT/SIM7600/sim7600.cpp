/*
 * sim7600.cpp
 *
 *  Created on: Oct 25, 2023
 *      Author: NTPhong
 * 		Email: ntphong011102@gmail.com
 */

#include "sim7600.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * The function `SIM7600_sendATCommand` sends an AT command to a SIM module and waits for a response,
 * checking for expected and unexpected responses within a specified timeout period.
 *
 * @param command The command to send to the SIM module.
 * @param expect The "expect" parameter is a string that represents the expected response from the SIM
 * module. The function will wait until it receives this response before returning APP_OK.
 * @param unexpect The "unexpect" parameter is a string that represents an unexpected response that you
 * do not want to receive from the SIM module. If the response received matches this string, the
 * function will return an error status.
 * @param timeout The timeout parameter is the maximum amount of time (in milliseconds) to wait for a
 * response from the SIM module before considering it a timeout.
 *
 * @return an APP_StatusTypeDef, which is an enumerated type defined in the code. The possible return
 * values are APP_OK, APP_ERROR, and APP_TIMEOUT.
 */
APP_StatusTypeDef SIM7600::SIM_sendATCommand(const char *command, const char *expect, const char *unexpect, uint32_t timeout)
{
    if (command == NULL || expect == NULL || unexpect == NULL)
    {
        return APP_ERROR;
    }

    APP_StatusTypeDef answer = APP_BUSY;

    /* Enable UART receive to IDLE DMA */
    HAL_UARTEx_ReceiveToIdle_DMA((UART_HandleTypeDef *)this->sim->huart, (uint8_t *)this->buffer, SIM_BUFFER_SIZE);
    __HAL_DMA_DISABLE_IT((DMA_HandleTypeDef *)this->sim->hdma, DMA_IT_HT);

    /* Clear buffer before receive new data */
    memset(this->buffer, '\0', SIM_BUFFER_SIZE);

    /* Sends command to SIM module: command + "\r\n" */
    HAL_UART_Transmit((UART_HandleTypeDef *)this->sim->huart, (uint8_t *)command, strlen(command),
                      HAL_MAX_DELAY);
    HAL_UART_Transmit((UART_HandleTypeDef *)this->sim->huart, (uint8_t *)"\r\n", 2, 1000);

    uint32_t tickStart = HAL_GetTick();

    /* Wait to receive the response - if the response is empty, the timeout is occured */
    while (answer == APP_BUSY)
    {
        const uint32_t timeElapsed = HAL_GetTick() - tickStart;

        if (timeElapsed >= timeout)
        {
            answer = APP_TIMEOUT;
            this->timeoutCnt++;

            /* Print the command and timeout counter */
            printf(command);
            printf("\n");
            printf("Timeout: %u\n", this->timeoutCnt);
            break;
        }
        else
        {
            if (SIM_GET_FLAG(&this->parent, SIM_FLAG_RX))
            {
                if (strstr(this->buffer, expect) != NULL)
                {
                    answer = APP_OK;
                    break;
                }
                else if (strstr(this->buffer, unexpect) != NULL)
                {
                    answer = APP_ERROR;
                    break;
                }
            }
        }
    }

    SIM_CLR_FLAG(&this->parent, SIM_FLAG_RX);
    /*	printf(me->parent.buffer); // Fatal Error: Không uncommment dòng này!
         Khi thực hiện tính năng FOTA, dòng này nhận được dữ liệu từ module và in ra,
         khi in ra thì nó k dừng lại mà sẽ in toàn bộ dữ liệu trong bộ nhớ của vi điều
         khiển và treo
    */
    return answer;
}

int8_t SIM7600::SIM_getRSSI()
{
    int8_t rssi = 0;

    APP_StatusTypeDef status = SIM_sendATCommand("AT+CSQ", SIM_URC_OK, SIM_URC_ERROR, 4000);

    if (APP_OK == status)
    {
        char *sPtr = strstr(this->buffer, " ");

        if (sPtr != NULL)
        {
            rssi = (2u * atoi(sPtr)) - 113u;
        }
    }

    return rssi;
}

uint8_t SIM7600::SIM_getNwtype(char pNwtype[], uint8_t size)
{
	return 0;
}

APP_StatusTypeDef SIM7600::SIM_getSimPhone(char pPhone[], uint8_t size)
{
    enum Enum
    {
        USSD_BUFFER_SIZE = 150
    };

    memset(pPhone, '\0', size);

    uint8_t counter = 0;

    char ussdResponse[USSD_BUFFER_SIZE] = {0};

    APP_StatusTypeDef status = SIM_sendATCommand("AT+COPS?", SIM_URC_OK, SIM_URC_ERROR, 4000);

    if (APP_OK == status)
    {
        /* "\r\n+COPS: 0,0,\"Viettel Viettel\",7\r\n\r\nOK\r\n" */
        do
        {
            /* Check if sim is Viettel Operator */
            if (strstr(this->buffer, "Viettel") != NULL ||
                strstr(this->buffer, "VIETTEL") != NULL)
            {
                status = SIM_callUSSD("*098#", ussdResponse, sizeof(ussdResponse));

                if (APP_OK == status)
                {
                    /* "Moi 01699741245 chon: */
                    memcpy(pPhone, "84", 2);

                    char *sPtr = strstr(ussdResponse, " ");
                    APP_copyStringUntilToken(sPtr + 2, pPhone + 2, size, " ");
                }
            }
            /* Check if sim is Vina Operator */
            else if (strstr(this->buffer, "Vina") != NULL ||
                     strstr(this->buffer, "VINA") != NULL)
            {
                status = SIM_callUSSD("*110#", ussdResponse, sizeof(ussdResponse));

                if (APP_OK == status)
                {
                    /* "TB (VINACARD):914486786 */
                    memcpy(pPhone, "84", 2);
                    char *sPtr = strstr(ussdResponse, ":");
                    APP_copyStringUntilToken(sPtr, pPhone + 2, size, "\r");
                }
            }
            /* Check if sim is Mobifone Operator */
            else if (strstr(this->buffer, "MOBI") != NULL ||
                     strstr(this->buffer, "Mobiphone") != NULL)
            {
                status = SIM_callUSSD("*0#", ussdResponse, sizeof(ussdResponse));

                if (APP_OK == status)
                {
                    /* "84914486786" */
                    char *sPtr = strstr(ussdResponse, "84");
                    APP_copyStringUntilToken(sPtr, pPhone, size, "\r");
                }
            }
            counter++;
        } while (counter < 3 && pPhone[0] == 0);
    }

    return status;
}

APP_StatusTypeDef SIM7600::SIM_callUSSD(const char *ussd, char response[], uint16_t size)
{
    enum Enum
    {
        COMMAND_SIZE = 40
    };

    char command[COMMAND_SIZE] = {0};

    snprintf(command, sizeof(command), "AT+CUSD=1,\"%s\",15", ussd);

    APP_StatusTypeDef status = SIM_sendATCommand(command, ",15", SIM_URC_ERROR, 15000);

    if (APP_OK == status)
    {
        char *sPtr = strstr(this->buffer, ",\"");

        APP_copyStringUntilToken(sPtr + 1, response, size, NULL);
        SIM_sendATCommand("AT+CUSD=2,\"\",15", SIM_URC_OK, SIM_URC_ERROR, 15000);
    }
    return status;
}

APP_StatusTypeDef SIM7600::SIM_getIMEI(char pIMEI[], uint8_t size)
{
    memset(pIMEI, '\0', size);

    uint8_t counter = 0;
    APP_StatusTypeDef status;

    do
    {
        status = SIM_sendATCommand("AT+SIMEI?", SIM_URC_OK, SIM_URC_ERROR, 4000);

        if (APP_OK == status)
        {
            char *sPtr = strstr(this->buffer, " ");
            APP_copyStringUntilToken(sPtr + 1, pIMEI, size, "\r");
        } 
        counter++;
    } while (pIMEI[0] == 0 && counter < 3);

    return status;
}
