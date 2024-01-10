/*
 * wlv_def.h
 *
 *  Created on: Mar 23, 2023
 *      Author: Hoang Van Binh (gmail: binhhv.23.1.99@gmail.com)
 */

#ifndef INC_WLV_DEF_H_
#define INC_WLV_DEF_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>
/**
 * @brief Water Level System Status
 */
typedef enum
{
	THT_OK,
	THT_ERROR,
	THT_BUSY,
	THT_TIMEOUT
} THT_StatusTypeDef;

#define WLV_EEPROM_BASE FLASH_EEPROM_BASE
#define WLV_FOTA_BASE (WLV_EEPROM_BASE)
#define WLV_CLOCK_BASE (WLV_FOTA_BASE + 0x80UL)
#define WLV_CONFIG_BASE (WLV_CLOCK_BASE + 0x80UL)
#define WLV_SENSOR_BASE (WLV_CONFIG_BASE + 0x80UL)
#define WLV_TELE_BASE (WLV_SENSOR_BASE + 0x80UL)

#define TEST 0
void THT_copyStringUntilToken(const char *src, char des[], uint16_t size, const char *token);
THT_StatusTypeDef THT_convertDoubleToHex(double d, uint8_t pBuffer[], size_t sizeMustSetIs8);
uint8_t THT_CRC8XOR(const uint8_t *array, size_t size);
THT_StatusTypeDef THT_CRC16(void);

#endif /* INC_WLV_DEF_H_ */
