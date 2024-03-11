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
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include "lora.h"

#include "handle.h"

#include "../../RFM/rfm.h"
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
SPI_HandleTypeDef hspi3;
DMA_HandleTypeDef hdma_spi3_rx;

/* USER CODE BEGIN PV */
RFM95_HandleTypeDef rfm95w = { 0 };

LORA_HandleTypeDef *lora;

uint8_t irqLoRaFlag = 0;
char id[15] = {0};
char master[15] = {0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_SPI3_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void LORA_begin() {
	lora = (LORA_HandleTypeDef*) &rfm95w;

	rfm95w.resetPort = RESET_GPIO_Port;
	rfm95w.resetPin = RESET_Pin;
	rfm95w.nssPort = NSS_GPIO_Port;
	rfm95w.nssPin = NSS_Pin;
	rfm95w.dio1Port = DIO1_GPIO_Port;
	rfm95w.dio1 = DIO1_Pin;
	rfm95w.hspi = &hspi3;

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
/**
 * The function CTL_CRC8XOR calculates the XOR checksum of a given array of bytes.
 *
 * @param array An array of uint8_t values that you want to calculate the CRC8 XOR checksum for.
 * @param size The `size` parameter in the `CTL_CRC8XOR` function represents the number of elements in
 * the array that is being passed to the function for calculating the CRC (Cyclic Redundancy Check)
 * value. It indicates the size of the array in terms of the number of elements it contains
 *
 * @return The function `CTL_CRC8XOR` returns a `uint8_t` value, which is the result of XORing all
 * elements in the input array.
 */
uint8_t CTL_CRC8XOR(const uint8_t *array, size_t size) {
	uint8_t value = 0;
	for (size_t i = 0; i < size; i++) {
		value ^= array[i];
	}
	return value;
}

void loraHandle() {
	enum {
		LORA_RESPONSE_SIZE = 4
	};
	uint8_t msgResLoRa[LORA_RESPONSE_SIZE] = { 0 };
	uint8_t messLora[4] = { 4,5,6,7 };

//	char id[CFG_DEVICE_ID_SIZE] = { 0 };
//	char master[CFG_DEVICE_ID_SIZE] = { 0 };
//
//	for (size_t i = 0; i < CFG_DEVICE_ID_SIZE - 1; i++) {
//		id[i] = *(volatile uint8_t*) (CFG_EEPROM_DEVICE_ID + i);
//	}
//
//	for (size_t i = 0; i < CFG_MASTER_ID_SIZE - 1; i++) {
//		master[i] = *(volatile uint8_t*) (CFG_EEPROM_MASTER_ID + i);
//	}

	messLora[0] = CTL_CRC8XOR((uint8_t*) id, strlen(id));
	messLora[1] = CTL_CRC8XOR((uint8_t*) master, strlen(master));
	messLora[2] = 0x01;
	messLora[3] = CTL_CRC8XOR(messLora, 3);

//	LORA_startReceiveIT(lora);
	while(1){

		LORA_receiveIT(lora,msgResLoRa,4);
		LORA_transmit(lora, messLora, 4, 1000);
		HAL_Delay(1000);
	}
//	uint16_t timeStap = HAL_GetTick();
//	while (HAL_GetTick() - timeStap < 60000) {
//		if (irqLoRaFlag) {
//			irqLoRaFlag = 0;
//
//			LORA_transmit(lora, messLora, 4, 1000);
//
//
//			break;
//		}
//	}
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
  MX_SPI3_Init();
  /* USER CODE BEGIN 2 */
  LORA_begin();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  loraHandle();
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
  * @brief SPI3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI3_Init(void)
{

  /* USER CODE BEGIN SPI3_Init 0 */

  /* USER CODE END SPI3_Init 0 */

  /* USER CODE BEGIN SPI3_Init 1 */

  /* USER CODE END SPI3_Init 1 */
  /* SPI3 parameter configuration*/
  hspi3.Instance = SPI3;
  hspi3.Init.Mode = SPI_MODE_MASTER;
  hspi3.Init.Direction = SPI_DIRECTION_2LINES;
  hspi3.Init.DataSize = SPI_DATASIZE_4BIT;
  hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi3.Init.NSS = SPI_NSS_SOFT;
  hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi3.Init.CRCPolynomial = 7;
  hspi3.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi3.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI3_Init 2 */

  /* USER CODE END SPI3_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Channel1_IRQn);

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
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, RESET_Pin|NSS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : RESET_Pin NSS_Pin */
  GPIO_InitStruct.Pin = RESET_Pin|NSS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : DIO1_Pin */
  GPIO_InitStruct.Pin = DIO1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DIO1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : DIO2_Pin */
  GPIO_InitStruct.Pin = DIO2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DIO2_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	/* Wake up MCU from stop mode by any EXTI line */
	if (GPIO_Pin == rfm95w.dio1) {
		/* Set flag to read LoRa message */
		irqLoRaFlag = 1;
	}
}
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
  while (1)
  {
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
