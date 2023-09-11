/*
 * fota.c
 *
 *  Created on: Apr 14, 2022
 *      Author: This PC
 */

#include "fota.h"

FirmwareInfo_t fw = {0};


void 	fota_gotoFw(uint32_t address)
{
	HAL_RCC_DeInit();					//Tắt hết ngoại vi xoá hết c�? ngắt

	HAL_DeInit();						//Xoá hết các Pending, đồng th�?i tắt System Tick

	SCB->SHCSR &= ~( SCB_SHCSR_USGFAULTENA_Msk |\
			SCB_SHCSR_BUSFAULTENA_Msk | \
			SCB_SHCSR_MEMFAULTENA_Msk ) ;

	__set_MSP(*((volatile uint32_t*) address));

	uint32_t jumpAddress = *((volatile uint32_t*) (address + 4));
	void (*reset_handler)(void) = (void*) jumpAddress;
	reset_handler();
}
