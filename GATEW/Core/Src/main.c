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
#include "lora.h"
#include "rfm95.h"
#include "sim7600.h"

#include <stdio.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define LORA_RESPONSE_SIZE 100U
#define LORA_REQUEST_SIZE  100U
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi2;

UART_HandleTypeDef huart2;
UART_HandleTypeDef huart6;
DMA_HandleTypeDef hdma_usart2_rx;

/* USER CODE BEGIN PV */
char *CosF = NULL;
char *Voltage = NULL;
char *Current = NULL;
char *Frequency = NULL;
char *ActivePower = NULL;
char *ReactivePower = NULL;
char *AvP = NULL;
char *AvQ = NULL;
char *CTT1 = NULL;
char *CTT2 = NULL;
char *CTT3 = NULL;
char *CTT4 = NULL;
char *coil1 = NULL;
char *coil2 = NULL;
char *coil3 = NULL;
char *coil4 = NULL;
RFM95_HandleTypeDef rfm95w = { 0 };
LORA_HandleTypeDef *lora;
uint8_t msgResLoRa[LORA_RESPONSE_SIZE] = { 0 };
uint8_t messLora[LORA_REQUEST_SIZE] = { 0 };

SIM_HandleTypeDef *simPtr;
SIM_HTTPTypeDef http = { 0 };
SIM_DataTypeDef simData = { 0 };
char pTime[100] = { };

uint8_t irqLoRaFlag = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_SPI2_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART6_UART_Init(void);
/* USER CODE BEGIN PFP */

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {

	if (huart->Instance == USART2) {
		SIM_checkEvent(simPtr, Size);
	}
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	/* Wake up MCU from stop mode by any EXTI line */
	if (GPIO_Pin == rfm95w.dio0) {
		/* Set flag to read LoRa message */
		irqLoRaFlag = 1;
	}
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void SIM_Init(void) {

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
	sim7600.pwrkeyPin = SIM7600_PWR_Pin;
	sim7600.pwrkeyPort = SIM7600_PWR_GPIO_Port;
	sim7600.resetPort = SIM7600_RESET_GPIO_Port;
	sim7600.resetPin = SIM7600_RESET_Pin;
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

	} else {
		printf("Sim init OK");

		/* Dưa module SIM vào chế độ hoạt động đầy đủ chức năng */
		SIM_setFunction(simPtr, SIM_MODE_FULL);
		SIM_setLocalTimeAuto(simPtr);
		SIM_getLocalTime(simPtr, pTime, 22);
		SIM_getData(simPtr,
				SIM_DR_IMEI | SIM_DR_RSSI | SIM_DR_NWTYPE | SIM_DR_SIMPHONE
						| SIM_DR_TEMP, &simData);
	}
}

void LORA_begin() {
	lora = (LORA_HandleTypeDef*) &rfm95w;

	rfm95w.resetPort = RFM95_RS_GPIO_Port;
	rfm95w.resetPin = RFM95_RS_Pin;
	rfm95w.nssPort = RFM95_NSS_GPIO_Port;
	rfm95w.nssPin = RFM95_NSS_Pin;
	rfm95w.dio0Port = RFM95_DIO0_GPIO_Port;
	rfm95w.dio0 = RFM95_DIO0_Pin;
	rfm95w.hspi = &hspi2;

	rfm95w.parent.init = (loraInit) &RFM95_init;
	rfm95w.parent.transmit = (loraTransmit) &RFM95_transmit;
	rfm95w.parent.receive = (loraReceive) &RFM95_receiveContinuous;
	rfm95w.parent.startReceiveIT =
			(loraStartReceiveOnIRQ) &RFM95_startReceiveIT;
	rfm95w.parent.receiveIT = (loraReceiveOnIRQ) &RFM95_receiveIT;

	rfm95w.parent.getRSSI = (loraGetRSSI) &RFM95_getRSSI;

	if (LORA_init(lora) == THT_OK) {
		printf("LORA OK");
	} else {
		printf("LORA ERROR");
	}
}
void processingValue() {
	char value[98] = { 0 };
	memset(value, '\0', 98);
	memcpy(&value, &msgResLoRa[2], 98);

	CosF = strtok(value, ",");
	Voltage = strtok(NULL, ",");
	Current = strtok(NULL, ",");
	Frequency = strtok(NULL, ",");
	ActivePower = strtok(NULL, ",");
	ReactivePower = strtok(NULL, ",");
	AvP = strtok(NULL, ",");
	AvQ = strtok(NULL, ",");
	CTT1 = strtok(NULL, ",");
	CTT2 = strtok(NULL, ",");
	CTT3 = strtok(NULL, ",");
	CTT4 = strtok(NULL, ",");
	coil1 = strtok(NULL, ",");
	coil2 = strtok(NULL, ",");
	coil3 = strtok(NULL, ",");
	coil4 = strtok(NULL, ",");
	sprintf(http.data, "{\"TIME\":\"%s\""
			",\"CosFi\":\"%s\""
			",\"Voltage\":\"%s\""
			",\"Current\":\"%s\""
			",\"Frequency\":\"%s\""
			",\"ActivePowe\":\"%s\""
			",\"ReactivePower\":\"%s\""
			",\"AvP\":\"%s\""
			",\"AvQ\":\"%s\""
			",\"CTT1\":\"%s\""
			",\"CTT2\":\"%s\""
			",\"CTT3\":\"%s\""
			",\"CTT4\":\"%s\""
			",\"Coil 1\":\"%s\""
			",\"Coil 2\":\"%s\""
			",\"Coil 3\":\"%s\""
			",\"Coil 4\":\"%s\"}", pTime, CosF, Voltage, Current, Frequency,
			ActivePower, ReactivePower, AvP, AvQ, CTT1, CTT2, CTT3, CTT4, coil1,
			coil2, coil3, coil4);
	SIM_sendHTTP(simPtr, &http);
}

void loraHandle() {

	messLora[0] = 0x01;
	messLora[1] = 0x02;

	messLora[LORA_REQUEST_SIZE - 1] = THT_CRC8XOR((uint8_t*) messLora,
	LORA_REQUEST_SIZE - 1);

	LORA_startReceiveIT(lora);
	uint32_t timeStap = HAL_GetTick();
	while (HAL_GetTick() - timeStap < 60000) {
		if (irqLoRaFlag) {
			irqLoRaFlag = 0;
			LORA_receiveIT(lora, msgResLoRa, LORA_RESPONSE_SIZE);
			if (msgResLoRa[0] == messLora[0] && msgResLoRa[1] == messLora[1]) {
				processingValue();
			}
			break;
		}
	}
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
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
  MX_SPI2_Init();
  MX_USART2_UART_Init();
  MX_USART6_UART_Init();
  /* USER CODE BEGIN 2 */
	SIM_Init();
	HTTP_init(&http,
			"https://capacitor-129e8-default-rtdb.firebaseio.com/citx.json",
			strlen(
					"https://capacitor-129e8-default-rtdb.firebaseio.com/citx.json"),
			HTTP_URL_URI_RESULT);
	LORA_begin();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
		loraHandle();
		HAL_Delay(2000);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

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
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

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
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

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
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 115200;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */

  /* USER CODE END USART6_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, SIM7600_PWR_Pin|SIM7600_RESET_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, RFM95_RS_Pin|RFM95_NSS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : SIM7600_PWR_Pin SIM7600_RESET_Pin */
  GPIO_InitStruct.Pin = SIM7600_PWR_Pin|SIM7600_RESET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : RFM95_DIO0_Pin */
  GPIO_InitStruct.Pin = RFM95_DIO0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(RFM95_DIO0_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : RFM95_RS_Pin RFM95_NSS_Pin */
  GPIO_InitStruct.Pin = RFM95_RS_Pin|RFM95_NSS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
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
