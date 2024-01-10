/*
 * http.c
 *
 *  Created on: Mar 24, 2023
 *      Author: Hoang Van Binh (gmail: binhhv.23.1.99@gmail.com)
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "midlib.h"

extern SIM_HandleTypeDef *simPtr;
//extern SDCARD_HandleTypeDef *sdcardPtr;


/**
 * @brief HTTP Process
 *
 */
void HTTP_process(void)
{
	SIM_HTTPTypeDef http = { 0 };

}

/**
 * @brief HTTP Initialize
 *
 * @param me [in] A pointer to SIM_HTTPTypeDef structure
 * @param pURL [in] A pointer to HTTP URL
 * @param URLSize [in]
 * @param URIType [out] URI
 * @return THT_StatusTypeDef
 */
THT_StatusTypeDef HTTP_init(SIM_HTTPTypeDef *const me, const char *pURL,
		uint8_t URLSize, uint8_t URIType)
{
	if (me == NULL || pURL == NULL)
	{
		return THT_ERROR;
	}
	(void) URIType;

	memset(me->url, '\0', sizeof(me->url));
	memset(me->type, '\0', sizeof(me->type));
	snprintf(me->url, sizeof(me->url), "%s", pURL);
	memcpy(me->type, HTTP_CONTENT_TYPE, strlen(HTTP_CONTENT_TYPE));
	me->method = SIM_HTTP_POST;

	return THT_OK;
}


THT_StatusTypeDef HTTP_sendCurrentData(SIM_HTTPTypeDef *const me)
{
	if (me == NULL)
	{
		return THT_ERROR;
	}
	printf("%s\n", __FUNCTION__);
	/* Read level sensor data in value array string */
	char valuestring[60] = { 0 };



	/* Get sim data and enable send HTTP immediately */
	SIM_DataTypeDef simData = { 0 };

	SIM_getData(simPtr, SIM_DR_RSSI | SIM_DR_NWTYPE | SIM_DR_IMEI, &simData);

	/* Get time period string */
	const char *pURL ;
	HTTP_init(me, pURL, strlen(pURL), HTTP_URL_URI_RESULT);

	/*-------------Send HTTP--------------*/
	memset(me->data, '\0', SIM_HTTP_DATA_SIZE);
	snprintf(me->data, SIM_HTTP_DATA_SIZE, "{\"%s\":{"
			"\"%s\":\"%s\","
			"\"%s\":\"%s\","
			"\"%s\":\"%s\","
			"\"%s\":\"%s\","
			"\"%s\":[%s],"
			"\"%s\":%d,"
			"\"%s\":%d,"
			"\"%s\":%d,"
			"\"%s\":\"%s\""
			"}}\n",

	HTTP_JSON_KEY_SIGS, simData.rssi,
	HTTP_JSON_KEY_NWTYPE, simData.network);

	//SDCARD_logData(sdcardPtr, sdcardPtr->currentFile, me->data,
			//strlen(me->data));

	/* Hạn chế gửi fail thì cho gửi thêm 1 lần nữa */
	if (THT_OK != SIM_sendHTTP(simPtr, me))
	{
		if (strlen(me->data) > 160)    //Dữ liệu dài hơn 160 ký tự mới lưu lại, để phòng dữ liệu sai
		{
			//SDCARD_logData(sdcardPtr, SDCARD_LOST_FILE, me->data,
					//strlen(me->data));
		}

		static uint16_t sendFailedPacketCounter = 0;

		sendFailedPacketCounter++;

		char reply[SIM_SMS_SIZE] = { 0 };

		snprintf(reply, sizeof(reply), "%s:%s:%s,%s:%s,%s:%s,%s:%s");

		/* Send message to telegram */
		if (sendFailedPacketCounter >= 2u)
		{
			printf("Reset module SIM: send packet fail!\n");
			SIM_init(simPtr);
			SIM_setFunction(simPtr, SIM_MODE_FULL);
			SIM_setLocalTimeAuto(simPtr);
			memset(reply, '\0', SIM_SMS_SIZE);
		}
	}


	return THT_OK;
}


/**
 * @brief Get configuraion
 *
 * @param me [in] A pointer to SIM_HTTPTypeDef structure
 * @return THT_StatusTypeDef
 */
THT_StatusTypeDef HTTP_sendGetConfig(SIM_HTTPTypeDef *const me)
{
	if (me == NULL)
	{
		return THT_ERROR;
	}


	return THT_ERROR;
}

/**
 * @brief Get Firmware URL to download firmawre
 *
 * @param me [in] A pointer to SIM_HTTPTypeDef structure
 * @return THT_StatusTypeDef
 */
THT_StatusTypeDef HTTP_sendGetFirmwareURL(SIM_HTTPTypeDef *const me)
{
	if (me == NULL)
	{
		return THT_ERROR;
	}
#if 0 //use http server Watec
	const char *pURL = CFG_getURL(cfgPtr);
	HTTP_init(me, pURL, strlen(pURL), HTTP_URL_URI_CMDS);
	SIM_DataTypeDef simData = {0};
	SIM_getData(simPtr, SIM_DR_IMEI | SIM_DR_TIME, &simData);
	memset(me->data, '\0', SIM_HTTP_DATA_SIZE);
	snprintf(me->data, SIM_HTTP_DATA_SIZE,
			 "{\"%s\":{\"%s\":\"%s\",\"%s\":\"%s\",\"%s\":\"%s\"}}",
			 HTTP_JSON_KEY_HEADER, HTTP_JSON_KEY_CMD, HTTP_JSON_VAL_GETFW,
			 HTTP_JSON_KEY_ID, CFG_getDeviceID(cfgPtr), HTTP_JSON_KEY_IMEI,
			 simData.imei);
#else //use http server firebase (used to test)
	me->method = SIM_HTTP_GET;
	memcpy(me->type, HTTP_CONTENT_TYPE, strlen(HTTP_CONTENT_TYPE));
	char url[] =
			"https://plucky-day-329409-default-rtdb.firebaseio.com/DOMUCNUOC/GETFW/-NJOfAXmrWHHuA9ATGxL.json";
	memcpy(me->url, url, strlen(url));
#endif
	if (THT_OK == SIM_sendHTTP(simPtr, me))
	{
		HTTP_checkResponse(me);
		return THT_OK;
	}

	return THT_ERROR;
}

/**
 * @brief Download file from HTTP server
 *
 * @param me [in] A pointer to SIM_HTTPTypeDef structure
 * @param pURL
 * @param pFilename
 * @return THT_StatusTypeDef
 */
THT_StatusTypeDef HTTP_downloadFile(SIM_HTTPTypeDef *const me, const char *pURL,
		const char *pFilename)
{
	if (me == NULL || pFilename == NULL)
	{
		return THT_ERROR;
	}

	return SIM_getFileHTTP(simPtr, pURL, pFilename);
}

