/*
 * sim7600.cpp
 *
 *  Created on: Oct 3, 2023
 *      Author: NTPhong
 * 		Email: ntphong01112002@gmail.com
 */

/**
 * The code defines a class Sim7600 that provides functions for initializing and controlling a SIM7600
 * module for tasks such as sending SMS messages, making HTTP requests, and retrieving signal strength.
 * 
 * @return The functions in the code are returning values of type SIM_StatusTypeDef.
 */
#include "sim7600.h"

#include <stdio.h>
#include <cstring>
#include <stdlib.h>

/**
 * The code defines a class Sim7600 that provides functions for initializing and controlling a SIM7600
 * module for tasks such as sending SMS messages, making HTTP requests, and retrieving signal strength.
 * 
 * @return The functions in the code are returning values of type SIM_StatusTypeDef.
 */
/**
 * The function initializes DMA for receiving data from the SIM7600 module using UART.
 *
 * @return a value of type SIM_StatusTypeDef, which is likely an enumeration or typedef defined
 * elsewhere in the code. The specific value being returned is SIM_OK.
 */
SIM_StatusTypeDef Sim7600::initDMA()
{
	HAL_UARTEx_ReceiveToIdle_DMA(huart, this->rxBuffer, SIM_BUFFER_SIZE);
	__HAL_DMA_DISABLE_IT(hdma, DMA_IT_HT);
	return SIM_OK;
}
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
SIM_StatusTypeDef Sim7600::sendATcommand(const char *ATCommand, const char *Response, uint32_t Timeout)
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
 * The function `simStart()` initializes the SIM7600 module by sending a series of AT commands and
 * checking for the expected response.
 *
 * @return a value of type SIM_StatusTypeDef.
 */
SIM_StatusTypeDef Sim7600::simStart()
{
	if (sendATcommand("ATE0", "OK", 1000) == SIM_ERROR)
	{
		return SIM_ERROR;
	}
	if (sendATcommand("AT+CPIN?", "OK", 5000) == SIM_ERROR)
	{
		return SIM_ERROR;
	}
	if (sendATcommand("AT+CGREG?", "OK", 500) == SIM_ERROR)
	{
		return SIM_ERROR;
	}
	if (sendATcommand("AT+COPS?", "OK", 1000) == SIM_ERROR)
	{
		return SIM_ERROR;
	}
	if (sendATcommand("AT+CNMP=2", "OK", 1000) == SIM_ERROR)
	{
		return SIM_ERROR;
	}
	if (sendATcommand("ATI", "OK", 5000) == SIM_ERROR)
	{
		return SIM_ERROR;
	}
	if (sendATcommand("AT+CNMI=2,1,0,0,0", "OK", 5000) == SIM_ERROR)
	{
		return SIM_ERROR;
	}
	if (sendATcommand("AT+CNMP=2", "OK", 1000) == SIM_ERROR)
	{
		return SIM_ERROR;
	}
	if (sendATcommand("AT+CPMS=\"SM\",\"SM\",\"SM\"", "OK", 2000) == SIM_ERROR)
	{
		return SIM_ERROR;
	}
	if (sendATcommand("AT+CMGL=\"ALL\"", "OK", 5000) == SIM_ERROR)
	{
		return SIM_ERROR;
	}
	if (sendATcommand("AT+CMGD=,4", "OK", 1000) == SIM_ERROR)
	{
		return SIM_ERROR;
	}
	if (sendATcommand("AT+CMGF=1", "OK", 1000) == SIM_ERROR)
	{
		return SIM_ERROR;
	}
	return SIM_OK;
}

/**
 * The function `getsingle()` retrieves the signal strength of the SIM7600 module and returns it as a
 * value between 0 and 31.
 *
 * @return an unsigned 8-bit integer (uint8_t).
 */
uint8_t Sim7600::getsingle()
{
	if (SIM_OK == sendATcommand("AT+CSQ", "OK", 9000))
	{
		char *token = strtok((char *)rxBuffer, " "); //\r\n+CSQ:\0
		token = strtok(NULL, ",");					 // 22\0
		return (2 * atoi(token)) - 113;
	}
	else
	{
		return 0;
	}
};

/**
 * The function "isReady" checks if the SIM7600 module is ready by sending an AT command and waiting
 * for a response.
 *
 * @return a SIM_StatusTypeDef value.
 */
SIM_StatusTypeDef Sim7600::isReady()
{
	return sendATcommand("AT+CPIN?", "OK", 9000);
}

/**
 * The function sets up DMA to receive data from the UART and enables the DMA interrupt.
 *
 * @return a value of type SIM_StatusTypeDef, which is likely an enumeration or typedef defined
 * elsewhere in the code. The specific value being returned is SIM_OK.
 */
SIM_StatusTypeDef Sim7600::rxEventCallback()
{
	HAL_UARTEx_ReceiveToIdle_DMA(huart, this->rxBuffer, SIM_BUFFER_SIZE);
	__HAL_DMA_DISABLE_IT(hdma, DMA_IT_HT);
	rxFlag = 1;
	return SIM_OK;
}

/**
 * The "reset" function resets the SIM7600 module by toggling the RST_PIN and configuring the UART DMA
 * for receiving data.
 */
void Sim7600::reset()
{

	HAL_GPIO_WritePin(RST_PORT, RST_PIN, GPIO_PIN_SET);
	HAL_Delay(500);
	HAL_GPIO_WritePin(RST_PORT, RST_PIN, GPIO_PIN_RESET);
	HAL_Delay(1000);
	HAL_UARTEx_ReceiveToIdle_DMA(huart, this->rxBuffer, SIM_BUFFER_SIZE);
	__HAL_DMA_DISABLE_IT(hdma, DMA_IT_HT);
}

/**
 * The function `sendSMS` sends an SMS message to a specified phone number using the SIM7600 module.
 *
 * @param phoneNumber The phoneNumber parameter is a pointer to a character array that represents the
 * phone number to which the SMS message will be sent.
 * @param message The "message" parameter is a pointer to a character array that contains the content
 * of the SMS message that you want to send.
 *
 * @return a SIM_StatusTypeDef, which is a custom data type that represents the status of the SIM
 * operation.
 */
SIM_StatusTypeDef Sim7600::sendSMS(const char *phoneNumber, const char *message)
{
	uint8_t number[30] = {0};
	uint8_t sms[SIM_BUFFER_SIZE] = {0};
	SIM_StatusTypeDef status;
	sendATcommand("AT+CMGF=1", "OK", 9000);

	sprintf((char *)number, "AT+CMGS=\"%s\"", phoneNumber);
	status = sendATcommand((char *)number, ">", 9000);
	if (status == SIM_OK)
	{
		sprintf((char *)sms, "%s%c", message, 0x1A);
		return sendATcommand((char *)sms, "OK", 9000);
	}
	else
	{
		return status;
	}
}

/**
 * The function "realTime" sends an AT command to the Sim7600 module to retrieve the current real-time
 * clock value.
 * 
 * @return a pointer to a character array (string).
 */
char *Sim7600::realTime()
{
	sendATcommand("AT+CCLK=?", "OK", 1000);
	return (char*)this->rxBuffer;
}


/**
 * The function "readSMS" in the Sim7600 class reads an SMS message at a specified index and returns
 * the message as a character array.
 * 
 * @param index The index parameter is an unsigned 8-bit integer that represents the index of the SMS
 * message to be read.
 * 
 * @return a pointer to a character array (char *) which represents the received SMS message.
 */
char *Sim7600::readSMS(uint8_t index)
{
	uint8_t send[12] = {0};
	sprintf((char *)send, "AT+CMGR=%d", index);
	sendATcommand((char *)send, "OK", 9000);
	return (char*)this->rxBuffer;
}

/**
 * The function "httpStart" initializes the HTTP connection with the SIM7600 module.
 * 
 * @return a SIM_StatusTypeDef, which is a user-defined data type.
 */
SIM_StatusTypeDef Sim7600::httpStart()
{
	return sendATcommand("AT+HTTPINIT", "OK", 12000);
}

/**
 * The function sets the URL for an HTTP request in the SIM7600 module.
 * 
 * @param url The `url` parameter is a pointer to a character array that represents the URL to be set
 * for the HTTP request.
 * 
 * @return a value of type SIM_StatusTypeDef.
 */
SIM_StatusTypeDef Sim7600::httpSetUrl(const char *url)
{
	uint8_t cmd[100] = {0};
	sprintf((char *)cmd, "AT+HTTPPARA=\"URL\",\"%s\"", url);
	return sendATcommand((char *)cmd, "OK", 12000);
}

/**
 * The function `httpStop()` stops the HTTP connection.
 * 
 * @return a SIM_StatusTypeDef, which is a user-defined data type.
 */
SIM_StatusTypeDef Sim7600::httpStop()
{
	return sendATcommand("AT+HTTPTERM", "OK", 12000);
}

/**
 * The function `httpReadResponse` sends an AT command to read the HTTP response from the SIM7600
 * module.
 * 
 * @return a value of type SIM_StatusTypeDef.
 */
SIM_StatusTypeDef Sim7600::httpReadResponse()
{
	sendATcommand("AT+HTTPREAD?", "OK", 2000);
	char *token[2] = {NULL};
	char *cmd = {NULL};
	token[0] = strtok((char *)this->rxBuffer, ",");
	token[1] = strtok(NULL, "");
	sprintf(cmd, "AT+HTTPREAD=0,%s", token[1]);
	return sendATcommand(cmd, "OK", 9000);
}

/**
 * The function `httpPost` sends an HTTP POST request with the specified data and timeout.
 * 
 * @param data The "data" parameter is a pointer to a character array that contains the data to be sent
 * in the HTTP POST request. It is assumed to be in JSON format.
 * @param timeout The timeout parameter is the maximum time in milliseconds that the function will wait
 * for a response from the SIM7600 module before timing out.
 * 
 * @return a SIM_StatusTypeDef, which is a user-defined data type. The specific value being returned
 * depends on the outcome of the function.
 */
SIM_StatusTypeDef Sim7600::	httpPost(const char *data, int timeout)
{
	sendATcommand("AT+HTTPPARA=\"CONTENT\",\"application/json\"", "OK", 12000);
	uint8_t cmd[30] = {0};
	uint8_t len = strlen(data);
	sprintf((char *)cmd, "AT+HTTPDATA=%d,%d", len, timeout);
	if (sendATcommand((char *)cmd, "DOWNLOAD", 12000) != SIM_OK)
	{
		return sendATcommand((char *)cmd, "DOWNLOAD", 12000);
	}
	else
	{
		sendATcommand(data, "OK", 12000);
	}
	return sendATcommand("AT+HTTPACTION=1", "+HTTPACTION: 1,200", 12000);
}

/**
 * The function `httpDownloadFile` is used to download a file from a specified URL and save it with a
 * specified filename.
 * 
 * @param url The URL of the file you want to download.
 * @param filename The filename parameter is a character array that represents the name of the file to
 * be downloaded.
 * 
 * @return a value of type SIM_StatusTypeDef.
 */
SIM_StatusTypeDef Sim7600::httpDownloadFile(char *url, char *filename)
{
	uint8_t cmd[200] = {0};

	sprintf((char *)cmd, "AT+HTTPPARA=\"URL\",\"%s\"\r", url);
	if (sendATcommand((char *)cmd, "OK", 12000) != SIM_OK)
	{
		return SIM_ERROR;
	}
	if (sendATcommand("AT+HTTPACTION=0", "+HTTPACTION: 0,200", 2000) != SIM_OK)
	{
		return SIM_ERROR;
	}
	uint8_t len = sizeof(cmd);
	for (uint8_t i = 0; i < len; i++)
	{
		cmd[i] = 0x00;
	}
	sprintf((char *)cmd, "AT+HTTPREADFILE=\"%s\",3\r", filename);
	return sendATcommand((char *)cmd, "+HTTPREADFILE:0", 60000);
}
