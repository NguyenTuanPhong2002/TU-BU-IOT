/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
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
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#include "sim7x00.h"
#include "fota.h"
#include "flash.h"
#include "RS485.h"
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef hrtc;

SD_HandleTypeDef hsd;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
UART_HandleTypeDef huart6;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart3_rx;

/* USER CODE BEGIN PV */
RTC_TimeTypeDef RTC_Time = {0};
RTC_DateTypeDef RTC_Date = {0};
RTC_TimeTypeDef sTime = {0};
RTC_DateTypeDef sDate = {0};
RTC_AlarmTypeDef sAlarm = {0};
FATFS fatfs;
FIL myfile;
FRESULT fresult;
int a = 10;
float b = 0.00123;

uint8_t buffer[50];
uint8_t recive_arr[100];
uint8_t byte_read = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART6_UART_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_RTC_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_SDIO_SD_Init(void);
/* USER CODE BEGIN PFP */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
  SIM_RxEvent();
}
uint8_t RTC_Flag = 0;
typedef struct
{
  uint8_t Minute;
  uint8_t Second;
} Time_TypeDef; // struct th�?i gian

Time_TypeDef GetMinSec = {0};
Time_TypeDef *Timeptr = &GetMinSec;

SMS_InfoTypeDef SmsTypedef = {0};
SMS_InfoTypeDef *Smsptr = &SmsTypedef;

HTTP_InfoTypeDef httpInfo = {0};
HTTP_InfoTypeDef *httpptr = &httpInfo;

PhoneNumber_InfoTypedef phoneNumTydef = {0};
PhoneNumber_InfoTypedef *phoneNumPtr = &phoneNumTydef;

char COSF_ReceivedBuff[9];
char I_ReceivedBuff[9];
char V_ReceivedBuff[9];
char P_ReceivedBuff[9];
char Q_ReceivedBuff[9];
char AvP_ReceivedBuff[9];
char AvQ_ReceivedBuff[9];
char PE_ReceivedBuff[9];
char QE_ReceivedBuff[9];
char coil_ReceivedBuff[7];
char CON1_ReceivedBuff[9];
char BT_ReceivedBuff[9];
char F_ReceivedBuff[9];
char Temp_ReceivedBuff[9];
char RTU_CosFi[] = {0x01, 0x03, 0x01, 0x61, 0x00, 0x02, 0x94, 0x29};

char UART2_ReceivedBuff[12];

char coil[7];
char jsonBuffer[RX_LEN] = {0};
bool IN[8];
extern char phoneNumber[12];
float CosFi = 0.5;
float Current = 0.1;
float Voltage = 0.1;
float ActivePower = 10.1;
float ReactivePower = 10.2;
float Frequency = 50;
float temperature = 10.2;

uint8_t CONTACTOR1;
uint8_t CONTACTOR2;

uint16_t AvP;
uint16_t AvQ;
extern char TimeString[24];

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void CONTACTOR_READ()
{
  IN[0] = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_7);
  IN[1] = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_9);
  IN[2] = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_11);
  IN[3] = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_13);
  IN[4] = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_15);
  IN[5] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13);
  IN[6] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15);
  IN[7] = HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_9);
  CONTACTOR1 = ~((uint8_t)IN[4] << 3 | (uint8_t)IN[5] << 2 | (uint8_t)IN[6] << 1 | (uint8_t)IN[7]);
  CONTACTOR2 = ~((uint8_t)IN[0] << 3 | (uint8_t)IN[1] << 2 | (uint8_t)IN[2] << 1 | (uint8_t)IN[3]);
}
RS485Query_t m_RS485Query5 =
    {
        .slaveAddress = 0x01,
        .mbFunction = Read_HoldingRegister, 
        .regAddress = 0x43,
        .regCount = 0x02};
RS485Query_t m_RS485Query2 =
    {
        .slaveAddress = 0x01,
        .mbFunction = Read_HoldingRegister,
        .regAddress = 0x03,
        .regCount = 0x02};
RS485Query_t m_RS485Query3 =
    {
        .slaveAddress = 0x01,
        .mbFunction = Read_HoldingRegister,
        .regAddress = 0x11,
        .regCount = 0x02};
RS485Query_t m_RS485Query4 =
    {
        .slaveAddress = 0x01,
        .mbFunction = Read_HoldingRegister,
        .regAddress = 0x01,
        .regCount = 0x02};
RS485Query_t m_RS485QueryP =
    {
        .slaveAddress = 0x01,
        .mbFunction = Read_HoldingRegister,
        .regAddress = 0x21,
        .regCount = 0x02};
RS485Query_t m_RS485QueryQ =
    {
        .slaveAddress = 0x01,
        .mbFunction = Read_HoldingRegister,
        .regAddress = 0x51,
        .regCount = 0x02};
RS485Query_t m_RS485QueryAvP =
    {
        .slaveAddress = 0x01,
        .mbFunction = Read_HoldingRegister,
        .regAddress = 0x23,
        .regCount = 0x02};
RS485Query_t m_RS485QueryAvQ =
    {
        .slaveAddress = 0x01,
        .mbFunction = Read_HoldingRegister,
        .regAddress = 0x53,
        .regCount = 0x02};

RS485Query_t m_RS485Query_coil =
    {
        .slaveAddress = 0x01,
        .mbFunction = Read_CoilStatus,
        .regAddress = 0x00,
        .regCount = 0x10};
RS485Query_t m_RS485Query_temp =
    {
        .slaveAddress = 0x01,
        .mbFunction = Read_HoldingRegister,
        .regAddress = 0x18D,
        .regCount = 0x02};
void Set_Time(int H, int M, int S);
void Set_Alarm(int m);
void SplitTime(Time_TypeDef *Timeptr);
void CONTACTOR_READ();
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc);
void RS485_Master_Receive(char Master_ReceivedBuff[], int length);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void SendDataToServer();

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
  /* USER CODE BEGIN 1 */
  SCB->VTOR = (uint32_t)APP1_START_ADDR;
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
  MX_USART2_UART_Init();
  MX_USART6_UART_Init();
  MX_USART3_UART_Init();
  MX_RTC_Init();
  MX_USART1_UART_Init();
  MX_SDIO_SD_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */

  SIM_Init(PWR_GPIO_Port, PWR_Pin);

  HAL_UART_Transmit(&DEBUG_UART, (uint8_t *)"\r\nSIM_Init/102	 Connecting\r\n", 28, HAL_MAX_DELAY); // Truy�?n lệnh ATCommnd sang Sim
  SIM_Config();
  SIM_GetLocalTime(TimeString);
  HTTP_InfoTypeDef httpInfo = {0};

  SIM_GetPhoneNumber(phoneNumber);
  SplitTime(&GetMinSec);
  Set_Time(5, Timeptr->Minute, Timeptr->Second);
  Set_Alarm(Timeptr->Minute);

  SIM_GetLocalTime(TimeString);
  sprintf(jsonBuffer,
          "{\"TIME\":\"%s\""
          ",\"PN\":\"%s\""
          ",\"CosFi\":\"%.2f\""
          ",\"Three-phase Equivalent Voltage\":\"%.2f\""
          ",\"Three-phase Equivalent Current\":\"%.2f\""
          ",\"Frequency\":\"%.2f\""
          ",\"ActivePowe\":\"%.2f\""
          ",\"ReactivePower\":\"%.2f\""
          ",\"Three-phase equivalent active power\":\"%d\""
          ",\"Three-phase equivalent reactive power\":\"%d\""
          ",\"CTT1\":\"%u\""
          ",\"CTT2\":\"%u\""
          ",\"CTT3\":\"%u\""
          ",\"CTT4\":\"%u\""
          ",\"Coil 1\":\"%u\""
          ",\"Coil 2\":\"%u\""
          ",\"Coil 3\":\"%u\""
          ",\"Coil 4\":\"%u\"}",
          TimeString, phoneNumber, CosFi, Voltage, Current, Frequency, ActivePower, ReactivePower,
          AvP, AvQ, (IN[0] ^ 0x01), (IN[1] ^ 0x01), (IN[2] ^ 0x01), (IN[3] ^ 0x01),
          (coil_ReceivedBuff[3] & 0x01), ((coil_ReceivedBuff[3] & 0x02) >> 1),
          ((coil_ReceivedBuff[3] & 0x04) >> 2), ((coil_ReceivedBuff[3] & 0x08) >> 3));
  SIM_HTTP_POST(
      "https://capacitor-129e8-default-rtdb.firebaseio.com/testA.json",
      "application/json", jsonBuffer, &httpInfo);

  HAL_UART_Receive_IT(&huart2, (uint8_t *)UART2_ReceivedBuff, 8);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET);

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    SMS_process();
    SendDataToServer();
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
 * @brief RTC Initialization Function
 * @param None
 * @retval None
 */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};
  RTC_AlarmTypeDef sAlarm = {0};

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
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
   */
  sTime.Hours = 20;
  sTime.Minutes = 0;
  sTime.Seconds = 0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_WEDNESDAY;
  sDate.Month = RTC_MONTH_APRIL;
  sDate.Date = 20;
  sDate.Year = 22;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable the Alarm A
   */
  sAlarm.AlarmTime.Hours = 0;
  sAlarm.AlarmTime.Minutes = 20;
  sAlarm.AlarmTime.Seconds = 0;
  sAlarm.AlarmTime.SubSeconds = 0;
  sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
  sAlarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY | RTC_ALARMMASK_HOURS | RTC_ALARMMASK_SECONDS;
  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  sAlarm.AlarmDateWeekDay = 1;
  sAlarm.Alarm = RTC_ALARM_A;
  if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */
}

/**
 * @brief SDIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_SDIO_SD_Init(void)
{

  /* USER CODE BEGIN SDIO_Init 0 */

  /* USER CODE END SDIO_Init 0 */

  /* USER CODE BEGIN SDIO_Init 1 */

  /* USER CODE END SDIO_Init 1 */
  hsd.Instance = SDIO;
  hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
  hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
  hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
  hsd.Init.BusWide = SDIO_BUS_WIDE_1B;
  hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd.Init.ClockDiv = 0;
  /* USER CODE BEGIN SDIO_Init 2 */

  /* USER CODE END SDIO_Init 2 */
}

/**
 * @brief USART1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART1_UART_Init(void)
{

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
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
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
 * @brief USART3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART3_UART_Init(void)
{

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
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */
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
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
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
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3 | PWR_Pin | RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, DTR_Pin | GPIO_PIN_9 | GPIO_PIN_10, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12 | PWRD14_Pin | GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC3 PWR_Pin RST_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_3 | PWR_Pin | RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : DTR_Pin PA9 PA10 */
  GPIO_InitStruct.Pin = DTR_Pin | GPIO_PIN_9 | GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PE7 PE9 PE11 PE13
                           PE15 */
  GPIO_InitStruct.Pin = GPIO_PIN_7 | GPIO_PIN_9 | GPIO_PIN_11 | GPIO_PIN_13 | GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PB13 PB15 */
  GPIO_InitStruct.Pin = GPIO_PIN_13 | GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PD9 */
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PD12 PWRD14_Pin PD15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12 | PWRD14_Pin | GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PD7 */
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PE0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
}

/* USER CODE BEGIN 4 */
void SplitTime(Time_TypeDef *Timeptr)
{
  char *p = NULL;
  char *q = NULL;
  SIM_GetLocalTime(TimeString);
  p = strtok(TimeString, "/");
  p = strtok(NULL, "/");
  p = strtok(NULL, ",");
  p = strtok(NULL, ":");
  q = strtok(NULL, ":");
  Timeptr->Minute = atoi(q);
  p = strtok(NULL, ":");
  Timeptr->Second = atoi(p);
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == huart2.Instance)
  {
    HAL_UART_Receive_IT(&huart2, (uint8_t *)UART2_ReceivedBuff, 8);
  }
}
void SendDataToServer()
{
  if (RTC_Flag == true)
  {
    SplitTime(&GetMinSec);
    if (Timeptr->Minute % 10 == 0)
    {
      SIM_GetLocalTime(TimeString);
      // Case 1:
      HAL_UART_Transmit(&huart6, (uint8_t *)RTU_CosFi, 8, 1000);
      RS485_Master_Receive(COSF_ReceivedBuff, 9);
      CosFi = (float)(((uint16_t)COSF_ReceivedBuff[5] << 8) | ((uint16_t)COSF_ReceivedBuff[6])) / 100;
      HAL_Delay(500);
      // Case 2:
      RS485_transmit(&huart6, m_RS485Query2);
      RS485_Master_Receive(V_ReceivedBuff, 9);
      Voltage = (float)(((uint16_t)V_ReceivedBuff[5] << 8) | ((uint16_t)V_ReceivedBuff[6]));
      HAL_Delay(500);
      // Case 3:
      RS485_transmit(&huart6, m_RS485Query3);
      RS485_Master_Receive(I_ReceivedBuff, 9);
      Current = (float)(((uint16_t)I_ReceivedBuff[5] << 8) | ((uint16_t)I_ReceivedBuff[6])) / 100;
      HAL_Delay(500);
      // Case 4:
      RS485_transmit(&huart6, m_RS485Query4);
      RS485_Master_Receive(F_ReceivedBuff, 9);
      Frequency = (float)(((uint16_t)F_ReceivedBuff[5] << 8) | ((uint16_t)F_ReceivedBuff[6])) / 10;
      HAL_Delay(500);
      // Case 5:
      RS485_transmit(&huart6, m_RS485QueryAvQ);
      RS485_Master_Receive(AvQ_ReceivedBuff, 9);
      AvQ = ((uint16_t)AvQ_ReceivedBuff[5] << 8) | ((uint16_t)AvQ_ReceivedBuff[6]);
      HAL_Delay(500);
      // Case 5:
      RS485_transmit(&huart6, m_RS485QueryP);
      RS485_Master_Receive(P_ReceivedBuff, 9);
      ActivePower = (((uint16_t)P_ReceivedBuff[5] << 8) | ((uint16_t)P_ReceivedBuff[6])) / 1000;
      HAL_Delay(500);
      // Case 7:
      RS485_transmit(&huart6, m_RS485QueryQ);
      RS485_Master_Receive(Q_ReceivedBuff, 9);
      ReactivePower = (((uint16_t)Q_ReceivedBuff[5] << 8) | ((uint16_t)Q_ReceivedBuff[6])) / 1000;
      HAL_Delay(500);
      // Case 8:
      RS485_transmit(&huart6, m_RS485QueryAvP);
      RS485_Master_Receive(AvP_ReceivedBuff, 9);
      AvP = ((uint16_t)AvP_ReceivedBuff[5] << 8) | ((uint16_t)AvP_ReceivedBuff[6]);
      HAL_Delay(500);
      ////Case 9:
      CONTACTOR_READ();
      RS485_transmit(&huart6, m_RS485Query_coil);
      RS485_Master_Receive(coil_ReceivedBuff, 7);
      HAL_Delay(500);
      //		Case 10:
      SIM_GetLocalTime(TimeString);
      //		printf("CosFi: %.2f\n", CosFi);
      //		printf("Voltage: %.2f\n", Voltage);
      //		printf("Current: %.2f\n", Current);
      //		printf("Frequency: %.2f\n", Frequency);
      //		printf("ActivePower(P): %.2f\n", ActivePower);
      //		printf("ReactivePower(Q): %.2f\n", ReactivePower);
      //		printf("Three-phase equivalent active power(AvP): %d\n", AvP);
      //		printf("Average equivalent reactive power(AvQ): %d\n\n", AvQ);
      //
      //		printf("Contactor1: %u\n", (IN[0]^0x01));
      //		printf("Contactor2: %u\n", (IN[1]^0x01));
      //		printf("Contactor3: %u\n", (IN[2]^0x01));
      //		printf("Contactor4: %u\n\n", (IN[3]^0x01));
      //
      //		printf("Coil 1: %u\n", (coil_ReceivedBuff[3] &	 0x01));
      //		printf("Coil 2: %u\n", ((coil_ReceivedBuff[3] & 0x02)>>1));
      //		printf("Coil 3: %u\n", ((coil_ReceivedBuff[3] & 0x04)>>2));
      //		printf("Coil 4: %u\n\n", ((coil_ReceivedBuff[3] & 0x08)>>3));
      //		printf("\n");
      sprintf(jsonBuffer,
              "{\"TIME\":\"%s\""
              ",\"PN\":\"%s\""
              ",\"CosFi\":\"%.2f\""
              ",\"Three-phase Equivalent Voltage\":\"%.2f\""
              ",\"Three-phase Equivalent Current\":\"%.2f\""
              ",\"Frequency\":\"%.2f\""
              ",\"ActivePowe\":\"%.2f\""
              ",\"ReactivePower\":\"%.2f\""
              ",\"Three-phase equivalent active power\":\"%d\""
              ",\"Three-phase equivalent reactive power\":\"%d\""
              ",\"CTT1\":\"%u\""
              ",\"CTT2\":\"%u\""
              ",\"CTT3\":\"%u\""
              ",\"CTT4\":\"%u\""
              ",\"Coil 1\":\"%u\""
              ",\"Coil 2\":\"%u\""
              ",\"Coil 3\":\"%u\""
              ",\"Coil 4\":\"%u\"}",
              TimeString, phoneNumber, CosFi, Voltage, Current, Frequency, ActivePower, ReactivePower,
              AvP, AvQ, (IN[0] ^ 0x01), (IN[1] ^ 0x01), (IN[2] ^ 0x01), (IN[3] ^ 0x01),
              (coil_ReceivedBuff[3] & 0x01), ((coil_ReceivedBuff[3] & 0x02) >> 1),
              ((coil_ReceivedBuff[3] & 0x04) >> 2), ((coil_ReceivedBuff[3] & 0x08) >> 3));
      SIM_HTTP_POST(
          "https://capacitor-129e8-default-rtdb.firebaseio.com/testA.json",
          "application/json", jsonBuffer, &httpInfo);
      HAL_Delay(120000);
      SplitTime(&GetMinSec);
      Set_Time(5,Timeptr->Minute, Timeptr->Second);
      Set_Alarm(Timeptr->Minute);
    }
  }
  RTC_Flag = false;
}

void Set_Alarm(int m)
{
  if (m % 10 == 0)
  {
    m = m + 10;
  }
  if (m == 60)
  {
    m = 0;
  }
  if ((m >= 1) && (m <= 9))
  {
    m = 10;
  }
  if ((m >= 11) && (m <= 19))
  {
    m = 20;
  }
  if ((m >= 21) && (m <= 29))
  {
    m = 30;
  }
  if ((m >= 31) && (m <= 39))
  {
    m = 40;
  }
  if ((m >= 41) && (m <= 49))
  {
    m = 50;
  }
  if ((m >= 51) && (m <= 59))
  {
    m = 0;
  }
  /** Enable the Alarm A
   */
  sAlarm.AlarmTime.Hours = 0;
  sAlarm.AlarmTime.Minutes = m;
  sAlarm.AlarmTime.Seconds = 0;
  sAlarm.AlarmTime.SubSeconds = 0;
  sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
  sAlarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY | RTC_ALARMMASK_HOURS | RTC_ALARMMASK_SECONDS;
  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  sAlarm.AlarmDateWeekDay = 1;
  sAlarm.Alarm = RTC_ALARM_A;
  if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
}

void Set_Time(int H, int M, int S)
{
  sTime.Hours = H;
  sTime.Minutes = M;
  sTime.Seconds = S;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_WEDNESDAY;
  sDate.Month = RTC_MONTH_APRIL;
  sDate.Date = 5;
  sDate.Year = 5;
  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
  //	if(RTC_Flag == 0)
  //	{
  //		RTC_Flag = 1;
  //	}
  RTC_Flag = true;
}

#ifdef __GNUC__
#define PUTCHAR_PROTOPYTE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOPYTE int fputc(int ch, FILE *f)
#endif
PUTCHAR_PROTOPYTE
{
  HAL_UART_Transmit(&DEBUG_UART, (uint8_t *)&ch, 1, 0xFFFF);
  return ch;
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

  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
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
