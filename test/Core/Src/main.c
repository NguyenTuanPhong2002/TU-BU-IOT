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
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
enum eCommand {
	SET_DEV_ID = 1, SET_LED_STATE, GET_DEV_ID, GET_LED_STATE,
};
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
char rxBuffer[50] = { 0 }; //Khai báo bộ đệm để chứa dữ liệu đến
enum eCommand eCmd = 0;
char deviceID[7] = { 0 }; //Lưu ID của thiết bị
uint8_t ledOrder = 0; //Số thứ tự của led
uint8_t ledState = 0; //Trạng thái của đèn led
uint8_t rxFlag = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART3_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* In phản hồi ra màn hình terminal */
void response(char *string);

/* Xoá bộ đệm nhận dữ liệu */
void deleteRxData();

/* Kiểm tra dữ liệu nhận được là command nào */
void checkCommand();

/* Hàm cấu hình ID cho thiết bị */
void setDeviceID();

/* Hàm lấy ID của thiết bị */
void getDeviceID();

/* Hàm cấu hình trạng thái cho đèn led */
void setLedState();

/* Hàm lấy trạng thái của đèn led */
void getLedState();
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
	MX_USART3_UART_Init();
	/* USER CODE BEGIN 2 */

	HAL_Delay(2000);
	HAL_UARTEx_ReceiveToIdle_IT(&huart3, (uint8_t*) rxBuffer, 50);
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		if (rxFlag == 1) {
			commandHandler();
			rxFlag = 0;
		}
		/* USER CODE END WHILE */

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
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	/* USER CODE BEGIN MX_GPIO_Init_1 */
	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOD, LED2_Pin | LED1_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pins : LED2_Pin LED1_Pin */
	GPIO_InitStruct.Pin = LED2_Pin | LED1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	/* USER CODE BEGIN MX_GPIO_Init_2 */
	/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/* In phản hồi ra màn hình terminal */
void response(char *string) {
	HAL_UART_Transmit(&huart3, (uint8_t*) string, strlen(string), 1000);
}

/* Xoá bộ đệm nhận dữ liệu */
void deleteRxData() {
	memset(rxBuffer, '\0', 50);
}

/* Kiểm tra dữ liệu nhận được là command nào */
void checkCommand() {
	if (strstr(rxBuffer, "*SET_DEV_ID:") != NULL)
		eCmd = SET_DEV_ID;
	else if (strstr(rxBuffer, "*GET_DEV_ID") != NULL)
		eCmd = GET_DEV_ID;
	else if (strstr(rxBuffer, "*SET_LED_STATE:") != NULL)
		eCmd = SET_LED_STATE;
	else if (strstr(rxBuffer, "*GET_LED_STATE:") != NULL)
		eCmd = GET_LED_STATE;
	else
		eCmd = 0;
}

/* Hàm cấu hình ID cho thiết bị */
void setDeviceID() {
	/* Kiểm tra độ dài của command +SET_DEV_ID:123456\r\n */
	if (strlen(rxBuffer) == 20) {
		/*Tách ID thiết bị */
		//Cách 1: sử dụng strtok
		char *token = NULL;
		token = strtok(rxBuffer, ":"); //+SET_DEV_ID
		token = strtok(NULL, "\r"); //123456
		memcpy(deviceID, token, 6); //deviceID = 123456;
		token = NULL;
		//Cách 2: sử dụng memcpy
		//memcpy(deviceID, rxBuffer + 12, 6);
		response("\r\n+OK\r\n ");
	} else {
		response("\r\n+ERROR:error command\r\n");
	}
}

/* Hàm lấy ID của thiết bị */
void getDeviceID() {
	/* Kiểm tra độ dài command +GET_DEV_ID\r\n */
	if (strlen(rxBuffer) == 13) {
		/* Kiểm tra độ dài của deviceID */
		if (strlen(deviceID) == 0) {
			response("\r\n+ERROR:undefined\r\n");
		} else {
			char *buffer = malloc(20 * sizeof(char));      //cấp phát động
			memset(buffer, '\0', 20); //xoá bộ đệm buffer
			sprintf(buffer, "\r\n+ID:%s\r\nOK\r\n", deviceID);
			response(buffer);
			free(buffer);
			buffer = NULL;
		}
	} else {
		response("+ERROR:error command\r\n");
	}
}

/* Hàm cấu hình trạng thái cho đèn led */
void setLedState() {
	/* Kiểm tra độ dài của command +SET_LED_STATE:n,s\r\n (n = 1, 2), (s = 0, 1)*/
	if (strlen(rxBuffer) == 20) {
		/* Tách số thứ tự của led */
		char *token = NULL;
		token = strtok(rxBuffer, ":"); //+SET_LED_STATE
		token = strtok(NULL, ","); //n
		ledOrder = atoi(token);
		/* Tách trạng thái của led */
		token = strtok(NULL, "\r"); //s
		ledState = atoi(token);
		token = NULL;

		if ((ledState == 0) || (ledState) == 1) {
			if (ledOrder == 1) {
				HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, ledState);
				response("\r\nOK\r\n");
			} else if (ledOrder == 2) {
				HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, ledState);
				response("\r\nOK\r\n");
			} else
				response("\r\n+ERROR:undefined led\r\n");
		} else {
			response("\r\n+ERROR:undefined state\r\n");
		}

	} else {
		response("\r\n+ERROR:error command\r\n");
	}
}

/* Hàm lấy trạng thái của đèn led */
void getLedState() {
	/* Kiểm tra độ dài command +GET_LED_STATE:n\r\n */
	if (strlen(rxBuffer) == 18) {
		char *token = NULL;
		token = strtok(rxBuffer, ":"); //+SET_LED_STATE
		/* Tách thứ tự của led */
		token = strtok(NULL, "\r"); //s
		ledOrder = atoi(token);
		token = NULL;

		char buffer[20] = { 0 };
		if (ledOrder == 1) {
			sprintf(buffer, "\r\n+LED:%d,%d\r\nOK\r\n", ledOrder,
					HAL_GPIO_ReadPin(LED1_GPIO_Port, LED1_Pin));
			response(buffer);
		} else if (ledOrder == 2) {
			sprintf(buffer, "\r\n+LED:%d,%d\r\nOK\r\n", ledOrder,
					HAL_GPIO_ReadPin(LED2_GPIO_Port, LED2_Pin));
			response(buffer);
		} else
			response("\r\n+ERROR:undefined led\r\n");
	} else {
		response("\r\n+ERROR:error command\r\n");
	}
}

/* Hàm xử lý chính */
void commandHandler() {
	switch (eCmd) {
	case SET_DEV_ID:
		setDeviceID();
		break;
	case GET_DEV_ID:
		getDeviceID();
		break;
	case SET_LED_STATE:
		setLedState();
		break;
	case GET_LED_STATE:
		getLedState();
		break;
	default:
		break;
	}
	eCmd = 0;
	deleteRxData();
}

/* Hàm gọi lại xử lý phục vụ ngắt UART */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
	if (huart->Instance == USART3) {
		checkCommand();
		rxFlag = 1;
		//HAL_UART_Transmit(huart, (uint8_t *) "\n", 1, 100);
		//HAL_UART_Transmit(huart, (uint8_t *) rxBuffer, strlen(rxBuffer), 100);
		HAL_UARTEx_ReceiveToIdle_IT(&huart3, (uint8_t*) rxBuffer, 50);
	}
}
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
