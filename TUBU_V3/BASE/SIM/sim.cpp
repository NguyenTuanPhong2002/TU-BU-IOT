/*
 * sim.cpp
 *
 *  Created on: Oct 25, 2023
 *      Author: NTPhong
 * 		Email: ntphong011102@gmail.com
 */

#include "sim.h"

void *SIM::simSleep()
{
    //return;
}

void *SIM::simWakeup()
{
    //return;
}

APP_StatusTypeDef SIM::SIM_sendATCommand(const char *command, const char *expect, const char *unexpect, uint32_t timeout)
{
    return APP_ERROR;
}

APP_StatusTypeDef SIM::getSimData(uint32_t data, SIM_DataTypeDef *pData)
{
    if ((data & SIM_DR_RSSI) == SIM_DR_RSSI)
    {
        SIM_getRSSI();
    }
    if ((data & SIM_DR_NWTYPE) == SIM_DR_NWTYPE)
    {
        SIM_getNwtype(pData->network, sizeof(pData->network));
    }
    if ((data & SIM_DR_SIMPHONE) == SIM_DR_SIMPHONE)
    {
        SIM_getSimPhone(pData->simPhone, sizeof(pData->simPhone));
    }
    // if ((data & SIM_DR_IMEI) == SIM_DR_IMEI)
    // {
    //     (*me->simGetIMEI)(me, pData->imei, sizeof(pData->imei));
    // }
    // if ((data & SIM_DR_TIME) == SIM_DR_TIME)
    // {
    //     (*me->simGetLocalTime)(me, pData->timezone, sizeof(pData->timezone));
    // }
    // if ((data & SIM_DR_TEMP) == SIM_DR_TEMP)
    // {
    //     pData->temp = (*me->simGetTemperature)(me);
    // }
    return APP_OK;
}

int8_t SIM::SIM_getRSSI()
{
    return APP_ERROR;
}

uint8_t SIM::SIM_getNwtype(char pNwtype[], uint8_t size)
{
    return 0;
}

APP_StatusTypeDef SIM::SIM_getSimPhone(char pPhone[], uint8_t size)
{
    return APP_ERROR;
}

APP_StatusTypeDef SIM::SIM_getIMEI(char pIMEI[], uint8_t size){
    return APP_ERROR;
}

APP_StatusTypeDef SIM::SIM_callUSSD(const char *ussd, char response[], uint16_t size)
{
    return APP_ERROR;
}
