/*
 * rs485.cpp
 *
 *  Created on: Oct 7, 2023
 *      Author: NTPhong
 * 		Email: ntphong011102@gmail.com
 */
#include "rs485.h"
#include "string.h"

void rs485::RS485_enablePowerOn()
{
    HAL_GPIO_WritePin(this->RS485_Power_Port, this->RS485_Power_Pin, GPIO_PIN_SET);
}

void rs485::RS485_enablePowerOff()
{
    HAL_GPIO_WritePin(this->RS485_Power_Port, this->RS485_Power_Pin, GPIO_PIN_RESET);
}

void rs485::RS485_enableTransmitMode()
{
    HAL_GPIO_WritePin(this->RS485_RE_Port, this->RS485_RE_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(this->RS485_DE_Port, this->RS485_DE_Pin, GPIO_PIN_RESET);
}

void rs485::RS485_enableReceiveMode()
{
    HAL_GPIO_WritePin(this->RS485_RE_Port, this->RS485_RE_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(this->RS485_DE_Port, this->RS485_DE_Pin, GPIO_PIN_RESET);
}

RS485_StatusTypeDef rs485::RS485_Transmit()
{
    RS485_enablePowerOn();
    RS485_enableTransmitMode();
    HAL_UART_Transmit(this->huart, (uint8_t *)(this->TxBuffer), strlen((char *)(this->TxBuffer)), 1000);
    RS485_enablePowerOff();
    return RS485_OK;
}

RS485_StatusTypeDef rs485::RS485_Receive()
{
    HAL_UART_Receive(this->huart, (uint8_t *)(this->RxBuffer), 255, 1000);
}

char *rs485::GetTxBuffer()
{
    return (char *)TxBuffer;
}
char *rs485::GetRxBuffer()
{
    return (char *)RxBuffer;
}

uint16_t rs485::crcCalculation(uint8_t *rs485_data, int in_dx)
{
    int i, j;
    uint16_t crc = 0xFFFF;
    uint16_t crctemp;
    for (i = 0; i < in_dx; i++)
    {
        crc = rs485_data[i] ^ crc;
        for (j = 1; j < 9; j++)
        {
            crctemp = crc / 2;
            if (crc - crctemp * 2 == 1)
            {
                crc = crctemp ^ 0xA001;
            }
            else
            {
                crc = crctemp;
            }
        }
    }
    return crc;
    //  crctemp1 = crc/256;
    //  crctemp2 = crc*256;
    //  crc= crctemp1 + crctemp2;
    //	crc1 =crc>>8;
    //	crc2=(uint8_t)crc;
}

void rs485::CONTACTOR_transmit(UART_HandleTypeDef *huart, RS485Query_t p_RS485Query)
{
}

RS485Data_t rs485::RS485_analysis(RS485Query_t RS485_SentStruct, uint8_t *p_RS485Data, uint8_t length)
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
                if (crc == crcCalculation((uint8_t *)p_RS485Data, 3 + lengthData))
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
                if (crc == crcCalculation((uint8_t *)p_RS485Data, 3 + lengthData))
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
                if (crc == crcCalculation((uint8_t *)p_RS485Data, 3 + lengthData))
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
                if (crc == crcCalculation((uint8_t *)p_RS485Data, 3 + lengthData))
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
                if (crc == crcCalculation((uint8_t *)p_RS485Data, 3 + lengthData))
                {
                    rs485.CoilStatus[0] = p_RS485Data[2];
                }
                break;
            case 0x0B:
                break;
            case 0x0C:
                break;
            case 0x0F:
                break;
            case 0x10:
                break;
            case 0x11:
                break;
            case 0x14:
                break;
            case 0x15:
                break;
            case 0x16:
                break;
            case 0x17:
                break;
            case 0x18:
                break;
            }
        }
    }
    return rs485;
}

void rs485::RS485_ForceSingleCoil(UART_HandleTypeDef *huart, uint8_t SlaveAddr, uint16_t Coil, uint8_t State)
{
    uint8_t queryData[10];
    uint8_t ind = 0;
    queryData[ind++] = SlaveAddr;
    queryData[ind++] = Force_SingleCoil;
    queryData[ind++] = (uint8_t)(Coil >> 8);
    queryData[ind++] = (uint8_t)(Coil & 0xFF);
    if (State == 1)
        queryData[ind++] = 0xFF;
    else
        queryData[ind++] = 0x00;
    queryData[ind++] = 0x00;
    //RS485_EnablePin(TRANSMIT);
    HAL_UART_Transmit(huart, (uint8_t *)queryData, ind, 1000);
    //RS485_EnablePin(RECEIVE);
}

void rs485::RS485_Master_Receive(char Master_ReceivedBuff[], int length)
{
	HAL_UART_Receive(this->huart, (uint8_t *)this->RxBuffer, length, 1000);
	for (int s = 0; s < length; s++)
	{
		int index_Adr = s;
		int index_Func = (s + 1) % length;
		// int index_Reg = (s + 3) % length;
		if (this->RxBuffer[index_Adr] == 0x01 && this->RxBuffer[index_Func] == 0x03)
		{
			for (int v = 0; v < length; v++)
			{
				Master_ReceivedBuff[v] = this->RxBuffer[(v + index_Adr) % length];
			}
		}
		if (this->RxBuffer[index_Adr] == 0x01 && this->RxBuffer[index_Func] == 0x01)
		{
			for (int v = 0; v < length; v++)
			{
				Master_ReceivedBuff[v] = this->RxBuffer[(v + index_Adr) % length];
			}
		}
	}
}