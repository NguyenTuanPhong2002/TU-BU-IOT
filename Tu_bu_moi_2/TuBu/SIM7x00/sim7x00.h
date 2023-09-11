/*
 * sim7x00.h
 *
 *  Created on: Jan 30, 2023
 *      Author: tthieu01
 */

#ifndef SIM7600_SIM7X00_H_
#define SIM7600_SIM7X00_H_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "main.h"

#if defined(STM32F4)
#include <stm32f4xx_hal.h>
#elif defined(STM32L4)
#include <stm32l4xx_hal.h>
#elif defined(STM32F1)
#include <stm32f1xx_hal.h>
#elif defined(STM32L1)
#include <stm32l1xx_hal.h>
//#else
//#error "Please select first the target used in your application"
#endif

/*Thay đổi cổng UART giao tiếp với module SIM*/
extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart1;

#define SIM_UART		huart2
#define SIM_UART_DMA 	hdma_usart2_rx
#define USART			USART2
#define DEBUG_UART 		huart1
#define RX_LEN			560
#define FOTA_LEN		512



typedef enum
{
	SIM_OK = 0x00U, SIM_ERROR = 0x01U, SIM_BUSY = 0x02U, SIM_TIMEOUT = 0x03U
} SIM_StatusTypeDef;

typedef enum
{
	MINI_MODE, FULL_MODE, FLIGHT_MODE = 4, TEST_MODE, RESET_MODE, OFFLINE_MODE,
} SIM_FunctionTypeDef;

typedef enum
{
	SIM_OFF,
	SIM_ON,
	SIM_IDLE,
	SIM_UNDER_VOLTAGE,
	SIM_UPPER_VOLTAGE,
	SIM_SMS,
	SIM_CALL,
} SIM_Event;

typedef enum
{
	SMS_RESET = 1, SMS_DEBUG, SMS_UPDATE_FIRMWARE,
}SMS_Event;


typedef struct
{
	char PhoneNumber[12];
	char TimeReceive[18];
	char SmsContent[20];
} SMS_InfoTypeDef;

typedef struct
{
	char HttpResponse[200];    // Chuỗi dữ liệu nhận được từ Server, thay đổi tuỳ ứng dụng
	uint16_t ErrorCode;    //Mã code phản hồi HTTP: 200 là thành công
	uint16_t ContentLen;    //Độ dài của dữ liệu mà Server trả về
} HTTP_InfoTypeDef;

typedef struct
{
    char Number1[13];
    char Number2[13];
    char Number3[13];
    char Number4[13];
    char Number5[13];
    char Number6[13];
    char Number7[13];
    char Number8[13];
    char Number9[13];
} PhoneNumber_InfoTypedef; // struct lưu số điện thoại

/**
  * Hàm SIM_RxEvent(void);
  * Mô tả: Sử dụng để nhận và phân loại dữ liệu từ module SIM gửi đến cho vi điều khiển STM32
  * Bắt buộc phải gọi hàm này bên trong HAL_UARTEx_RxEventCallback() ở file main.c
  */
void SIM_RxEvent(void);

/*
 * Hàm SIM_Init();
 * Mô tả: Khởi tạo module SIM
 * [in] PowerPort: Port của chân PWR
 * [in] PowerPin: chân PWR được cấu hình
 */
void SIM_Init(GPIO_TypeDef *PowerPort, uint16_t PowerPin);

/*
 * Hàm SIM_Start();
 * Mô tả: Khởi động module Sim trước khi thực hiện các chức năng khác
 * Gọi trong hàm Sim_Config() trước các lệnh khác.
 */
void SIM_Start(void);

/*
 * Mô tả: Cấu hình các tham số hoạt động cho module SIM
 * Gọi sau hàm SIM_Init()
 */
void SIM_Config(void);

/*
 * Mô tả: Cấu hình các tham số hoạt động cho chức năng SMS của module SIM
 * Gọi sau hàm SIM_Config() nếu cần sử dụng chức năng SMS
 */
void SMS_Config(void);

/*
 * Mô tả: Truyền vào một mảng bộ đệm để chứa chuỗi thời gian.
 * Kích thước mảng: >= 22
 * [out] TimeString: mảng chứa dữ liệu thời gian
 */
char SIM_GetLocalTime(char TimeString[]);

/*
 * Mô tả: Hàm cập nhật thời gian thực lại cho sim
 */
void SMS_syncLocalTime(void);

/*
 * Mô tả: Kiểm tra xem thằng Sim có chạy khum
 */
SIM_StatusTypeDef SIM_TestAT();

/*
 * Mô tả: Reset Module SIM
 */
SIM_StatusTypeDef SIM_Reset();

/*
 * Mô tả: Tra mã USSD
 * [in] USSD: mã USSD, ví dụ *0#, *101#
 * [out] Response: Bộ đệm để chứa kết quả của lệnh USSD
 */
SIM_StatusTypeDef SIM_SendUSSD(const char *USSD, char Response[]);

/*
 * Mô tả: Gửi tin nhắn SMS
 * [in] PhoneNumber: Số điện thoại người nhận
 * [in] Message: Nội dung tin nhắn
 */
SIM_StatusTypeDef SIM_SendSMS(const char *PhoneNumber, const char *Message);

/*
 * Mô tả: Nhận tin nhắn SMS từ module Sim và lưu vào bộ đệm rxBuffer
 */
SIM_StatusTypeDef SIM_ReadSMS();

/*
 * Mô tả: Nhận tin nhắn SMS từ module Sim và lưu vào bộ đệm rxBuffer
 */
SMS_InfoTypeDef SIM_GetSMS(SMS_InfoTypeDef *Smsptr);

/*
 * Mô tả: xử lý khi có tin nhắn đến
 */
//SMS_InfoTypeDef SMS_process(SMS_InfoTypeDef *Smsptr);
void SMS_process();
/*
 * Mô tả: Gửi dữ liệu lên CSDL bằng HTTP POST
 * [in] URL: đường dẫn HTTP của CSDL
 * [in] ContentType: Kiểu định dạng chuỗi dữ liệu, ví dụ: application/json
 * [in] Data: Chuỗi dữ liệu
 * [out] RespInfo: Thông tin trả về của HTTP request
 */
SIM_StatusTypeDef SIM_HTTP_POST(const char *URL, const char *ContentType,
		const char *Data, HTTP_InfoTypeDef *RespInfo);

/*
 * Mô tả: Gửi dữ liệu từ STM32 sang Module Sim bằng UART, thường là tập lệnh AT.
 * [in]: ATCommand: Lệnh AT gửi qua Module Sim, ví dụ "AT+HTTPINIT"
 * [in]: Response: Mong muốn trả về từ module Sim, có thể là "OK", hoặc "DOWNLOAD"
 * [in]: Timeout: Thời gian chờ Module Sim phản hồi lại.
 * */
SIM_StatusTypeDef sendATcommand(const char *ATCommand, const char *Response,
		uint32_t Timeout);
void downloadFW();
void SIM_GetNewFWFileInfo();
SIM_StatusTypeDef SIM_waitRespond(const char *Response, uint32_t Timeout);
HTTP_InfoTypeDef SIM_GetHTTP_Info(HTTP_InfoTypeDef *RespInfo);
char SIM_GetPhoneNumber(char phoneNumber[]);
void SIM_SetID();

//typedef struct
//{
//	volatile bool RxFlag;
//	uint8_t gBuffer[RX_LEN];
//} SIM_BufferTypeDef;

//volatile bool RxFlag;
//uint8_t rxBuffer[RX_LEN]= {0};

#endif /* SIM7600_SIM7X00_H_ */

/*
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */

