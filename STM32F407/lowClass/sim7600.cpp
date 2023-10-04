/*
 * sim7600.cpp
 *
 *  Created on: Oct 3, 2023
 *      Author: NTPhong
 */

#include "sim7600.h"

#include <stdio.h>
#include <cstring>

/**
 * The function `sendATcommand` sends an AT command to a SIM7600 module and waits for a response within
 * a specified timeout period.
 * 
 * @param ATCommand The AT command to send to the SIM7600 module.
 * @param Response The "Response" parameter is a string that represents the expected response from the
 * SIM7600 module after sending the AT command. The function will check if this response is present in
 * the received data from the module. If the response is found, the function will return SIM_OK. If the
 * response is not
 * @param Timeout The Timeout parameter is the maximum time in milliseconds that the function will wait
 * for a response from the SIM7600 module before considering it a timeout.
 * 
 * @return a value of type SIM_StatusTypeDef.
 */
SIM_StatusTypeDef sim7600::sendATcommand(const char *ATCommand, const char *Response, uint32_t Timeout)
{
	HAL_UARTEx_ReceiveToIdle_DMA(huart, this->rxBuffer, SIM_BUFFER_SIZE);
	__HAL_DMA_DISABLE_IT(hdma, DMA_IT_HT);
	SIM_StatusTypeDef status = SIM_BUSY;
	memset((char *)rxBuffer, 0, SIM_BUFFER_SIZE);
	memset((char *)txBuffer, 0, SIM_BUFFER_SIZE);
	sprintf((char *)txBuffer, "%s\r\n", ATCommand);
	uint16_t len = strlen((char *)txBuffer);
	HAL_UART_Transmit(huart, (uint8_t *)txBuffer, len, HAL_MAX_DELAY);
	//    print((char*) txBuffer);
	uint32_t tickStart = HAL_GetTick();
	while (status == SIM_BUSY)
	{
		if ((HAL_GetTick() - tickStart >= Timeout))
		{
			status = SIM_TIMEOUT;
			break;
		}
		if (rxFlag == true)
		{
			if (strstr((char *)rxBuffer, Response) != NULL)
			{
				status = SIM_OK;
				break;
			}
			else if (strstr((char *)rxBuffer, "ERROR") != NULL)
			{
				status = SIM_ERROR;
				break;
			}
		}
	}
	//	print((char*) rxBuffer);
	rxFlag = false;
	return status;
}

/**
 * The simStart function initializes and configures the SIM7600 module for communication.
 * 
 */
SIM_StatusTypeDef sim7600::simStart()
{
	sendATcommand("ATE0", "OK", 1000);
	sendATcommand("AT+CPIN?", "OK", 5000);
	sendATcommand("AT+CGREG?", "OK", 500);
	sendATcommand("AT+COPS?", "OK", 1000);
	sendATcommand("AT+CNMP=2", "OK", 1000);
	sendATcommand("ATI", "OK", 5000);
	sendATcommand("AT+CNMI=2,1,0,0,0", "OK", 5000);
	sendATcommand("AT+CNMP=2", "OK", 1000);
	sendATcommand("AT+CPMS=\"SM\",\"SM\",\"SM\"", "OK", 2000);
	sendATcommand("AT+CMGL=\"ALL\"", "OK", 5000);
	sendATcommand("AT+CMGD=,4", "OK", 1000);
	sendATcommand("AT+CMGF=1", "OK", 1000);
}