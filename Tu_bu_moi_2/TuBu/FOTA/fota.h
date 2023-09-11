/*
 * fota.h
 *
 *  Created on: Apr 14, 2022
 *      Author: HOANG VAN BINH
 *      Email: binhhv.23.1.99@gmail.com
 */

#ifndef INC_FOTA_H_
#define INC_FOTA_H_


#include "stdio.h"
#include "flash.h"
#include "sim7x00.h"
#include "stm32f4xx_hal.h"


typedef enum {
	NO_UPDATE,
	UPDATE_SUCCESS,
	SIGNAL_WEAK,
	DOWNLOAD_FAIL,
	FILE_FAIL,
	WRITE_FAIL,
	CHECK_CRC_FAIL,
	OLD_VER,
	RESET_DEVICE,
} eUpdateState;
typedef struct {
	uint64_t major;
	uint64_t minor;
	uint64_t patch;
	//1.1.0
} Version_t;

typedef struct {
	uint64_t sizeOfFw;
	uint64_t curRunningFWAddr;
	uint64_t newPositionAddr;
	char	 nameOfNewFirmware[60];
} FirmwareInfo_t;


#define MAX_SIZE_OF_APP_FW				262144//(256 * 1024)
#define SIZE_OF_PAGE					256  //change here, we don't have page in STM32F407
#define DB_SIZEOF_PAGE					512

//#define	BOOT_START_ ADDRESS	0x08000000
#define APP1_START_ADDR 				0x08080000 //APP1 memory: 256kB, Sector 8,9
#define APP2_START_ADDR 				0x080C0000 //APP2 memory: 256kB, Sector 10,11

/* Define variable region
 * Start at Sector 5, 0x08020000
 */

#define FLAG_NEED_UPDATE_FW_ADDR 			0x08020000  //0: không update, 1: update, START AT SECTOR 5
#define CURRENT_RUNNING_FW_ADDR_ADDR 		0x08020004	//cách nhau 4
#define CURRENT_RUNNING_FW_VER_MAJOR_ADDR 	0x08020010
#define CURRENT_RUNNING_FW_VER_MINOR_ADDR 	0x08020014
#define CURRENT_RUNNING_FW_VER_PATCH_ADDR 	0x08020018
#define NEW_FW_ON_SERVER_MAJOR_ADDR 		0x0802001C
#define NEW_FW_ON_SERVER_MINOR_ADDR 		0x08020020
#define NEW_FW_ON_SERVER_PATCH_ADDR 		0x08020024

//START AT SECTOR 6
#define FOTA_STATE_ADDR 				0x08040000			// 4
#define FW_BIN_FILE_SIZE				0x08040004  		// 4 lưu kích thước của firmaware, bên bootloader sẽ đọc kích thước firmware để ghi flash//lưu trạng thái của cập nhật firmware
#define PHONE_NUMBER_REQUESTED_ADDR 	0x08040008 			// 12 lưu số điện thoại của người gửi tin nhắn đến để phản hồi
#define FILENAME_OF_FW_ADDR				0x08040014			// 60 lưu tên hiện tại của firmware
#define SIZE_LINK_FW_ON_SERVER			0x08040074			// 4: Kích thước đường dẫn chứa FW mới trên Firebase Storage
#define LINK_FW_ON_SERVER				0x08040078			// Lưu đường dẫn chứa Firmware mới.

// START AT SECTOR 7(128kB)
#define FL_DEVICEID_ADDR				0x08060000			//12
#define FL_RAINTHRES_ADDR				0x0806000C			//4
#define FL_BATTTHRES_ADDR				0x08060010			//4
#define FL_GATESEND_ADDR				0x08060014			//4
#define FL_GATEREAD_ADDR				0x08060018			//4
#define FL_SENDPERIOD_ADDR				0x0806001C			//4
#define FL_READPERIOD_ADDR				0x08060020			//4
#define FL_HTTPURL_ADDR					0x08060024			//4

//uint8_t fota_getNewFwVersion();
//uint8_t fota_compareVersion();
//uint8_t fota_downloadFw();
uint8_t fota_writeFw();
uint8_t fota_checkCrc(uint32_t address);
void 	fota_setAddress2Jump(uint32_t address);
void 	fota_gotoFw(uint32_t address);

#endif /* INC_FOTA_H_ */
