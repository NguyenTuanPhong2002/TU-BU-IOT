/*
 * ducati.c
 *
 *  Created on: Oct 4, 2023
 *      Author: Trump2003
 */
#include "ducati.h"

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
	//  crctemp1 = crc/256;
	//  crctemp2 = crc*256;
	//  crc= crctemp1 + crctemp2;
	//	crc1 =crc>>8;
	//	crc2=(uint8_t)crc;
}