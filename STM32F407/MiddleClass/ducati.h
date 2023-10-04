/*
 * ducati.h
 *
 *  Created on: Oct 4, 2023
 *      Author: Trump2003
 */

/* 
	Calculate CRC of Modbus, with polynomial value = 0xA001
	-	rs485_data  : the array contains the query or response
	-	in_dx		: the length of array ( not include the CRC )
*/
uint16_t crcCalculation(uint8_t* rs485_data, int in_dx);
