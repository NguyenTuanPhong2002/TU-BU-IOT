/*
 * flash.c
 *
 *  Created on: Apr 19, 2022
 *      Author: This PC
 */

#include "flash.h"

/**
  * @brief Unlock the flash.
  * @retval None
  */
void flash_unlock()
{
	HAL_FLASH_Unlock();
}
/**
  * @brief Lock the flash.
  * @retval None
  */
void flash_lock()
{
	HAL_FLASH_Lock();
}

void flash_EraseSector(uint8_t SectorNum)
{
	HAL_FLASH_Unlock();
	FLASH_Erase_Sector(SectorNum, FLASH_VOLTAGE_RANGE_3);
	HAL_FLASH_Lock();
}

void flash_write_Int(uint32_t address, uint32_t iData)
{
	HAL_FLASH_Unlock();
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, iData);
	HAL_FLASH_Lock();
}
uint32_t flash_read_Int(uint32_t address)
{
		uint32_t*val = (uint32_t *)address;
		return *val;
//	return *(__IO uint16_t *)(address);
}
void flash_Write_Float(uint32_t address, float f)
{
	HAL_FLASH_Unlock();
	uint8_t data[4];
	*(float*)data = f;
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, *(uint32_t*)data);
	HAL_FLASH_Lock();
}
float flash_Read_Float(uint32_t address)
{
	uint32_t data = *(__IO uint32_t *)(address);
	return *(float *)(&data);
}
void flash_write_Array(uint32_t address, char* arrData, uint16_t length)
{
	HAL_FLASH_Unlock();
	uint32_t *pt = (uint32_t*) arrData;
	for(uint16_t i = 0; i < (length + 3) / 4; i++)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address+4*i, *pt);
		pt++;
	}
	HAL_FLASH_Lock();
}
//void flash_write_Array(uint32_t address, char* arrData, uint16_t length)
//{
//	HAL_FLASH_Unlock();
//	uint16_t *pt = (uint16_t*) arrData;
//	for(uint16_t i = 0; i < (length + 1) / 2; i++)
//	{
//		HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, address+2*i, *pt);
//		pt++;
//	}
//	HAL_FLASH_Lock();
//}
void flash_read_Array(uint32_t address, char* arrData, uint64_t length)
{
	uint16_t *pt = (uint16_t *)arrData;
	for(uint16_t i = 0; i < (length + 1) / 2; i++)
	{
		*pt = *(__IO uint16_t *)(address+2*i);
		pt++;
	}
}
//void flash_Write_Struct(uint32_t address, Struct_info_t data)
//{
//	flash_write_Array(address, (uint8_t *)&data, sizeof(data));
//}
//void flash_Read_Struct(uint32_t address, Struct_info_t *data)
//{
//	flash_read_Array(address, (uint8_t*)&data, sizeof(Struct_info_t));
//}
/*
 * Hàm này ghi vào flash 1 word mỗi lần
 * */
//void flash_writeArray(uint32_t address, char* arrData, uint16_t length)
//{
//	uint32_t *pt = (uint32_t*) arrData;
//	for(uint16_t i = 0; i < (length + 1) / 4; i++)
//	{
//		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address+4*i, *pt);
//		pt++;
//	}
//	HAL_FLASH_Lock();
//}

