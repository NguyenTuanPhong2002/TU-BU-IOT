/*
 * ducatir8.cpp
 *
 *  Created on: Oct 27, 2023
 *      Author: NTPhong
 * 		Email: ntphong011102@gmail.com
 */

#include "ducatir8.h"

extern CRC_HandleTypeDef hcrc;

DUCATI_R8::DUCATI_R8(DUCATI_R8ptr *ducati)
{
    this->ducatiR8->DE_PIN = ducati->DE_PIN;
    this->ducatiR8->DE_PORT = ducati->DE_PORT;
    this->ducatiR8->hdma = ducati->hdma;
    this->ducatiR8->huart = ducati->huart;
    this->ducatiR8->PWR_PIN = ducati->PWR_PIN;
    this->ducatiR8->PWR_PORT = ducati->PWR_PORT;
    this->ducatiR8->RE_PIN = ducati->RE_PIN;
    this->ducatiR8->RE_PORT = ducati->RE_PORT;
}

void DUCATI_R8::OnPower()
{
    HAL_GPIO_WritePin(this->ducatiR8->PWR_PORT, this->ducatiR8->PWR_PIN, GPIO_PIN_SET);
}

void DUCATI_R8::OffPower()
{
    HAL_GPIO_WritePin(this->ducatiR8->PWR_PORT, this->ducatiR8->PWR_PIN, GPIO_PIN_SET);
}

void DUCATI_R8::EnableTransmit()
{
    OnPower();
    HAL_GPIO_WritePin(this->ducatiR8->RE_PORT, this->ducatiR8->RE_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(this->ducatiR8->DE_PORT, this->ducatiR8->DE_PIN, GPIO_PIN_SET);
}

void DUCATI_R8::EnableReceiver()
{
    OnPower();
    HAL_GPIO_WritePin(this->ducatiR8->RE_PORT, this->ducatiR8->RE_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(this->ducatiR8->DE_PORT, this->ducatiR8->DE_PIN, GPIO_PIN_SET);
}

void DUCATI_R8::Disable()
{
    OffPower();
    HAL_GPIO_WritePin(this->ducatiR8->RE_PORT, this->ducatiR8->RE_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(this->ducatiR8->DE_PORT, this->ducatiR8->DE_PIN, GPIO_PIN_SET);
}

void DUCATI_R8::DUCATI_transmit(RS485Query_t p_RS485Query)
{
    uint8_t queryData[8];
    queryData[0] = p_RS485Query.slaveAddress;
    queryData[1] = p_RS485Query.mbFunction;
    if ((p_RS485Query.mbFunction == Read_CoilStatus) || (p_RS485Query.mbFunction == Read_InputStatus) || (p_RS485Query.mbFunction == Read_HoldingRegister) || (p_RS485Query.mbFunction == Read_InputRegister) || (p_RS485Query.mbFunction == Force_SingleCoil) || (p_RS485Query.mbFunction == Preset_SingleRegister))
    {
        queryData[2] = (uint8_t)(p_RS485Query.regAddress >> 8);
        queryData[3] = (uint8_t)(p_RS485Query.regAddress & 0xFF);
        queryData[4] = (uint8_t)(p_RS485Query.regCount >> 8);
        queryData[5] = (uint8_t)(p_RS485Query.regCount & 0xFF);
    }

    p_RS485Query.crc = (uint16_t)HAL_CRC_Calculate(&hcrc, queryData, 6);
    queryData[6] = (uint8_t)(p_RS485Query.crc & 0xFF);
    queryData[7] = (uint8_t)(p_RS485Query.crc >> 8);

    EnableTransmit();
    HAL_UART_Transmit(this->ducatiR8->huart, (uint8_t *)queryData, 8, 1000);
    Disable();
}

void DUCATI_R8::DUCATI_forceSingleCoil(uint8_t SlaveAddr, uint16_t Coil, uint8_t State)
{
    uint8_t queryData[10];
    queryData[0] = SlaveAddr;
    queryData[1] = Force_SingleCoil;
    queryData[2] = (uint8_t)(Coil >> 8);
    queryData[3] = (uint8_t)(Coil & 0xFF);
    if (State == 1)
        queryData[4] = 0xFF;
    else
        queryData[4] = 0x00;
    queryData[5] = 0x00;
    EnableTransmit();
    HAL_UART_Transmit(this->ducatiR8->huart, (uint8_t *)queryData, 6, 1000);
    EnableReceiver();
}

RS485Data_t DUCATI_R8::DUCATI_analysis(RS485Query_t RS485_SentStruct, uint8_t *p_RS485Data, uint8_t length)
{
    RS485Data_t rs485;
    if ((length > 3) && (p_RS485Data[0] == RS485_SentStruct.slaveAddress))
    {

        uint8_t lengthData = 0;
        uint16_t crc;
        rs485.Function = (RS485_Function)p_RS485Data[1];
        if (rs485.Function == RS485_SentStruct.mbFunction)
        {
            rs485.slaveAddress = p_RS485Data[0];
            switch (p_RS485Data[1])
            {
            case 0x01: // Read Coil Status
                lengthData = p_RS485Data[2];
                rs485.length = lengthData;
                crc = (uint16_t)p_RS485Data[3 + rs485.length] | ((uint16_t)p_RS485Data[4 + rs485.length] << 8);
                if (crc == HAL_CRC_Calculate(&hcrc, (uint8_t *)p_RS485Data, 3 + lengthData))
                {
                    for (uint8_t i = 0; i < rs485.length; i++)
                    {
                        rs485.CoilStatus[i] = p_RS485Data[3 + i];
                    }
                }
                break;
            case 0x02: // Read Input Status
                lengthData = p_RS485Data[2];
                rs485.length = lengthData;
                crc = (uint16_t)p_RS485Data[3 + rs485.length] | ((uint16_t)p_RS485Data[4 + rs485.length] << 8);
                if (crc == HAL_CRC_Calculate(&hcrc, (uint8_t *)p_RS485Data, 3 + lengthData))
                {
                    for (uint8_t i = 0; i < rs485.length; i++)
                    {
                        rs485.CoilStatus[i] = p_RS485Data[3 + i];
                    }
                }
                break;
            case 0x03: // Read Holding Register
                lengthData = p_RS485Data[2];
                rs485.length = lengthData / 2;
                crc = (uint16_t)p_RS485Data[3 + lengthData] | ((uint16_t)p_RS485Data[4 + lengthData] << 8);
                if (crc == HAL_CRC_Calculate(&hcrc, (uint8_t *)p_RS485Data, 3 + lengthData))
                {
                    for (uint8_t i = 0; i < rs485.length; i++)
                    {
                        rs485.dataReg[i] = (uint16_t)(p_RS485Data[3 + i * 2] << 8) | p_RS485Data[4 + i * 2]; // Vi tri bat dau du lieu se la 3
                    }
                }
                break;
            case 0x04: // Read Input Register
                lengthData = p_RS485Data[2];
                rs485.length = lengthData / 2;
                crc = (uint16_t)p_RS485Data[3 + lengthData] | ((uint16_t)p_RS485Data[4 + lengthData] << 8);
                if (crc == HAL_CRC_Calculate(&hcrc, (uint8_t *)p_RS485Data, 3 + lengthData))
                {
                    for (uint8_t i = 0; i < rs485.length; i++)
                    {
                        rs485.dataReg[i] = (uint16_t)(p_RS485Data[3 + i * 2] << 8) | p_RS485Data[4 + i * 2]; // Vi tri bat dau du lieu se la 3
                    }
                }
                break;
            case 0x05:
                break;
            case 0x06:
                break;
            case 0x07:
                crc = (uint16_t)p_RS485Data[3] | ((uint16_t)p_RS485Data[4] << 8);
                if (crc == HAL_CRC_Calculate(&hcrc, (uint8_t *)p_RS485Data, 3 + lengthData))
                {
                    rs485.CoilStatus[0] = p_RS485Data[2];
                }
                break;
            default:
                break;
            }
        }
    }
    return rs485;
}

void DUCATI_R8::DUCATI_Master_Receive(char Master_ReceivedBuff[], int length)
{
    HAL_UART_Receive(this->ducatiR8->huart, (uint8_t *)this->buffer, length, 1000);
    for (int s = 0; s < length; s++)
    {
        int index_Adr = s;
        int index_Func = (s + 1) % length;
        // int index_Reg = (s + 3) % length;
        if (this->buffer[index_Adr] == 0x01 && this->buffer[index_Func] == 0x03)
        {
            for (int v = 0; v < length; v++)
            {
                Master_ReceivedBuff[v] = this->buffer[(v + index_Adr) % length];
            }
        }
        if (this->buffer[index_Adr] == 0x01 && this->buffer[index_Func] == 0x01)
        {
            for (int v = 0; v < length; v++)
            {
                Master_ReceivedBuff[v] = this->buffer[(v + index_Adr) % length];
            }
        }
    }
}

float DUCATI_R8::DUCATI_getCosF()
{
    char buffer[9];
    char RTU_CosFi[] = {0x01, 0x03, 0x01, 0x61, 0x00, 0x02, 0x94, 0x29};
    HAL_UART_Transmit(this->ducatiR8->huart, (uint8_t *)RTU_CosFi, 8, 1000);
    DUCATI_Master_Receive(buffer, 9);
    float CosFi = (float)(((uint16_t)buffer[5] << 8) | ((uint16_t)buffer[6])) / 100;
    return CosFi;
}

float DUCATI_R8::DUCATI_getVoltage()
{
    char buffer[9];
    this->msg_Query.slaveAddress = 0x01;
    this->msg_Query.mbFunction = Read_HoldingRegister;
    this->msg_Query.regAddress = 0x03;
    this->msg_Query.regCount = 0x02;
    // this->msg_Query =
    //     {
    //         .slaveAddress = 0x01,
    //         .mbFunction = Read_HoldingRegister,
    //         .regAddress = 0x03,
    //         .regCount = 0x02};
    DUCATI_transmit(this->msg_Query);
    DUCATI_Master_Receive(buffer, 9);
    float Voltage = (float)(((uint16_t)buffer[5] << 8) | ((uint16_t)buffer[6]));
    return Voltage;
}

float DUCATI_R8::DUCATI_getCurrent()
{
    char buffer[9];
    this->msg_Query.slaveAddress = 0x01;
    this->msg_Query.mbFunction = Read_HoldingRegister;
    this->msg_Query.regAddress = 0x11;
    this->msg_Query.regCount = 0x02;
    // this->msg_Query =
    //     {
    //         .slaveAddress = 0x01,
    //         .mbFunction = Read_HoldingRegister,
    //         .regAddress = 0x11,
    //         .regCount = 0x02};
    DUCATI_transmit(this->msg_Query);
    DUCATI_Master_Receive(buffer, 9);
    float current = (float)(((uint16_t)buffer[5] << 8) | ((uint16_t)buffer[6])) / 100;
    return current;
}

float DUCATI_R8::DUCATI_getFrequency()
{
    char buffer[9];
    this->msg_Query.slaveAddress = 0x01;
    this->msg_Query.mbFunction = Read_HoldingRegister;
    this->msg_Query.regAddress = 0x01;
    this->msg_Query.regCount = 0x02;
    // this->msg_Query =
    //     {
    //         .slaveAddress = 0x01,
    //         .mbFunction = Read_HoldingRegister,
    //         .regAddress = 0x01,
    //         .regCount = 0x02};
    DUCATI_transmit(this->msg_Query);
    DUCATI_Master_Receive(buffer, 9);
    float Frequency = (float)(((uint16_t)buffer[5] << 8) | ((uint16_t)buffer[6])) / 10;
    return Frequency;
}

uint16_t DUCATI_R8::DUCATI_getAvQ()
{
    char buffer[9];
    this->msg_Query.slaveAddress = 0x01;
    this->msg_Query.mbFunction = Read_HoldingRegister;
    this->msg_Query.regAddress = 0x53;
    this->msg_Query.regCount = 0x02;
    // this->msg_Query =
    // {
    //     .slaveAddress = 0x01,
    //     .mbFunction = Read_HoldingRegister,
    //     .regAddress = 0x53,
    //     .regCount = 0x02};
    DUCATI_transmit(this->msg_Query);
    DUCATI_Master_Receive(buffer, 9);
    uint16_t AvQ = ((uint16_t)buffer[5] << 8) | ((uint16_t)buffer[6]);
    return AvQ;
}

uint16_t DUCATI_R8::DUCATI_getActivePower()
{
    char buffer[9];
    this->msg_Query.slaveAddress = 0x01;
    this->msg_Query.mbFunction = Read_HoldingRegister;
    this->msg_Query.regAddress = 0x21;
    this->msg_Query.regCount = 0x02;
    // this->msg_Query =
    // {
    //     .slaveAddress = 0x01,
    //     .mbFunction = Read_HoldingRegister,
    //     .regAddress = 0x21,
    //     .regCount = 0x02};
    DUCATI_transmit(this->msg_Query);
    DUCATI_Master_Receive(buffer, 9);
    uint16_t ActivePower = (((uint16_t)buffer[5] << 8) | ((uint16_t)buffer[6])) / 1000;
    return ActivePower;
}

uint16_t DUCATI_R8::DUCATI_getReactivePower()
{
    char buffer[9];
    this->msg_Query.slaveAddress = 0x01;
    this->msg_Query.mbFunction = Read_HoldingRegister;
    this->msg_Query.regAddress = 0x51;
    this->msg_Query.regCount = 0x02;
    // this->msg_Query =
    // {
    //     .slaveAddress = 0x01,
    //     .mbFunction = Read_HoldingRegister,
    //     .regAddress = 0x51,
    //     .regCount = 0x02};
    DUCATI_transmit(this->msg_Query);
    DUCATI_Master_Receive(buffer, 9);
    uint16_t ReactivePower = (((uint16_t)buffer[5] << 8) | ((uint16_t)buffer[6])) / 1000;
    return ReactivePower;
}

uint16_t DUCATI_R8::DUCATI_getAvP()
{
    char buffer[9];
    this->msg_Query.slaveAddress = 0x01;
    this->msg_Query.mbFunction = Read_HoldingRegister;
    this->msg_Query.regAddress = 0x51;
    this->msg_Query.regCount = 0x02;
    // this->msg_Query =
    // {
    //     .slaveAddress = 0x01,
    //     .mbFunction = Read_HoldingRegister,
    //     .regAddress = 0x51,
    //     .regCount = 0x02};
    DUCATI_transmit(this->msg_Query);
    DUCATI_Master_Receive(buffer, 9);
    uint16_t AvP = ((uint16_t)buffer[5] << 8) | ((uint16_t)buffer[6]);
    return AvP;
}