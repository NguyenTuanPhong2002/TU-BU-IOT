/*
 * fota.h
 *
 *  Created on: Oct 5, 2023
 *      Author: NTPhong
 * 		Email: ntphong01112002@gmail.com
 */

#ifndef FOTA_FOTA_H_
#define FOTA_FOTA_H_

#include "sim.h"

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_def.h"
#include "stm32f4xx_hal_flash_ex.h"
#include "stm32f4xx_hal_crc.h"

#define URL_LENGTH 200

#define FOTA_BOOT_START_ADDR 0x08000000U /* The bootloader firmware's start address */

#define FOTA_EEPROM_CURRENT_ADDR (0)                              /* EEPROM address to store the current running address value */
#define FOTA_EEPROM_CURRENT_MAJOR (FOTA_EEPROM_CURRENT_ADDR + 4)  /* EEPROM address to store the major of firmware version value */
#define FOTA_EEPROM_CURRENT_MINOR (FOTA_EEPROM_CURRENT_MAJOR + 2) /* EEPROM address to store the minor of firmware version value */
#define FOTA_EEPROM_CURRENT_PATCH (FOTA_EEPROM_CURRENT_MINOR + 2) /* EEPROM address to store the patch of firmware version value */

#define FOTA_BOOT_START_ADDR 0x08000000U                                          /* The bootloader firmware's start address */
#define FOTA_BOOT_MAX_SIZE (32U)                                                  /* The max size of bootloader firmware in KBytes */
#define FOTA_BOOT_END_ADDR ((FOTA_BOOT_START_ADDR) + ((FOTA_BOOT_MAX_SIZE)*1024)) /* The bootloader firmware's end address  */

#define FOTA_FIRMWARE_HEADER_NAME "controller_" /* The header of the firmware - Must be followed */

#define FOTA_APP_1_START_ADDR FOTA_BOOT_END_ADDR                                     /* The application1 firmware's start address */
#define FOTA_APP_1_MAX_SIZE (240U)                                                   /* The max size of application1 firwamre in KBytes */
#define FOTA_APP_1_END_ADDR ((FOTA_APP_1_START_ADDR) + ((FOTA_APP_1_MAX_SIZE)*1024)) /* The application1 firmware's end address */

#define FOTA_APP_2_START_ADDR FOTA_APP_1_END_ADDR                                    /* The application2 firmware's start address */
#define FOTA_APP_2_MAX_SIZE (FOTA_APP_1_MAX_SIZE)                                    /* The max size of application2 firwamre in KBytes */
#define FOTA_APP_2_END_ADDR ((FOTA_APP_2_START_ADDR) + ((FOTA_APP_2_MAX_SIZE)*1024)) /* The application2 firmware's end address */

#define FOTA_NUMBER_FLASH_PAGES ((FOTA_APP_1_MAX_SIZE * 1024) / (256U)) /* The total flash pages of the application firwmare */

#define FOTA_SR_STT_Pos (0U)

#define FOTA_FLAG_UPEN (0x1UL << FOTA_SR_STT_Pos)  /* Flag update firmware enable */ 
#define FOTA_FLAG_UPOK (0x2UL << FOTA_SR_STT_Pos)  /* Flag update firmware status */
#define FOTA_FLAG_GUER (0x3UL << FOTA_SR_STT_Pos)  /* Flag update ??? */
#define FOTA_FLAG_VERER (0x4UL << FOTA_SR_STT_Pos) /* Flag old version or version error */
#define FOTA_FLAG_NWSIG (0x5UL << FOTA_SR_STT_Pos) /* Flag bad network signal */
#define FOTA_FLAG_DOWER (0x6UL << FOTA_SR_STT_Pos) /* Flag download firmware error */
#define FOTA_FLAG_FFER (0x7UL << FOTA_SR_STT_Pos)  /* Flag file system error */
#define FOTA_FLAG_WRIER (0x8UL << FOTA_SR_STT_Pos) /* Flag write firwmare error */
#define FOTA_FLAG_CRCER (0x9UL << FOTA_SR_STT_Pos) /* Flag check firmware CRC error  */
#define FOTA_FLAG_RST (0xAUL << FOTA_SR_STT_Pos)   /* Flag reset the device */

typedef enum
{
    FOTA_OK,
    FOTA_ERROR,
    FOTA_BUSY,
    FOTA_TIMEOUT
} FOTA_StatusTypeDef;

class FOTA : public Sim
{
private:
    /* data */
    char firmwareURL[URL_LENGTH];
    uint16_t curVersion[3]; /* major - minor - patch */
    uint16_t newVersion[3]; /* major - minor - patch */
    char firmwareName[100];
    uint32_t curAddress;
    uint32_t newAddress;
    uint32_t firmwareSize;
    uint32_t CRC32;
    uint8_t Flag;

public:
    FOTA_StatusTypeDef FOTA_process();
    FOTA_StatusTypeDef FOTA_setDownloadURL(const char *pURL, uint16_t URLSize);
    FOTA_StatusTypeDef FOTA_compareVersion();
    FOTA_StatusTypeDef FOTA_parseFirmwareInfo();
    FOTA_StatusTypeDef FOTA_downloadFirmware();
    FOTA_StatusTypeDef FOTA_writeFirmware();
    uint32_t FOTA_getCurrentAddress();
    FOTA_StatusTypeDef FOTA_getCurrentVersion(uint16_t pVersion[], size_t size);
    FOTA_StatusTypeDef FOTA_readEPPROMs();
    FOTA_StatusTypeDef FOTA_writeEPPROMs();
    void FOTA_gotoFirmware();
    uint8_t FOTA_SET_FLAG(uint8_t setFlag);
    uint8_t FOTA_GET_FLAG(uint8_t setFlag);
    uint8_t FOTA_CLR_FLAG(uint8_t setFlag);
};

#endif /* FOTA_FOTA_H_ */
