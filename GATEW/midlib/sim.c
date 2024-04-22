/*
 * sim.c
 *
 *  Created on: Mar 23, 2023
 *      Author: Hoang Van Binh (gmail: binhhv.23.1.99@gmail.com)
 */

#include "sim.h"
#include <stdio.h>
/**
 * @brief  Get data of module SIM
 * @param me [in] A pointer to SIM_HandleTypeDef structure [in] SIM Handle
 * @param data [in] combine of data want to get:
 * 				@arg SIM_DR_RSSI
 * 				@arg SIM_DR_NWTYPE
 * 				@arg SIM_DR_SIMPHONE
 * 				@arg SIM_DR_IMEI
 * 				@arg SIM_DR_TIME
 * @param [out] pData a pointer to SIM data structure
 */
THT_StatusTypeDef SIM_getData(SIM_HandleTypeDef *const me, uint32_t data, SIM_DataTypeDef *pData)
{
	if (me == NULL)
	{
		return THT_ERROR;
	}

	if ((data & SIM_DR_RSSI) == SIM_DR_RSSI)
	{
		pData->rssi = (*me->simGetRSSI)(me);
	}
	if ((data & SIM_DR_NWTYPE) == SIM_DR_NWTYPE)
	{
		(*me->simGetNwtype)(me, pData->network, sizeof(pData->network));
	}
	if ((data & SIM_DR_SIMPHONE) == SIM_DR_SIMPHONE)
	{
		(*me->simGetSimPhone)(me, pData->simPhone, sizeof(pData->simPhone));
	}
	if ((data & SIM_DR_IMEI) == SIM_DR_IMEI)
	{
		(*me->simGetIMEI)(me, pData->imei, sizeof(pData->imei));
	}
	if ((data & SIM_DR_TIME) == SIM_DR_TIME)
	{
		(*me->simGetLocalTime)(me, pData->timezone, sizeof(pData->timezone));
	}
	if ((data & SIM_DR_TEMP) == SIM_DR_TEMP)
	{
		pData->temp = (*me->simGetTemperature)(me);
	}
	return THT_OK;
}

/**
 * @brief send AT command
 *
 * @param me [in] A pointer to SIM_HandleTypeDef structure
 * @param command
 * @param expect
 * @param unexpect
 * @param timeout
 * @return THT_StatusTypeDef
 */
THT_StatusTypeDef SIM_sendATCommand(SIM_HandleTypeDef *const me, const char *command, const char *expect, const char *unexpect, uint32_t timeout)
{
	if (me == NULL || command == NULL || expect == NULL || unexpect == NULL)
	{
		return THT_ERROR;
	}

	return (*me->simSendATCommand)(me, command, expect, unexpect, timeout);
}

/**
 * @brief Init module SIM
 *
 * @param me [in] A pointer to SIM_HandleTypeDef structure
 * @return THT_StatusTypeDef
 */
THT_StatusTypeDef SIM_init(SIM_HandleTypeDef *const me)
{
	if (me == NULL)
	{
		return THT_ERROR;
	}

	return (*me->simInit)(me);
}

/**
 * @brief Wake up module SIM
 *
 * @param me [in] A pointer to SIM_HandleTypeDef structure
 */
void SIM_wakeup(SIM_HandleTypeDef *const me)
{
	if (me == NULL)
	{
		return;
	}

	(*me->simWakeup)(me);
}

/**
 * @brief Enter sleep mode
 *
 * @param me [in] A pointer to SIM_HandleTypeDef structure
 */
void SIM_sleep(SIM_HandleTypeDef *const me)
{
	if (me == NULL)
	{
		return;
	}

	(*me->simSleep)(me);
}

/**
 * @brief Set functional
 *
 * @param me [in] A pointer to SIM_HandleTypeDef structure
 * @param mode
 * @return THT_StatusTypeDef
 */
THT_StatusTypeDef SIM_setFunction(SIM_HandleTypeDef *const me, uint8_t mode)
{
	if (me == NULL)
	{
		return THT_ERROR;
	}

	return (*me->simSetFunction)(me, mode);
}

/**
 * @brief SIM process
 *
 * @param me [in] A pointer to SIM_HandleTypeDef structure
 */
void SIM_process(SIM_HandleTypeDef *const me)
{
	if (me == NULL)
	{
		return;
	}

	(*me->simProcess)(me);
}

/**
 * @brief SIM Check Event
 *
 * @param me [in] A pointer to SIM_HandleTypeDef structure
 * @param size
 */
void SIM_checkEvent(SIM_HandleTypeDef *const me, uint16_t size)
{
	if (me == NULL)
	{
		return;
	}

	(*me->simCheckEvent)(me, size);
}

/**
 * @brief SIM read SMS
 *
 * @param me [in] A pointer to SIM_HandleTypeDef structure
 * @param order
 * @param pSMS
 * @return THT_StatusTypeDef
 */
THT_StatusTypeDef SIM_readSMS(SIM_HandleTypeDef *const me, uint8_t order, SIM_SMSTypeDef *pSMS)
{
	if (me == NULL || pSMS == NULL)
	{
		return THT_ERROR;
	}

	printf("Read SMS:\n"
			"Order: %u\n\n", order);

	return (*me->simReadSMS)(me, order, pSMS);
}

/**
 * @brief SIM send SMS
 *
 * @param me [in] A pointer to SIM_HandleTypeDef structure
 * @param pPhone
 * @param pMsg
 * @param phoneSize
 * @param msgSize
 * @return THT_StatusTypeDef
 */
THT_StatusTypeDef SIM_sendSMS(SIM_HandleTypeDef *const me, const char *pPhone, const char *pMsg, uint8_t phoneSize, uint16_t msgSize)
{
	if (me == NULL || pPhone == NULL || pMsg == NULL)
	{
		return THT_ERROR;
	}

	printf("Send SMS:\n"
			"To: %s\n"
			"Content: %s\n\n", pPhone, pMsg);

	return (*me->simSendSMS)(me, pPhone, pMsg, phoneSize, msgSize);
}

/**
 * @brief Get SMS counter
 *
 * @param me [in] A pointer to SIM_HandleTypeDef structure
 * @return int8_t
 */
int8_t SIM_getSMSCounter(SIM_HandleTypeDef *const me)
{
	if (me == NULL)
	{
		return THT_ERROR;
	}

	return (*me->simGetSMSCounter)(me);
}

/**
 * @brief Request USSD
 *
 * @param me [in] A pointer to SIM_HandleTypeDef structure
 * @param ussd
 * @param response
 * @param size
 * @return THT_StatusTypeDef
 */
THT_StatusTypeDef SIM_callUSSD(SIM_HandleTypeDef *const me, const char *ussd, char response[], uint16_t size)
{
	if (me == NULL || ussd == NULL || response == NULL)
	{
		return THT_ERROR;
	}

	return (*me->simCallUSSD)(me, ussd, response, size);
}

/**
 * @brief Get RSSI
 *
 * @param me [in] A pointer to SIM_HandleTypeDef structure
 * @return int8_t
 */
int8_t SIM_getRSSI(SIM_HandleTypeDef *const me)
{
	if (me == NULL)
	{
		return THT_ERROR;
	}

	return (*me->simGetRSSI)(me);
}

/**
 * @brief Get Nework Type
 *
 * @param me [in] A pointer to SIM_HandleTypeDef structure
 * @param pNwtype
 * @param size
 * @return uint8_t
 */
uint8_t SIM_getNwtype(SIM_HandleTypeDef *const me, char pNwtype[], uint8_t size)
{
	if (me == NULL)
	{
		return THT_ERROR;
	}

	return (*me->simGetNwtype)(me, pNwtype, size);
}

/**
 * @brief Get Sim Phone Number
 *
 * @param me [in] A pointer to SIM_HandleTypeDef structure
 * @param pPhone
 * @param size
 * @return THT_StatusTypeDef
 */
THT_StatusTypeDef SIM_getSimPhone(SIM_HandleTypeDef *const me, char pPhone[], uint8_t size)
{
	if (me == NULL || pPhone == NULL)
	{
		return THT_ERROR;
	}

	return (*me->simGetSimPhone)(me, pPhone, size);
}

/**
 * @brief Get IMEI
 *
 * @param me [in] A pointer to SIM_HandleTypeDef structure
 * @param pIMEI
 * @param size
 * @return THT_StatusTypeDef
 */
THT_StatusTypeDef SIM_getIMEI(SIM_HandleTypeDef *const me, char pIMEI[], uint8_t size)
{
	if (me == NULL || pIMEI == NULL)
	{
		return THT_ERROR;
	}

	return (*me->simGetIMEI)(me, pIMEI, size);
}

/**
 * @brief Get Local Time
 *
 * @param me [in] A pointer to SIM_HandleTypeDef structure
 * @param pTime
 * @param size
 * @return THT_StatusTypeDef
 */
THT_StatusTypeDef SIM_getLocalTime(SIM_HandleTypeDef *const me, char pTime[], uint8_t size)
{
	if (me == NULL || pTime == NULL)
	{
		return THT_ERROR;
	}

	return (*me->simGetLocalTime)(me, pTime, size);
}

/**
 * @brief Get Size of File
 *
 * @param me [in] A pointer to SIM_HandleTypeDef structure
 * @param pFilename
 * @return uint32_t
 */
uint32_t SIM_getFileSize(SIM_HandleTypeDef *const me, const char *pFilename)
{
	if (me == NULL || pFilename == NULL)
	{
		return THT_ERROR;
	}

	return (*me->simGetFileSize)(me, pFilename);
}

/**
 * @brief Delete File
 *
 * @param me [in] A pointer to SIM_HandleTypeDef structure
 * @param pFilename
 * @return THT_StatusTypeDef
 */
THT_StatusTypeDef SIM_deleteFile(SIM_HandleTypeDef *const me, const char *pFilename)
{
	if (me == NULL || pFilename == NULL)
	{
		return THT_ERROR;
	}

	printf("Delete file:\n"
			"Filename: %s\n\n", pFilename);

	return (*me->simDeleteFile)(me, pFilename);
}

/**
 * @brief Set Local time Automatically
 *
 * @param me [in] A pointer to SIM_HandleTypeDef structure
 * @return THT_StatusTypeDef
 */
THT_StatusTypeDef SIM_setLocalTimeAuto(SIM_HandleTypeDef *const me)
{
	if (me == NULL)
	{
		return THT_ERROR;
	}

	return (*me->simSetLocalTimeAuto)(me);
}

/**
 * @brief Set Local Time Manually
 *
 * @param me [in] A pointer to SIM_HandleTypeDef structure
 * @param pTime
 * @param size
 * @return THT_StatusTypeDef
 */
THT_StatusTypeDef SIM_setLocalTimeManual(SIM_HandleTypeDef *const me, const char *pTime, uint8_t size)
{
	if (me == NULL || pTime)
	{
		return THT_ERROR;
	}

	printf("Set local time manual:\n"
			"Time: %s\n\n", pTime);

	return (*me->simSetLocalTimeManual)(me, pTime, size);
}

/**
 * @brief Select Phonebook Storage
 *
 * @param me [in] A pointer to SIM_HandleTypeDef structure
 * @return THT_StatusTypeDef
 */
THT_StatusTypeDef SIM_selectPhonebookMem(SIM_HandleTypeDef *const me)
{
	if (me == NULL)
	{
		return THT_ERROR;
	}

	return (*me->simSelectPhonebookMem)(me);
}

/**
 * @brief Save New Contact
 *
 * @param me [in] A pointer to SIM_HandleTypeDef structure
 * @param order
 * @param pPhone
 * @param phoneSize
 * @return THT_StatusTypeDef
 */
THT_StatusTypeDef SIM_savePhonebook(SIM_HandleTypeDef *const me, uint8_t order, const char *pPhone, uint8_t phoneSize)
{
	if (me == NULL || pPhone == NULL)
	{
		return THT_ERROR;
	}

	printf("Save contact:\n"
			"Order: %u\n"
			"Phonenumber: %s\n\n", order, pPhone);

	return (*me->simSavePhonebook)(me, order, pPhone, phoneSize);
}

/**
 * @brief Read Contact
 *
 * @param me [in] A pointer to SIM_HandleTypeDef structure
 * @param order
 * @param pPhone
 * @param phoneSize
 * @return THT_StatusTypeDef
 */
THT_StatusTypeDef SIM_readPhonebook(SIM_HandleTypeDef *const me, uint8_t order, char pPhone[], uint8_t phoneSize)
{
	if (me == NULL || pPhone == NULL)
	{
		return THT_ERROR;
	}

	printf("Read contact:\n"
			"Order: %u\n\n", order);

	return (*me->simReadPhonebook)(me, order, pPhone, phoneSize);
}

/**
 * @brief Find a Contact
 *
 * @param me [in] A pointer to SIM_HandleTypeDef structure
 * @param pPhone
 * @return THT_StatusTypeDef
 */
THT_StatusTypeDef SIM_findPhonebook(SIM_HandleTypeDef *const me, const char *pPhone)
{
	if (me == NULL || pPhone == NULL)
	{
		return THT_ERROR;
	}

	printf("Find contact:\n"
			"Phone number: %s\n\n", pPhone);

	return (*me->simFindPhonebook)(me, pPhone);
}

/**
 * @brief Send HTTP Request
 *
 * @param me [in] A pointer to SIM_HandleTypeDef structure
 * @param pHTTP
 * @return THT_StatusTypeDef
 */
THT_StatusTypeDef SIM_sendHTTP(SIM_HandleTypeDef *const me, SIM_HTTPTypeDef *pHTTP)
{
	if (me == NULL || pHTTP == NULL)
	{
		return THT_ERROR;
	}

	printf("HTTP request information:\n"
			"URL: %s\n"
			"Content Type: %s\n"
			"Method: %u\n"
			"Data: %s\n\n", pHTTP->url, pHTTP->type, pHTTP->method, pHTTP->data);

	THT_StatusTypeDef res = (*me->simSendHTTP)(me, pHTTP);

	printf("HTTP response information \n"
			"ErrorCode: %d\n"
			"Length: %lu\n"
			"Response: %s\n\n", pHTTP->errorcode, pHTTP->datalen, pHTTP->response);

	if (THT_OK != res)
	{
		return THT_ERROR;
	}
	else
	{
		return THT_OK;
	}
}

/**
 * @brief Download File over HTTP
 *
 * @param me [in] A pointer to SIM_HandleTypeDef structure
 * @param pUrl
 * @param pFilename
 * @return THT_StatusTypeDef
 */
THT_StatusTypeDef SIM_getFileHTTP(SIM_HandleTypeDef *const me, const char *pUrl, const char *pFilename)
{
	if (me == NULL || pUrl == NULL || pFilename == NULL)
	{
		return THT_ERROR;
	}

	printf("HTTP download information:\n"
			"URL: %s\n"
			"Filename: %s\n\n", pUrl, pFilename);

	return (*me->simGetFileHTTP)(me, pUrl, pFilename);
}

#if TEST
#include <stdio.h>

/**
 * @brief
 *
 * @param me [in] A pointer to SIM_HandleTypeDef structure
 * @return THT_StatusTypeDef
 */
THT_StatusTypeDef SIM_test(SIM_HandleTypeDef *const me)
{
	if (me == NULL)
	{
		return THT_ERROR;
	}
	SIM_wakeup(me);
	SIM_DataTypeDef simData = {0};
	//	SIM_setLocalTimeAuto(me);
	SIM_getData(me, SIM_DR_IMEI | SIM_DR_RSSI | SIM_DR_NWTYPE | SIM_DR_SIMPHONE | SIM_DR_TIME, &simData);

	printf("==============\n"
		   "IMEI: %s\n"
		   "RSSI: %d\n"
		   "NWTYPE: %s\n"
		   "SIMPHONE: %s\n"
		   "TIME: %s\n\n",
		   simData.imei, simData.rssi, simData.network, simData.simPhone, simData.timezone);
	SIM_sleep(me);
	return THT_OK;
}
#endif
