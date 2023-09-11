/*
 * flash.h
 *
 *  Created on: Apr 14, 2022
 *      Author: HOANG VAN BINH
 *      Email: binhhv.23.1.99@gmail.com
 */

#ifndef INC_FLASH_H_
#define INC_FLASH_H_

#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "stm32f4xx_hal.h"

void flash_lock();
void flash_unlock();
//void flash_erase(uint32_t pageAddress);
//void flash_writeInt(uint32_t address, int32_t iData);
//void flash_writeArray(uint32_t address, char* arrData, uint16_t length);
//uint32_t flash_readInt(uint32_t address);
//void flash_readArray(uint32_t address, char* arrData, uint16_t length);


#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "stm32f4xx_hal.h"

typedef struct{
	uint8_t no;
	uint8_t ssid[30];
	uint8_t pass[30];
}Struct_info_t;
void flash_EraseSector(uint8_t SectorNum);
/*
 * Hàm void flash_EraseSector(uint8_t SectorNum);
 * Mô tả: Chọn Sector muốn xóa
 * [in] SectorNum: Chọn sector muốn xóa.
 * Trong STM32F407 có 12 Sector với thứ tự đánh số từ 0 đến 11. Reference manual trang 75.
 */

void flash_write_Int(uint32_t address, uint32_t iData);
/*
 * Hàm void flash_write_Int(uint32_t address, uint32_t iData);
 * Mô tả: Ghi vào Flash 1 số kiểu Int với mỗi lần ghi là 1 halfword
 * [in] address: địa chỉ flash muốn ghi vào
 * [in] iData: số muốn ghi vào
 */

void flash_Write_Float(uint32_t address, float f);
/*
 * void flash_Write_Float(uint32_t address, float f);
 * Mô tả: Ghi vào Flash 1 số kiểu Float với mỗi lần ghi là 1 Word
 * [in] address: địa chỉ flash muốn ghi vào
 * [in] iData: số muốn ghi vào
 */

void flash_write_Array(uint32_t address, char* arrData, uint16_t length);
/*
 * void flash_write_Array(uint32_t address, char* arrData, uint16_t length);
 * Mô tả: Ghi vào Flash 1 Array
 * [in] address: địa chỉ flash muốn ghi vào
 * [in] arrData: array muốn ghi vào
 * [in] Length: kích thước của Array muốn ghi vào
 */

uint32_t flash_read_Int(uint32_t address);
/*
 * uint32_t flash_read_Int(uint32_t address);
 * Mô tả: Đọc giá trị Int từ địa chỉ của Flash
 * [in] address: địa chỉ flash muốn đọc
 * [out] Gán biến cho hàm này để lấy ra giá trị từ Flash
 */

float flash_Read_Float(uint32_t address);
/*
 * float flash_Read_Float(uint32_t address);
 * Mô tả: Đọc giá trị Float từ địa chỉ của Flash
 * [in] address: Địa chỉ flash muốn đọc
 * [out] Gán biến cho hàm này để lấy ra giá trị từ Flash
 */

void flash_read_Array(uint32_t address, char* arrData, uint64_t length);
/*
 * float flash_Read_Float(uint32_t address);
 * Mô tả: Đọc giá trị Float từ địa chỉ của Flash
 * [in] address: Địa chỉ flash muốn đọc
 * [out] Gán biến cho hàm này để lấy ra giá trị từ Flash
 */

//void flash_Write_Struct(uint32_t address, Struct_info_t data);
//
//void flash_Read_Struct(uint32_t address, Struct_info_t *data);

#endif /* INC_FLASH_H_ */
