/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ducati.h"
#include "lora.h"
#include "sim7600.h"
#include "sd.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define RX_LEN			500
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart3_rx;

/* USER CODE BEGIN PV */
ducati_typeDef ducati = { 0 };
//STATUS_CONTACTOR *CON[8] = { 0 };
bool IN[8];
SIM_HandleTypeDef *simPtr;
SIM_HTTPTypeDef http = { 0 };
SIM_DataTypeDef simData = { 0 };

SIM_HTTPTypeDef *jsonBuffer;
char TimeString[24] = {0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_SPI1_Init(void);
static void MX_SPI2_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_RTC_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
	if (huart->Instance == USART3) {

		ducati.DucatiFlag = true;
		Get_size(&ducati, Size);

	}
	if (huart->Instance == USART2) {
		SIM_checkEvent(simPtr, Size);
	}
}

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void ducati_Init(void) {
	ducati.dma = &hdma_usart3_rx;
	ducati.huart = &huart3;
}

void CONTACTOR_READ()
{
  IN[0] = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0);
  IN[1] = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1);
  IN[2] = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2);
  IN[3] = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3);
}

void SIM_Init(void) {
	HAL_GPIO_WritePin(SIM7600_PWRKEY_GPIO_Port, SIM7600_PWRKEY_Pin,
			GPIO_PIN_SET); // Bat PWR len

	printf("%s\n", __FUNCTION__);
	/* Khởi tạo biến tĩnh để các giá trị khởi tạo không bị mất đi */
	static SIM7600_HandleTypeDef sim7600 = { 0 };

	/* Khởi tạo giá trị cho con tr�?, biến con tr�? là biến toàn cục */
	simPtr = (SIM_HandleTypeDef*) &sim7600;

	/* Khởi tạo giá trị của các member của struct */
	sim7600.hdma = &hdma_usart2_rx;
	sim7600.huart = &huart2;
	sim7600.smsCnt = 0;
	sim7600.smsPos = 0;
//	sim7600.pwrkeyPin = SIM7600_PWRKEY_Pin;
//	sim7600.pwrkeyPort = SIM7600_PWRKEY_GPIO_Port;
//    sim7600.resetPort = SIM7600_RESET_GPIO_Port;
//    sim7600.resetPin = SIM7600_RESET_Pin;
//    sim7600.sleepPort = SIM7600_DTR_GPIO_Port;
//    sim7600.sleepPin = SIM7600_DTR_Pin;
//    sim7600.powerEnPort = SIM7600_PWREN_GPIO_Port;
//    sim7600.powerEnPin = SIM7600_PWREN_Pin;
	sim7600.parent.CSR = 0;

	sim7600.parent.simSendATCommand = (simSendATCommand) &SIM7600_sendATCommand;
	sim7600.parent.simWakeup = (simWakeup) &SIM7600_wakeup;
	sim7600.parent.simSleep = (simSleep) &SIM7600_sleep;
	sim7600.parent.simInit = (simInit) &SIM7600_init;
	sim7600.parent.simSetFunction = (simSetFunction) &SIM7600_setFunction;
	sim7600.parent.simProcess = (simProcess) &SIM7600_process;
	sim7600.parent.simCheckEvent = (simCheckEvent) &SIM7600_checkEvent;
	sim7600.parent.simReadSMS = (simReadSMS) &SIM7600_readSMS;
	sim7600.parent.simSendSMS = (simSendSMS) &SIM7600_sendSMS;
	sim7600.parent.simGetSMSCounter = (simGetSMSCounter) &SIM7600_getSMSCounter;
	sim7600.parent.simCallUSSD = (simCallUSSD) &SIM7600_callUSSD;
	sim7600.parent.simGetRSSI = (simGetRSSI) &SIM7600_getRSSI;
	sim7600.parent.simGetTemperature =
			(simGetTemperature) &SIM7600_getTemperature;
	sim7600.parent.simGetNwtype = (simGetNwtype) &SIM7600_getNetworkType;
	sim7600.parent.simGetSimPhone = (simGetSimPhone) &SIM7600_getSimPhone;
	sim7600.parent.simGetIMEI = (simGetIMEI) &SIM7600_getIMEI;
	sim7600.parent.simGetLocalTime = (simGetLocalTime) &SIM7600_getLocalTime;
	sim7600.parent.simGetFileSize = (simGetFileSize) &SIM7600_getFileSize;
	sim7600.parent.simDeleteFile = (simDeleteFile) &SIM7600_deleteFile;
	sim7600.parent.simSetLocalTimeAuto =
			(simSetLocalTimeAuto) &SIM7600_setLocalTimeAuto;
	sim7600.parent.simSetLocalTimeManual =
			(simSetLocalTimeManual) &SIM7600_setLocalTimeManual;
	sim7600.parent.simSelectPhonebookMem =
			(simSelectPhonebookMem) &SIM7600_selectPhonebookMem;
	sim7600.parent.simSavePhonebook = (simSavePhonebook) &SIM7600_savePhonebook;
	sim7600.parent.simReadPhonebook = (simReadPhonebook) &SIM7600_readPhonebook;
	sim7600.parent.simFindPhonebook = (simFindPhonebook) &SIM7600_findPhonebook;
	sim7600.parent.simSendHTTP = (simSendHTTP) &SIM7600_sendHTTP;
	sim7600.parent.simGetFileHTTP = (simGetFileHTTP) &SIM7600_getFileHTTP;

	/* Khởi tạo module SIM */
	if (THT_OK != SIM_init(simPtr)) {
		printf("Sim init fail\n");
		HAL_UART_Transmit(&huart2, "SIM START ERROR: \n",
				sizeof("SIM START ERROR: \n"), HAL_MAX_DELAY);
	} else {
		HAL_UART_Transmit(&huart2, "SIM START OK: \n",
				sizeof("SIM START OK: \n"), HAL_MAX_DELAY);
		char pTime[100] = { };
		/* Dưa module SIM vào chế độ hoạt động đầy đủ chức năng */
		SIM_setFunction(simPtr, SIM_MODE_FULL);
		SIM_setLocalTimeAuto(simPtr);
		SIM_getLocalTime(simPtr, pTime, 22);
		SIM_getData(simPtr,
				SIM_DR_IMEI | SIM_DR_RSSI | SIM_DR_NWTYPE | SIM_DR_SIMPHONE
						| SIM_DR_TEMP, &simData);
	}
}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_SPI1_Init();
	MX_SPI2_Init();
	MX_USART2_UART_Init();
	MX_USART3_UART_Init();
	MX_FATFS_Init();
	MX_RTC_Init();
	MX_USART1_UART_Init();
	/* USER CODE BEGIN 2 */
	ducati_Init();
	CONTACTOR_READ();
	SIM_Init();
	HTTP_init(&http,
			"https://capacitor-129e8-default-rtdb.firebaseio.com/citx.json",
			strlen(
					"https://capacitor-129e8-default-rtdb.firebaseio.com/citx.json"),
			HTTP_URL_URI_RESULT);
	 sprintf(jsonBuffer->data,
		"{\"CosFi\":\"%.2f\""
		",\"Three-phase Equivalent Voltage\":\"%.2f\""
		",\"Three-phase Equivalent Current\":\"%.2f\""
		",\"Frequency\":\"%.2f\""
		",\"ActivePowe\":\"%.2f\""
		",\"ReactivePower\":\"%.2f\""
		",\"Three-phase equivalent ActivePower\":\"%d\""
		",\"Three-phase equivalent ReactivePower\":\"%d\"}",
		getCosF(&ducati), getVol(&ducati),
		getCurrent(&ducati), getFrequency(&ducati), getActivePower(&ducati), getReactivePower(&ducati),
		getAvP(&ducati), getAvQ(&ducati));

	 HTTP_sendCurrentData(jsonBuffer);
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		getVol(&ducati);
		getCosF(&ducati);
		getCurrent(&ducati);
		getFrequency(&ducati);

		getAvQ(&ducati);
		getAvP(&ducati);
		getActivePower(&ducati);
		getReactivePower(&ducati);
		getCoilStatus(&ducati);
		HAL_Delay(1000);
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI
			| RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.LSIState = RCC_LSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 25;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 4;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief RTC Initialization Function
 * @param None
 * @retval None
 */
static void MX_RTC_Init(void) {

	/* USER CODE BEGIN RTC_Init 0 */

	/* USER CODE END RTC_Init 0 */

	RTC_TimeTypeDef sTime = { 0 };
	RTC_DateTypeDef sDate = { 0 };
	RTC_AlarmTypeDef sAlarm = { 0 };

	/* USER CODE BEGIN RTC_Init 1 */

	/* USER CODE END RTC_Init 1 */

	/** Initialize RTC Only
	 */
	hrtc.Instance = RTC;
	hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
	hrtc.Init.AsynchPrediv = 127;
	hrtc.Init.SynchPrediv = 255;
	hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
	hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
	if (HAL_RTC_Init(&hrtc) != HAL_OK) {
		Error_Handler();
	}

	/* USER CODE BEGIN Check_RTC_BKUP */

	/* USER CODE END Check_RTC_BKUP */

	/** Initialize RTC and set the Time and Date
	 */
	sTime.Hours = 0x9;
	sTime.Minutes = 0x0;
	sTime.Seconds = 0x0;
	sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sTime.StoreOperation = RTC_STOREOPERATION_RESET;
	if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK) {
		Error_Handler();
	}
	sDate.WeekDay = RTC_WEEKDAY_THURSDAY;
	sDate.Month = RTC_MONTH_MARCH;
	sDate.Date = 0x28;
	sDate.Year = 24;

	if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK) {
		Error_Handler();
	}

	/** Enable the Alarm A
	 */
	sAlarm.AlarmTime.Hours = 0x9;
	sAlarm.AlarmTime.Minutes = 0x15;
	sAlarm.AlarmTime.Seconds = 0x1;
	sAlarm.AlarmTime.SubSeconds = 0x0;
	sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
	sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
	sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
	sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
	sAlarm.AlarmDateWeekDay = 0x1;
	sAlarm.Alarm = RTC_ALARM_A;
	if (HAL_RTC_SetAlarm(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN RTC_Init 2 */

	/* USER CODE END RTC_Init 2 */

}

/**
 * @brief SPI1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI1_Init(void) {

	/* USER CODE BEGIN SPI1_Init 0 */

	/* USER CODE END SPI1_Init 0 */

	/* USER CODE BEGIN SPI1_Init 1 */

	/* USER CODE END SPI1_Init 1 */
	/* SPI1 parameter configuration*/
	hspi1.Instance = SPI1;
	hspi1.Init.Mode = SPI_MODE_MASTER;
	hspi1.Init.Direction = SPI_DIRECTION_2LINES;
	hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi1.Init.NSS = SPI_NSS_SOFT;
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
	hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi1.Init.CRCPolynomial = 10;
	if (HAL_SPI_Init(&hspi1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN SPI1_Init 2 */

	/* USER CODE END SPI1_Init 2 */

}

/**
 * @brief SPI2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI2_Init(void) {

	/* USER CODE BEGIN SPI2_Init 0 */

	/* USER CODE END SPI2_Init 0 */

	/* USER CODE BEGIN SPI2_Init 1 */

	/* USER CODE END SPI2_Init 1 */
	/* SPI2 parameter configuration*/
	hspi2.Instance = SPI2;
	hspi2.Init.Mode = SPI_MODE_MASTER;
	hspi2.Init.Direction = SPI_DIRECTION_2LINES;
	hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi2.Init.NSS = SPI_NSS_SOFT;
	hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
	hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi2.Init.CRCPolynomial = 10;
	if (HAL_SPI_Init(&hspi2) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN SPI2_Init 2 */

	/* USER CODE END SPI2_Init 2 */

}

/**
 * @brief USART1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART1_UART_Init(void) {

	/* USER CODE BEGIN USART1_Init 0 */

	/* USER CODE END USART1_Init 0 */

	/* USER CODE BEGIN USART1_Init 1 */

	/* USER CODE END USART1_Init 1 */
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 9600;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USART1_Init 2 */

	/* USER CODE END USART1_Init 2 */

}

/**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART2_UART_Init(void) {

	/* USER CODE BEGIN USART2_Init 0 */

	/* USER CODE END USART2_Init 0 */

	/* USER CODE BEGIN USART2_Init 1 */

	/* USER CODE END USART2_Init 1 */
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart2) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USART2_Init 2 */

	/* USER CODE END USART2_Init 2 */

}

/**
 * @brief USART3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART3_UART_Init(void) {

	/* USER CODE BEGIN USART3_Init 0 */

	/* USER CODE END USART3_Init 0 */

	/* USER CODE BEGIN USART3_Init 1 */

	/* USER CODE END USART3_Init 1 */
	huart3.Instance = USART3;
	huart3.Init.BaudRate = 115200;
	huart3.Init.WordLength = UART_WORDLENGTH_8B;
	huart3.Init.StopBits = UART_STOPBITS_1;
	huart3.Init.Parity = UART_PARITY_NONE;
	huart3.Init.Mode = UART_MODE_TX_RX;
	huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart3.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart3) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USART3_Init 2 */

	/* USER CODE END USART3_Init 2 */

}

/**
 * Enable DMA controller clock
 */
static void MX_DMA_Init(void) {

	/* DMA controller clock enable */
	__HAL_RCC_DMA1_CLK_ENABLE();

	/* DMA interrupt init */
	/* DMA1_Stream1_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
	/* DMA1_Stream5_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	/* USER CODE BEGIN MX_GPIO_Init_1 */
	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, SIM7600_PWRKEY_Pin | SIM_RTS_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOC,
			SD_CS_Pin | RFM95_DIO0_Pin | RFM95_DIO1_Pin | RFM95_DIO2_Pin,
			GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOD,
			RFM95_RS_Pin | RFM95_NSS_Pin | LED4_Pin | LED3_Pin | LED2_Pin
					| LED1_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pins : DETECT1_Pin DETECT2_Pin DETECT3_Pin DETECT4_Pin */
	GPIO_InitStruct.Pin = DETECT1_Pin | DETECT2_Pin | DETECT3_Pin | DETECT4_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pin : SIM7600_PWRKEY_Pin */
	GPIO_InitStruct.Pin = SIM7600_PWRKEY_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(SIM7600_PWRKEY_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : SIM_RTS_Pin */
	GPIO_InitStruct.Pin = SIM_RTS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(SIM_RTS_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : SD_CS_Pin RFM95_DIO0_Pin RFM95_DIO1_Pin RFM95_DIO2_Pin */
	GPIO_InitStruct.Pin = SD_CS_Pin | RFM95_DIO0_Pin | RFM95_DIO1_Pin
			| RFM95_DIO2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pins : RFM95_RS_Pin RFM95_NSS_Pin LED4_Pin LED3_Pin
	 LED2_Pin LED1_Pin */
	GPIO_InitStruct.Pin = RFM95_RS_Pin | RFM95_NSS_Pin | LED4_Pin | LED3_Pin
			| LED2_Pin | LED1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	/* USER CODE BEGIN MX_GPIO_Init_2 */
	/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */


/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
