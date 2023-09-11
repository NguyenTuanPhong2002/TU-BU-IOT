/*
 * sim7x00.c
 *
 *  Created on: Jan 30, 2023
 *      Author: tthieu01
 */

#include "sim7x00.h"
#include "fota.h"
volatile bool RxFlag = 0;
volatile bool smsFlag = 0;
extern HTTP_InfoTypeDef httpInfo;
extern HTTP_InfoTypeDef *httpptr;
extern FirmwareInfo_t fw;
char rxBuffer[RX_LEN]= {0};
char TimeString[24] = { 0 };
char phoneBook[RX_LEN] = {0};
char phoneNumber[13] = {0};
char number[13] = {0};
Version_t curFwRunningVer = {0};
Version_t newFwVerOnServer = {0};

/*void SIM_Init(GPIO_TypeDef *PowerPort, uint16_t PowerPin)
{
	HAL_GPIO_WritePin(PowerPort, PowerPin, GPIO_PIN_SET);			//Bat PWR len

	HAL_GPIO_WritePin(DTR_GPIO_Port, DTR_Pin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, GPIO_PIN_RESET);		//Khong Reset
	HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t*) rxBuffer, RX_LEN);
	__HAL_DMA_DISABLE_IT(&SIM_UART_DMA, DMA_IT_HT);
}*/

void SIM_Init(GPIO_TypeDef *PowerPort, uint16_t PowerPin)
{
	HAL_GPIO_WritePin(PowerPort, PowerPin, GPIO_PIN_SET);			//Bat PWR len
	//	HAL_GPIO_WritePin(PowerPort, PowerPin, GPIO_PIN_RESET);			//Bat PWR len
	//	HAL_GPIO_WritePin(PowerPort, PowerPin, GPIO_PIN_SET);			//Bat PWR len
	//	HAL_GPIO_WritePin(PowerPort, PowerPin, GPIO_PIN_RESET);			//Bat PWR len

	HAL_GPIO_WritePin(DTR_GPIO_Port, DTR_Pin, GPIO_PIN_RESET);
	//	HAL_GPIO_WritePin(DTR_GPIO_Port, DTR_Pin, GPIO_PIN_SET);
	//	HAL_GPIO_WritePin(DTR_GPIO_Port, DTR_Pin, GPIO_PIN_RESET);
	//	HAL_GPIO_WritePin(DTR_GPIO_Port, DTR_Pin, GPIO_PIN_SET);

//	while(1)
//	{
//		HAL_GPIO_WritePin(DTR_GPIO_Port, DTR_Pin, GPIO_PIN_RESET);
//		HAL_Delay(2000);
//		HAL_GPIO_WritePin(DTR_GPIO_Port, DTR_Pin, GPIO_PIN_SET);
//		HAL_Delay(2000);
//		HAL_GPIO_WritePin(DTR_GPIO_Port, DTR_Pin, GPIO_PIN_RESET);
//		HAL_Delay(2000);
//		HAL_GPIO_WritePin(DTR_GPIO_Port, DTR_Pin, GPIO_PIN_SET);
//		HAL_Delay(2000);
//	}


	HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, GPIO_PIN_RESET);		//Khong Reset
	//	HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, GPIO_PIN_SET);		// Reset
	//	HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, GPIO_PIN_RESET);		//Khong Reset
	//	HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, GPIO_PIN_SET);		// Reset

	HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t*) rxBuffer, RX_LEN);
	__HAL_DMA_DISABLE_IT(&SIM_UART_DMA, DMA_IT_HT);
}

void SIM_RxEvent(void)
{
	HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t*) rxBuffer, RX_LEN);
	__HAL_DMA_DISABLE_IT(&SIM_UART_DMA, DMA_IT_HT);

	RxFlag = 1;
	if(strstr(rxBuffer, "+CMT:") != NULL)
	{
		smsFlag = true;
	}
}

void SIM_Start(void){
	while(strstr(rxBuffer, "OK") == NULL)
	{
		uint8_t count;
		HAL_UART_Transmit(&SIM_UART, (uint8_t*)"AT\r\n", 4, 1000);
		HAL_UART_Transmit(&DEBUG_UART, (uint8_t*) "\r\nSIM_Init/102	 Connecting\r\n", 28, HAL_MAX_DELAY); // Truyền lệnh ATCommnd sang Sim
		HAL_Delay(200);
		count += 1;
		if(count == 10)
		{
			HAL_NVIC_SystemReset();
		}
	}
}
void SIM_Config(void)
{
	//	SIM_Start();
	sendATcommand("AT+CSCLK=0", "OK", 1000);							// tắt Echo
	sendATcommand("ATE0", "OK", 1000);							// tắt Echo
	sendATcommand("AT+CVALARM=1,3300,4300", "OK", 1000);
	sendATcommand("AT+CPMVT=0,3300,4300", "OK", 1000);

	sendATcommand("AT+CPIN?", "OK", 9000);						//
	sendATcommand("AT+CREG?", "OK", 9000);						//
	sendATcommand("AT+CGREG?", "OK", 9000);						//
	sendATcommand("AT+CNMP=39", "OK",9000);						// Chọn chế độ cho thẻ sim
	sendATcommand("AT+CPBS=\"ME\"", "OK", 9000);				// Chọn chế độ lưu trữ cho module Sim
	sendATcommand("AT+CPMS=\"SM\",\"SM\",\"SM\"", "OK", 9000);	// Lưu trữ tin nhắn ưu thích
	sendATcommand("AT+CMGL=\"ALL\"", "OK", 9000);
	sendATcommand("AT+CMGD=,4", "OK", 9000);						// Xóa tin nhắn khỏi bộ tin nhắn ưu thích
	sendATcommand("AT+CTZU=1", "OK", 9000);						// cài đặt chế độ cập nhật thời gian, 0 là tắt, 1 là bật tự động
	sendATcommand("AT+COPS?", "OK", 9000);						// cài đặt chế độ cập nhật thời gian, 0 là tắt, 1 là bật tự động
	/*	Get parameters need for application*/
	SMS_Config();
}

void SMS_Config(void)
{
	//	sendATcommand("AT+CMGL=\"ALL\"", "OK", 9000);
	sendATcommand("AT+CMGR=0", "OK", 9000);
	//	sendATcommand("AT+CMGD=0", "OK", 9000);
	sendATcommand("AT+CMGF=1", "OK", 9000);
	sendATcommand("AT+CSCS=\"GSM\"", "OK", 9000);
	sendATcommand("AT+CSMP=17,167,0,0", "OK", 9000);
	sendATcommand("AT+CNMI=1,2,0,0,0", "OK", 100);
	sendATcommand("AT+CPBW=1,\"+84934829856\",145,\"Hieu\"", "OK", 9000);
	HAL_Delay(500);
	sendATcommand("AT+CPBW=2,\"+84946109075\",145,\"Tien\"", "OK", 9000);
	HAL_Delay(500);
	sendATcommand("AT+CPBW=3,\"+84934927556\",145,\"Huy\"", "OK", 9000);
	HAL_Delay(500);
	sendATcommand("AT+CPBR=1,10", "OK", 9000);
	memcpy(phoneBook, rxBuffer, RX_LEN);
}

void SMS_syncLocalTime()
{
	sendATcommand("AT+CTZU=1", "OK", 9000);						// cài đặt chế độ cập nhật thời gian, 0 là tắt, 1 là bật tự động
	sendATcommand("AT+CTZU=0", "OK", 9000);						// cài đặt chế độ cập nhật thời gian, 0 là tắt, 1 là bật tự động
	sendATcommand("AT+CCLK?", "OK", 9000);
}

char SIM_GetLocalTime(char TimeString[])
{
	char *token = NULL;
	sendATcommand("AT+CCLK?", "OK", 9000);
	token= strtok(rxBuffer, "\"");
	token = strtok(NULL, "+");
	strcpy(TimeString, token);
	return TimeString;
}
char SIM_GetPhoneNumber(char phoneNumber[])
{
	char *token = NULL;
	sendATcommand("AT+CUSD=1,\"*098#\",15", "OK", 9000);
	SIM_waitRespond("+CUSD", 9000);
	token = strtok (rxBuffer, "\"");
	token = strtok (NULL, " ");
	token = strtok (NULL, " ");
	strcpy(phoneNumber, token);
	HAL_UART_Transmit(&DEBUG_UART, (uint8_t*) phoneNumber, 13, HAL_MAX_DELAY); // Truy�?n lệnh ATCommnd sang Sim
	sendATcommand("\nAT+CUSD=2", "OK", 9000);
	return phoneNumber;
}

SIM_StatusTypeDef SIM_TestAT()
{
	return sendATcommand("AT", "OK", 9000);
}
SIM_StatusTypeDef SIM_Reset()
{
	return sendATcommand("AT+CRESET", "OK", 9000);
}
SIM_StatusTypeDef SIM_SendSMS(const char *phoneNumber,const char *message) {
	uint8_t number[30] = { 0 };
	uint8_t sms[RX_LEN] = { 0 };
	SIM_StatusTypeDef status;
	sendATcommand("AT+CMGF=1", "OK", 9000);

	sprintf((char*) number, "AT+CMGS=\"%s\"", phoneNumber);
	status = sendATcommand((char*) number, ">", 9000);
	if (status == SIM_OK) {
		sprintf((char*) sms, "%s%c", message, 0x1A);
		return sendATcommand((char*) sms, "OK", 9000);
	} else {
		return status;
	}
}
SIM_StatusTypeDef SIM_ReadSMS(){
	sendATcommand("AT+CMGR=0", "OK", 9000);
	return SIM_OK;
}

SMS_InfoTypeDef SIM_GetSMS(SMS_InfoTypeDef *Smsptr)
{
	char *tok = NULL;
	char smsBuffer[RX_LEN]= {0};
	strcpy(smsBuffer, rxBuffer);

	tok = (char *)strtok(smsBuffer, "\"");
	tok = strtok(NULL, "\"");
	strcpy(Smsptr->PhoneNumber, tok);

	tok = strtok(NULL, "\"");
	tok = strtok(NULL, "\"");
	tok = strtok(NULL, "+");
	strcpy(Smsptr->TimeReceive, tok);

	tok = strtok(NULL, "*");
	tok = strtok(NULL, "#");
	strcpy(Smsptr->SmsContent, tok);
	return *Smsptr;
}
void SMS_process()
{
	if(smsFlag == true)
	{
		for(uint8_t j = 0; j < 12; j++)
		{
			number[j] = rxBuffer[j+9];
		}
		if(strstr(phoneBook, number)!= NULL)
		{
			if(strstr(rxBuffer, "DEBUG")!= NULL)
			{
				SIM_SendSMS(number, "SMS DEBUG");
			}
			else if (strstr(rxBuffer, "CHECKPHONEBOOK") != NULL)
			{
				sendATcommand("AT+CPBR=1,10", "OK", 9000);
				SIM_SendSMS(number, rxBuffer);
			}
			else if (strstr(rxBuffer, "SEND") != NULL)
			{
				SIM_SendSMS(number, "Hello Hieu NGU");
			}
			else if (strstr(rxBuffer, "RESET") != NULL)
			{
				HAL_NVIC_SystemReset();
			}
			else if (strstr(rxBuffer, "UPFW") != NULL)
			{
				SIM_GetNewFWFileInfo("https://test-fota-default-rtdb.asia-southeast1.firebasedatabase.app/fota1.json",&httpInfo);
			}
			else if (strstr(rxBuffer, "SETID") != NULL)
			{
				SIM_SetID();
			}
			smsFlag = false;
		}
	}
}


//SMS_InfoTypeDef SMS_process(SMS_InfoTypeDef *Smsptr)
//{
//	if(smsFlag == true)
//	{
//		SIM_GetSMS(Smsptr);
//		if(strstr(Smsptr -> PhoneNumber, "+84934829856")!= NULL)
//		{
//			if(strstr(Smsptr-> SmsContent, "DEBUG")!= NULL)
//			{
//				SIM_SendSMS("+84934829856", "Bao cao thuc tap tot nghiep");
//			}
//			else if (strstr(Smsptr-> SmsContent, "NGU") != NULL)
//			{
//				SIM_SendSMS("+84934829856", "Hello Hieu NGU");
//			}
//			else if (strstr(Smsptr -> SmsContent, "RESET") != NULL)
//			{
//				HAL_NVIC_SystemReset();
//			}
//			else if (strstr(Smsptr -> SmsContent, "UPFW") != NULL)
//			{
//				SIM_GetNewFWFileInfo("https://test-fota-default-rtdb.asia-southeast1.firebasedatabase.app/fota1.json",&httpInfo);
//				SIM_SendSMS("+84934829856", "UPDATE SUCCESSFUL");
//			}
////			else if (strstr(Smsptr -> SmsContent, "SETID") != NULL)
////			{
////
////			}
//			smsFlag = false;
//		}
//	}
//	return *Smsptr;
//}
void SIM_SetID()
{
	char *tok1 = NULL;
	char *tok2 = NULL;
	char *tok3 = NULL;
	char paraString[RX_LEN] = {0};
	tok1 = strtok(rxBuffer, "*");
	tok1 = strtok(NULL, "*");
	tok1 = strtok(NULL, "*");	//vị trí lưu
	tok2 = strtok(NULL, "*");	//số điện thoại
	tok3 = strtok(NULL, "#");	//Người sở hữu SĐT
	sprintf(paraString, "AT+CPBW=%s,\"%s\",129,\"%s\"", tok1,tok2,tok3);
	sendATcommand(paraString, "OK", 9000);
	sendATcommand("AT+CPBR=1,10", "OK", 9000);
}
SIM_StatusTypeDef SIM_HTTP_POST(const char *URL, const char *ContentType,
		const char *Data, HTTP_InfoTypeDef *RespInfo)
{
	char paraString[RX_LEN] = {0};
	sendATcommand("AT+HTTPTERM", "OK", 9000);
	sendATcommand("AT+HTTPINIT", "OK", 9000);
	sprintf(paraString, "AT+HTTPPARA=\"URL\",\"%s\"", URL);
	sendATcommand(paraString, "OK", 9000);								//"AT+HTTPPARA=\"URL\",\"%s\"", URL
	memset((char*) paraString,0, 100);

	sprintf(paraString, "AT+HTTPPARA=\"CONTENT\",\"%s\"", ContentType);
	sendATcommand(paraString, "OK", 9000);								//"AT+HTTPPARA=\"CONTENT\",\"%s\"", ContentType
	memset((char*) paraString,0, 100);

	sprintf(paraString, "AT+HTTPDATA=%d,10000", strlen(Data));
	sendATcommand(paraString, "DOWNLOAD", 9000);						//"AT+HTTPDATA=%d,10000", strlen(Data)
	memset((char*) paraString,0, 100);

	sendATcommand(Data, "OK", 9000);								//DATA gửi đi
	sendATcommand("AT+HTTPACTION=1", "OK", 9000);					//Chọn phương thức Post cho dữ liệu gửi lên
	SIM_waitRespond("+HTTPACTION: 1", 9000);
	printf((char*) rxBuffer);										// In Buffer này ra log

	SIM_GetHTTP_Info(RespInfo);

	sprintf(paraString, "AT+HTTPREAD=0,%u", RespInfo->ContentLen);
	sendATcommand(paraString, "OK", 9000);
	SIM_waitRespond("+HTTPREAD: 0", 9000);

	//	sendATcommand("AT+HTTPTERM", "OK", 9000);
	return SIM_OK;
}
void SIM_GetNewFWFileInfo(const char *URL, HTTP_InfoTypeDef *RespInfo)
{
	char buffer[RX_LEN]={0};
	char paraString[RX_LEN] = {0};

	char *linkFirebaseStorage = NULL;
	char *nameOffireware = NULL;
	char *major = NULL;
	char *minor = NULL;
	char *patch = NULL;

	sendATcommand("AT+HTTPTERM", "OK", 2000);
	sendATcommand("AT+HTTPINIT", "OK", 2000);
	sprintf(paraString, "AT+HTTPPARA=\"URL\",\"%s\"", URL);					//Truy cập vào link (Realtime Database) chứa dữ liệu tên của firmware mới
	sendATcommand(paraString, "OK", 9000);									//"AT+HTTPPARA=\"URL\",\"%s\"", URL

	sendATcommand("AT+HTTPACTION=0","200", 9000);							//HTTP GET
	SIM_waitRespond("+HTTPACTION: 0", 9000);

	SIM_GetHTTP_Info(RespInfo);

	memset((char*) paraString, 0, RX_LEN);
	sprintf(paraString, "AT+HTTPREAD=0,%u", RespInfo->ContentLen);			//AT+HTTPREAD=0,151
	sendATcommand(paraString, "}", 9000);									//Send message, nhận về phản hồi là đường dẫn chứa firmware mới, có tên của firmware
	linkFirebaseStorage = strtok(rxBuffer, "{");
	for(int i =0; i<3; i++)
	{
		linkFirebaseStorage = strtok(NULL, "\"");
	}
	strcpy(buffer, linkFirebaseStorage);
	nameOffireware = strtok(buffer, "/");
	for(int i = 0; i< 5; i++)
	{
		nameOffireware = strtok(NULL, "/");
	}
	nameOffireware = strtok(NULL, "?");
	major = strtok(nameOffireware, "_");
	major = strtok(NULL, ".");
	newFwVerOnServer.major = atoi(major);

	minor = strtok(NULL, ".");
	newFwVerOnServer.minor = atoi(minor);

	patch = strtok(NULL, "_");
	newFwVerOnServer.patch = atoi(patch);

	curFwRunningVer.major 	= flash_read_Int(CURRENT_RUNNING_FW_VER_MAJOR_ADDR);
	curFwRunningVer.minor 	= flash_read_Int(CURRENT_RUNNING_FW_VER_MINOR_ADDR);
	curFwRunningVer.patch 	= flash_read_Int(CURRENT_RUNNING_FW_VER_PATCH_ADDR);
	if(newFwVerOnServer.patch > curFwRunningVer.patch)
	{
		if(newFwVerOnServer.minor != curFwRunningVer.minor)
		{
			flash_EraseSector(5);
			flash_write_Int(FLAG_NEED_UPDATE_FW_ADDR, 1);
			flash_EraseSector(6);
			flash_write_Array(PHONE_NUMBER_REQUESTED_ADDR, number, 12);
			flash_write_Array(LINK_FW_ON_SERVER, linkFirebaseStorage, strlen(linkFirebaseStorage));
			flash_write_Int(SIZE_LINK_FW_ON_SERVER, strlen(linkFirebaseStorage));
		}
		else
		{
			memset((char*) paraString, 0, RX_LEN);
			sprintf(paraString, "\r\nAT+HTTPPARA=\"URL\",\"%s\"",
					"https://test-fota-default-rtdb.asia-southeast1.firebasedatabase.app/fota2.json");
			sendATcommand(paraString, "OK", 9000);									//"AT+HTTPPARA=\"URL\",\"%s\"", URL
			sendATcommand("AT+HTTPACTION=0", "OK", 9000);							//HTTP GET
			SIM_waitRespond("+HTTPACTION: 0", 9000);

			SIM_GetHTTP_Info(RespInfo);

			memset((char*) paraString, 0, RX_LEN);
			sprintf(paraString, "AT+HTTPREAD=0,%u", RespInfo->ContentLen);			//AT+HTTPREAD=0,151
			sendATcommand(paraString, "}", 9000);									//Send message, nhận về phản hồi là đường dẫn chứa firmware mới, có tên của firmware

			linkFirebaseStorage = NULL;
			linkFirebaseStorage = strtok(rxBuffer, "{");
			for(int i =0; i<3; i++)
			{
				linkFirebaseStorage = strtok(NULL, "\"");
			}
			flash_EraseSector(5);
			flash_write_Int(FLAG_NEED_UPDATE_FW_ADDR, 1);
			flash_EraseSector(6);
			flash_write_Array(PHONE_NUMBER_REQUESTED_ADDR, number, 12);
			flash_write_Array(LINK_FW_ON_SERVER, linkFirebaseStorage, strlen(linkFirebaseStorage));
			flash_write_Int(SIZE_LINK_FW_ON_SERVER, strlen(linkFirebaseStorage));
		}
		HAL_NVIC_SystemReset();
	}
}

SIM_StatusTypeDef sendATcommand(const char *ATCommand, const char *Response, uint32_t Timeout)
{
	SIM_StatusTypeDef status = SIM_BUSY;
	memset((char*) rxBuffer, 0, RX_LEN);		// Xóa hết buffer
	char txBuffer[RX_LEN] = { 0 };			// Khai báo txBuffer gửi đi

	sprintf((char*) txBuffer, "%s\r\n", ATCommand);		// Ép buffer thành lệnh nhập vào

	HAL_UART_Transmit(&SIM_UART, (uint8_t*) txBuffer, (uint16_t)strlen(txBuffer), HAL_MAX_DELAY); // Truyền lệnh ATCommnd sang Sim
	printf((char*) txBuffer);		// In Buffer này ra log
	uint32_t tickStart = HAL_GetTick();
	while (status == SIM_BUSY) {
		if ((HAL_GetTick() - tickStart >= Timeout)) {
			status = SIM_TIMEOUT;
			break;
		}
		if (RxFlag == true) { // RxFlag = 1 nghĩa là có ngắt nhận về chuỗi DMA
			if (strstr((char*) rxBuffer, Response) != NULL) { // tìm cái response trong rx buffer trả về, thường là OK
				status = SIM_OK;
				break;
			} else if (strstr((char*) rxBuffer, "ERROR") != NULL) { // vẫn là tìm response trả về, nhưng là lỗi ERROR
				status = SIM_ERROR;
				break;
			}
		}
	}
	printf((char*) rxBuffer); // in ra hàm trả về
	RxFlag = false;
	return status;
}
SIM_StatusTypeDef SIM_waitRespond(const char *Response, uint32_t Timeout)
{
	SIM_StatusTypeDef status = SIM_BUSY;
	uint32_t tickStart = HAL_GetTick();
	while (status == SIM_BUSY) {
		if ((HAL_GetTick() - tickStart >= Timeout)) {
			status = SIM_TIMEOUT;
			break;
		}
		if (RxFlag == true) { // RxFlag = 1 nghĩa là có ngắt nhận về chuỗi DMA
			if (strstr((char*) rxBuffer, Response) != NULL) { // tìm cái response trong rx buffer trả về, thường là OK
				status = SIM_OK;
				break;
			} else if (strstr((char*) rxBuffer, "ERROR") != NULL) { // vẫn là tìm response trả về, nhưng là lỗi ERROR
				status = SIM_ERROR;
				break;
			}
		}
	}
	RxFlag = false;
	return status;
}

HTTP_InfoTypeDef SIM_GetHTTP_Info(HTTP_InfoTypeDef *RespInfo)
{
	char *errorstring = NULL;												//Phản hồi của http
	char *ContentLenString = NULL;											//Độ dài của dữ liệu
	ContentLenString = strtok(rxBuffer, ",");
	errorstring = strtok(NULL, ",");
	ContentLenString = strtok(NULL, " ");
	RespInfo->ErrorCode = atoi(errorstring);
	RespInfo->ContentLen = atoi(ContentLenString);
	return *RespInfo;
}
