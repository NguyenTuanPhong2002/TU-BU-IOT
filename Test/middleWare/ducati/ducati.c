/*
 * ducati.c
 *
 *  Created on: Oct 4, 2023
 *      Author: Trump2003
 */
#include "ducati.h"
#include "rs485.h"

extern RS485 *data;

uint16_t crcCalculation(uint8_t *rs485_data, int in_dx)
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
}

void Ducati_transmit(UART_HandleTypeDef *huart, Ducati_Query_t Query)
{
	uint8_t queryData[8] = {0};
	uint8_t ind = 0;
	queryData[ind++] = Query.slaveAddress;
	queryData[ind++] = Query.FunC;
	queryData[ind++] = Query.DataStartRegisterH;
	queryData[ind++] = Query.DataStartRegisterL;
	queryData[ind++] = Query.DataRegsH;
	queryData[ind++] = Query.DataRegsL;
	Query.crc = crcCalculation(queryData, ind );
	queryData[ind++] = (uint8_t)(Query.crc & 0xFF);
	queryData[ind++] = (uint8_t)(Query.crc >> 8);
	HAL_UART_Transmit(data->huart, (uint8_t *)queryData, ind, 1000);
	RS485_Receive(data);
}

void Ducati_Master_Receive(int length)
{
	char Master_ReceivedBuff[300];
	RS485_Receive(data);
	Master_ReceivedBuff[300] = data->RxBuffer[255];
	// 	for (int s = 0; s < length; s++)
	// 	{
	// 		int index_Adr = s;
	// 		int index_Func = (s + 1) % length;
	// 		// Master_ReceivedBuff[300] = data->RxBuffer[( index_Adr) % length];
	// 		int index_Reg = (s + 3) % length;
	// 		// Master_ReceivedBuff[300] = data->RxBuffer[( index_Adr) % length];
	// 		// if (data->RxBuffer[index_Adr] == 0x01 && data->RxBuffer[index_Func] == 0x03)
	// 		// {
	// 		// 	for (int v = 0; v < length; v++)
	// 		// 	{
	// 		// 		Master_ReceivedBuff[v] = data->RxBuffer[(v + index_Adr) % length];
	// 		// 	}
	// 		// }
	// 		// if (data->RxBuffer[index_Adr] == 0x01 && data->RxBuffer[index_Func] == 0x01)
	// 		// {
	// 		// 	for (int v = 0; v < length; v++)
	// 		// 	{
	// 		// 		Master_ReceivedBuff[v] = data->RxBuffer[(v + index_Adr) % length];
	// 		// 	}
	// 		// }
	// 	}
}
