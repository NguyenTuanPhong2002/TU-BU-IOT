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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "midlib.h"
#include "lowlib.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart4;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart2_rx;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_UART4_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
SIM_HandleTypeDef *simPtr;
SIM_HTTPTypeDef http = { 0 };
SIM_DataTypeDef simData = { 0 };
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

UART_HandleTypeDef huart2;

#define MAX_LENGTH 251 // �?ộ dài tối đa của chuỗi
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;
char buffer[MAX_LENGTH];

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
	if (huart->Instance == USART1) {
		// SENSOR_process(sensorPtr, Size);
		SIM_checkEvent(simPtr, Size);
	}
}

void removeFirstChar(char *str) {
	int length = strlen(str);

	if (length > 0) {
		for (int i = 0; i < length - 1; i++) {
			str[i] = str[i + 1]; // Di chuyển mỗi ký tự lên một vị trí
		}
		str[length - 1] = '\0'; // Ghi đè ký tự cuối cùng bằng '\0' để đảm bảo chuỗi kết thúc đúng
	}
}

uint8_t flagUCA = 0;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART2) {

		HAL_UART_Transmit(&huart4, "Received: \n", sizeof("Received: \n"),
		HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart4, (uint8_t*) buffer, strlen((uint8_t*) buffer),
		HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart4, "\n", sizeof("\n"),
		HAL_MAX_DELAY);
		flagUCA = 1;
//		removeFirstChar(buffer);
//
//		SIM_getData(simPtr, SIM_DR_TIME, &simData);
//
//		memset(http.data, '\0', SIM_HTTP_DATA_SIZE);
//		snprintf(http.data, SIM_HTTP_DATA_SIZE,
//				"{\"TIME\":\"%s\",\"PN\":\"%s\",%s", simData.timezone,
//				simData.simPhone, buffer);
//		HAL_UART_Transmit(&huart4, "Send to data: \n",
//				sizeof("Send to data: \n"),
//				HAL_MAX_DELAY);
//		HAL_UART_Transmit(&huart4, (uint8_t*) http.data,
//				strlen((uint8_t*) http.data),
//				HAL_MAX_DELAY);
//		HAL_UART_Transmit(&huart4, "\n", sizeof("\n"),
//		HAL_MAX_DELAY);
//		SIM_sendHTTP(simPtr, &http);
//
//		// Reset buffer
//		memset(buffer, "\0", strlen((uint8_t*) buffer));
//
//		HAL_UART_Receive_DMA(&huart2, (uint8_t*) buffer, MAX_LENGTH);
	}
}

void SIM_Init(void) {

	printf("%s\n", __FUNCTION__);
	/* Khởi tạo biến tĩnh để các giá trị khởi tạo không bị mất đi */
	static SIM7600_HandleTypeDef sim7600 = { 0 };

	/* Khởi tạo giá trị cho con tr�?, biến con tr�? là biến toàn cục */
	simPtr = (SIM_HandleTypeDef*) &sim7600;

	/* Khởi tạo giá trị của các member của struct */
	sim7600.hdma = &hdma_usart1_rx;
	sim7600.huart = &huart1;
	sim7600.smsCnt = 0;
	sim7600.smsPos = 0;
//    sim7600.pwrkeyPin = SIM7600_PWRKEY_Pin;
//    sim7600.pwrkeyPort = SIM7600_PWRKEY_GPIO_Port;
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
		HAL_UART_Transmit(&huart4, "SIM START ERROR: \n",
				sizeof("SIM START ERROR: \n"), HAL_MAX_DELAY);
	} else {
		HAL_UART_Transmit(&huart4, "SIM START OK: \n",
				sizeof("SIM START OK: \n"), HAL_MAX_DELAY);
		char pTime[100] = { };
		/* �?ưa module SIM vào chế độ hoạt động đầy đủ chức năng */
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
	MX_UART4_Init();
	MX_USART2_UART_Init();
	MX_USART1_UART_Init();
	/* USER CODE BEGIN 2 */

	HAL_UART_Transmit(&huart4, "system start \n", sizeof("system start \n"),
	HAL_MAX_DELAY);

	HAL_Delay(5000);

	HAL_UART_Transmit(&huart4, "debug start \n", sizeof("debug start \n"),
	HAL_MAX_DELAY);

	SIM_Init();
	HTTP_init(&http,
			"https://capacitor-129e8-default-rtdb.firebaseio.com/halo.json",
			strlen(
					"https://capacitor-129e8-default-rtdb.firebaseio.com/halo.json"),
			HTTP_URL_URI_RESULT);

//	memset(Rx_data, '\0', 600);
//
//	HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t*) Rx_data, 600);
//	__HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);

	HAL_UART_Receive_DMA(&huart2, (uint8_t*) buffer, MAX_LENGTH);

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */
		if (flagUCA == 1) {
			flagUCA = 0;
			removeFirstChar(buffer);

			SIM_getData(simPtr, SIM_DR_TIME, &simData);

			memset(http.data, '\0', SIM_HTTP_DATA_SIZE);
			snprintf(http.data, SIM_HTTP_DATA_SIZE,
					"{\"TIME\":\"%s\",\"PN\":\"%s\",%s", simData.timezone,
					simData.simPhone, buffer);
			HAL_UART_Transmit(&huart4, "Send to data: \n",
					sizeof("Send to data: \n"),
					HAL_MAX_DELAY);
			HAL_UART_Transmit(&huart4, (uint8_t*) http.data,
					strlen((uint8_t*) http.data),
					HAL_MAX_DELAY);
			HAL_UART_Transmit(&huart4, "\n", sizeof("\n"),
			HAL_MAX_DELAY);
			SIM_sendHTTP(simPtr, &http);

			// Reset buffer
			memset(buffer, "\0", strlen((uint8_t*) buffer));

			HAL_UART_Receive_DMA(&huart2, (uint8_t*) buffer, MAX_LENGTH);
		}
		/* USER CODE BEGIN 3 */

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
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief UART4 Initialization Function
 * @param None
 * @retval None
 */
static void MX_UART4_Init(void) {

	/* USER CODE BEGIN UART4_Init 0 */

	/* USER CODE END UART4_Init 0 */

	/* USER CODE BEGIN UART4_Init 1 */

	/* USER CODE END UART4_Init 1 */
	huart4.Instance = UART4;
	huart4.Init.BaudRate = 115200;
	huart4.Init.WordLength = UART_WORDLENGTH_8B;
	huart4.Init.StopBits = UART_STOPBITS_1;
	huart4.Init.Parity = UART_PARITY_NONE;
	huart4.Init.Mode = UART_MODE_TX_RX;
	huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart4.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart4) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN UART4_Init 2 */

	/* USER CODE END UART4_Init 2 */

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
	huart1.Init.BaudRate = 115200;
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
 * Enable DMA controller clock
 */
static void MX_DMA_Init(void) {

	/* DMA controller clock enable */
	__HAL_RCC_DMA1_CLK_ENABLE();
	__HAL_RCC_DMA2_CLK_ENABLE();

	/* DMA interrupt init */
	/* DMA1_Stream5_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
	/* DMA2_Stream2_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);

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
	__HAL_RCC_GPIOE_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(SIM_PWR_GPIO_Port, SIM_PWR_Pin, GPIO_PIN_SET);

	/*Configure GPIO pin : SIM_PWR_Pin */
	GPIO_InitStruct.Pin = SIM_PWR_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(SIM_PWR_GPIO_Port, &GPIO_InitStruct);

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
