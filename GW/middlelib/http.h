/*
 * http.h
 *
 *  Created on: Mar 24, 2023
 *      Author: Hoang Van Binh (gmail: binhhv.23.1.99@gmail.com)
 */

#ifndef INC_HTTP_H_
#define INC_HTTP_H_

#include "sim.h"
//#include "sms.h"
#include "wlv_def.h"

/**
 * @brief HTTP JSON Key and Value
 *
 */
#define HTTP_JSON_KEY_HEADER ("DOMUCNUOCV2")
#define HTTP_JSON_KEY_CMD ("CMD")
#define HTTP_JSON_KEY_ID ("ID")
#define HTTP_JSON_KEY_IMEI ("IMEI")
#define HTTP_JSON_KEY_TIME ("TIME")
#define HTTP_JSON_KEY_BATT ("BATT")
#define HTTP_JSON_KEY_POWR ("POWR")
#define HTTP_JSON_KEY_SIGS ("SIGS")
#define HTTP_JSON_KEY_NWTYPE ("NWTYPE")
#define HTTP_JSON_KEY_VALS ("VALS")
#define HTTP_JSON_KEY_VAL ("VAL")
#define HTTP_JSON_VAL_RESULT ("RESULT")
#define HTTP_JSON_VAL_BATCAP ("BATCAP")
#define HTTP_JSON_VAL_GETCFG ("GETCFG")
#define HTTP_JSON_VAL_GETFW ("GETFW")
//#define HTTP_JSON_VAL_GETAUDIO ("GETAUDIO")
#define HTTP_JSON_VAL_WART ("WART")

/**
 * @brief HTTP API command
 *
 */
#define HTTP_API_RESULT 0U  /* Command used for JSON send result */
#define HTTP_API_WARNING 1U /* Command used for JSON sensor warning */
#define HTTP_API_BATCAP 2U  /* Command used for JSON battery warning */
#define HTTP_API_GETCFG 3U  /* Command used for JSON get configuration */
#define HTTP_API_GETFW 4U   /* Command used for JSON get firmware URL */

/**
 * @brief HTTP URL URI
 *
 */
#define HTTP_URL_URI_BASE 0u   /* The original URL */
#define HTTP_URL_URI_RESULT 1u /* The URL combines with URI /result */
#define HTTP_URL_URI_CMDS 2u   /* The URL combines with URI /cmds */

#define HTTP_CONTENT_TYPE "application/json" /* HTTP content type */

void HTTP_process(void);
THT_StatusTypeDef HTTP_makeAPIString(uint8_t type, char pAPI[], uint16_t size);
THT_StatusTypeDef HTTP_init(SIM_HTTPTypeDef *const me, const char *pURL, uint8_t URLSize, uint8_t URIType);
THT_StatusTypeDef HTTP_checkResponse(SIM_HTTPTypeDef *const me);
THT_StatusTypeDef HTTP_sendCurrentData(SIM_HTTPTypeDef *const me);
THT_StatusTypeDef HTTP_sendLastData(SIM_HTTPTypeDef *const me);
THT_StatusTypeDef HTTP_sendBatteryWarn(SIM_HTTPTypeDef *const me);
THT_StatusTypeDef HTTP_sendSensorWarn(SIM_HTTPTypeDef *const me);
THT_StatusTypeDef HTTP_sendGetConfig(SIM_HTTPTypeDef *const me);
THT_StatusTypeDef HTTP_sendGetFirmwareURL(SIM_HTTPTypeDef *const me);
THT_StatusTypeDef HTTP_downloadFile(SIM_HTTPTypeDef *const me, const char *pURL, const char *pFilename);

#endif /* INC_HTTP_H_ */
