/*
 * config.h
 *
 *  Created on: Thu 1, 2024
 *      Author: DVTrung
 *
 */

#include "configID.h"
#include "stdio.h"
#include "stm32f4xx_hal_flash.h"

THT_StatusTypeDef CFG_setDeviceID(CFG_HandleTypeDef *const me, const char *pID,
                                  uint8_t size)
{
    if (me == NULL)
    {
        return THT_ERROR;
    }
    if (me == NULL || pID == NULL || size >= CFG_DEVICE_ID_SIZE)
    {
        return THT_ERROR;
    }
    memset(me->deviceID, '\0', CFG_DEVICE_ID_SIZE);

    memcpy(me->deviceID, pID, size);
    me->deviceID[size] = '\0';

    printf("Device ID: %s\n\n", me->deviceID);
    return THT_OK;
}

THT_StatusTypeDef CFG_setDeviceMaster(CFG_HandleTypeDef *const me, const char *pID,
                                      uint8_t size)
{
    if (me == NULL)
    {
        return THT_ERROR;
    }
    if (me == NULL || pID == NULL || size >= CFG_MASTER_ID_SIZE)
    {
        return THT_ERROR;
    }
    memset(me->masterID, '\0', CFG_MASTER_ID_SIZE);

    memcpy(me->masterID, pID, size);
    me->masterID[size] = '\0';

    printf("Device ID: %s\n\n", me->masterID);
    return THT_OK;
}

char *CFG_getDeviceID(CFG_HandleTypeDef *const me)
{
    if (me == NULL)
    {
        return '\0';
    }

    return me->deviceID;
}

char *CFG_getDeviceMaster(CFG_HandleTypeDef *const me)
{
    if (me == NULL)
    {
        return '\0';
    }

    return me->masterID;
}

THT_StatusTypeDef CFG_writeEEPROM(CFG_HandleTypeDef *me)
{

    if (me == NULL)
    {
        return THT_ERROR;
    }

    char deviceIDStoredInDataEEPROM[CFG_DEVICE_ID_SIZE] = {0};
    char deviceMasterStoredInDataEEPROM[CFG_DEVICE_ID_SIZE] = {0};

    for (size_t i = 0; i < CFG_DEVICE_ID_SIZE - 1; i++)
    {
        deviceIDStoredInDataEEPROM[i] =
            *(volatile uint8_t *)(CFG_EEPROM_DEVICE_ID + i);
    }

    for (size_t i = 0; i < CFG_MASTER_ID_SIZE - 1; i++)
    {
        deviceMasterStoredInDataEEPROM[i] =
            *(volatile uint8_t *)(CFG_EEPROM_MASTER_ID + i);
    }

    HAL_FLASHEx_DATAEEPROM_Unlock();
    HAL_FLASHEx_DATAEEPROM_EnableFixedTimeProgram();

    if (deviceIDStoredInDataEEPROM != me->deviceID)
    {

        for (size_t i = 0; i < CFG_DEVICE_ID_SIZE - 1; i++)
        {

            // HAL_FLASHEx_DATAEEPROM_Erase(CFG_EEPROM_DEVICE_ID + i);
            HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAM_BYTE,
                                           CFG_EEPROM_DEVICE_ID + i, (uint32_t) * (me->deviceID + i));
        }
    }

    if (deviceMasterStoredInDataEEPROM != me->masterID)
    {

        for (size_t i = 0; i < CFG_MASTER_ID_SIZE - 1; i++)
        {
            // HAL_FLASHEx_DATAEEPROM_Erase(CFG_EEPROM_DEVICE_ID + i);
            HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAM_BYTE,
                                           CFG_EEPROM_MASTER_ID + i, (uint32_t) * (me->masterID + i));
        }
    }

    HAL_FLASHEx_DATAEEPROM_DisableFixedTimeProgram();
    HAL_FLASHEx_DATAEEPROM_Lock();

    //---------------------------------------------------------------------------------------------------------------------
    for (size_t i = 0; i < CFG_DEVICE_ID_SIZE - 1; i++)
    {
        deviceIDStoredInDataEEPROM[i] =
            *(volatile uint8_t *)(CFG_EEPROM_DEVICE_ID + i);
    }

    for (size_t i = 0; i < CFG_MASTER_ID_SIZE - 1; i++)
    {
        deviceMasterStoredInDataEEPROM[i] =
            *(volatile uint8_t *)(CFG_EEPROM_MASTER_ID + i);
    }

    return THT_OK;
}

THT_StatusTypeDef CFG_getID(CFG_HandleTypeDef *me)
{
    char IDStoredInDataEEPROM[CFG_DEVICE_ID_SIZE] = {0};
    for (size_t i = 0; i < CFG_DEVICE_ID_SIZE - 1; i++)
    {
        IDStoredInDataEEPROM[i] =
            *(volatile uint8_t *)(CFG_EEPROM_DEVICE_ID + i);
    }

    CFG_setDeviceID(me, IDStoredInDataEEPROM, strlen(IDStoredInDataEEPROM));
    return THT_OK;
}

THT_StatusTypeDef CFG_getMaster(CFG_HandleTypeDef *me)
{
    char MasterStoredInDataEEPROM[CFG_MASTER_ID_SIZE] = {0};
    for (size_t i = 0; i < CFG_MASTER_ID_SIZE - 1; i++)
    {
        MasterStoredInDataEEPROM[i] =
            *(volatile uint8_t *)(CFG_EEPROM_MASTER_ID + i);
    }

    CFG_setDeviceMaster(me, MasterStoredInDataEEPROM, strlen(MasterStoredInDataEEPROM));
    return THT_OK;
}