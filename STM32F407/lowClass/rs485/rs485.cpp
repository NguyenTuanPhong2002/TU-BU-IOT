/*
 * rs485.cpp
 *
 *  Created on: Sep 11, 2023
 *      Author: TRUNG
 */
#include "RS485.h"
#include "string.h"

static void RS485_enablePowerOn(RS485 *me)
{
    if (me == NULL)
    {
        return;
    }
    HAL_GPIO_WritePin(me->RS485_Power_Port, me->RS485_Power_Pin, GPIO_PIN_SET);
}

static void RS485_enablePowerOff(RS485 *me)
{
    if (me == NULL)
    {
        return;
    }
    HAL_GPIO_WritePin(me->RS485_Power_Port, me->RS485_Power_Pin, GPIO_PIN_RESET);
}

/* To enable the transmit mode,
You need to set the RE pin to logic level 0 and the DE pin to logic level 0.*/
static void RS485_enableTransmitMode(RS485 *me)
{
    if (me == NULL)
    {
        return;
    }
    HAL_GPIO_WritePin(me->RS485_RE_Port, me->RS485_RE_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(me->RS485_DE_Port, me->RS485_DE_Pin, GPIO_PIN_RESET);
}

/* To enable the receive mode,
You need to set the RE pin to logic level 1 and the DE pin to logic level 0.*/
static void RS485_enableReceiveMode(RS485 *me)
{
    if (me == NULL)
    {
        return;
    }
    HAL_GPIO_WritePin(me->RS485_RE_Port, me->RS485_RE_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(me->RS485_DE_Port, me->RS485_DE_Pin, GPIO_PIN_RESET);
}

void RS485_Transmit(RS485 *me)
{
    if (me == NULL)
    {
        return;
    }
    RS485_enablePowerOn(me);
    RS485_enableTransmitMode(me);
    uint8_t Size = strlen((char *)(me->TxBuffer));
    HAL_UART_Transmit(me->huart, (uint8_t *)(me->TxBuffer), Size, 1000);
    RS485_enablePowerOff(me);
}

void RS485_Receive(RS485 *me)
{
    if (me == NULL)
    {
        return;
    }
    RS485_enablePowerOn(me);
    RS485_enableReceiveMode(me);
    uint8_t Size = 255;
    HAL_UART_Receive(me->huart, (uint8_t *)(me->RxBuffer), Size, 1000);
}

