/*
 * config.h
 *
 *  Created on: Thu 1, 2024
 *      Author: DVTrung
 *      
 */
#include "wlv_def.h"

#ifndef CONFIG_CONFIG_H_
#define CONFIG_CONFIG_H_

#define CFG_DEVICE_ID_SIZE 15U
#define CFG_MASTER_ID_SIZE 15U
#define LORA_RESPONSE_SIZE 256U
#define LORA_REQUEST_SIZE  256U

#define CFG_EEPROM_DEVICE_ID (0x00080060UL)
#define CFG_EEPROM_MASTER_ID (CFG_EEPROM_DEVICE_ID + CFG_DEVICE_ID_SIZE)

typedef struct
{
	char deviceID[CFG_DEVICE_ID_SIZE];
	char masterID[CFG_MASTER_ID_SIZE];
} CFG_HandleTypeDef;

THT_StatusTypeDef CFG_setDeviceID(CFG_HandleTypeDef *const me, const char *pID, uint8_t size);
THT_StatusTypeDef CFG_setDeviceMaster(CFG_HandleTypeDef *const me, const char *pID, uint8_t size);

char *CFG_getDeviceID(CFG_HandleTypeDef *const me);
char *CFG_getDeviceMaster(CFG_HandleTypeDef *const me);
THT_StatusTypeDef CFG_writeEEPROM(CFG_HandleTypeDef *me);

THT_StatusTypeDef CFG_getID(CFG_HandleTypeDef *me);
THT_StatusTypeDef CFG_getMaster(CFG_HandleTypeDef *me);
#endif /* CONFIG_CONFIG_H_ */
