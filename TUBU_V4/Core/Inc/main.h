/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define DETECT1_Pin GPIO_PIN_0
#define DETECT1_GPIO_Port GPIOC
#define DETECT2_Pin GPIO_PIN_1
#define DETECT2_GPIO_Port GPIOC
#define DETECT3_Pin GPIO_PIN_2
#define DETECT3_GPIO_Port GPIOC
#define DETECT4_Pin GPIO_PIN_3
#define DETECT4_GPIO_Port GPIOC
#define SIM7600_PWRKEY_Pin GPIO_PIN_0
#define SIM7600_PWRKEY_GPIO_Port GPIOA
#define SIM_TX_Pin GPIO_PIN_2
#define SIM_TX_GPIO_Port GPIOA
#define SIM_RX_Pin GPIO_PIN_3
#define SIM_RX_GPIO_Port GPIOA
#define SIM_RTS_Pin GPIO_PIN_4
#define SIM_RTS_GPIO_Port GPIOA
#define SD_SCK_Pin GPIO_PIN_5
#define SD_SCK_GPIO_Port GPIOA
#define SD_MISO_Pin GPIO_PIN_6
#define SD_MISO_GPIO_Port GPIOA
#define SD_MOSI_Pin GPIO_PIN_7
#define SD_MOSI_GPIO_Port GPIOA
#define SD_CS_Pin GPIO_PIN_4
#define SD_CS_GPIO_Port GPIOC
#define RFM95_DIO0_Pin GPIO_PIN_5
#define RFM95_DIO0_GPIO_Port GPIOC
#define RS485_TX_Pin GPIO_PIN_10
#define RS485_TX_GPIO_Port GPIOB
#define RS485_RX_Pin GPIO_PIN_11
#define RS485_RX_GPIO_Port GPIOB
#define RFM95_SCK_Pin GPIO_PIN_13
#define RFM95_SCK_GPIO_Port GPIOB
#define RFM95_MISO_Pin GPIO_PIN_14
#define RFM95_MISO_GPIO_Port GPIOB
#define RFM95_MOSI_Pin GPIO_PIN_15
#define RFM95_MOSI_GPIO_Port GPIOB
#define RFM95_RS_Pin GPIO_PIN_8
#define RFM95_RS_GPIO_Port GPIOD
#define RFM95_NSS_Pin GPIO_PIN_10
#define RFM95_NSS_GPIO_Port GPIOD
#define RFM95_DIO1_Pin GPIO_PIN_6
#define RFM95_DIO1_GPIO_Port GPIOC
#define RFM95_DIO2_Pin GPIO_PIN_7
#define RFM95_DIO2_GPIO_Port GPIOC
#define LED4_Pin GPIO_PIN_0
#define LED4_GPIO_Port GPIOD
#define LED3_Pin GPIO_PIN_2
#define LED3_GPIO_Port GPIOD
#define LED2_Pin GPIO_PIN_4
#define LED2_GPIO_Port GPIOD
#define LED1_Pin GPIO_PIN_6
#define LED1_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
