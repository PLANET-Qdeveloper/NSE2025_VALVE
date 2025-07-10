/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
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
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "types.h"
#include "valve.h"
#include "ff.h"
#include "fatfs_sd.h"
#include "MAX31855.h"
#include "MCP3425.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
ServoControl_t servo_state = {
    .valve_operation_active = false,
    .valve_operation_start_time = 0,
};
SolenoidControl_t solenoid_state = {
    .solenoid_operation_active = false,
    .solenoid_operation_start_time = 0};
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MAX_FILENAME_LENGTH 64
uint8_t cmd = 0;
FATFS fs;
FIL fil;
UINT bw;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
DMA_HandleTypeDef hdma_i2c1_rx;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;
DMA_HandleTypeDef hdma_spi2_rx;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart5;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
// SDカード用タイマー変数
volatile uint16_t Timer1, Timer2; /* 1ms Timer Counter for SD card operations */
extern volatile uint8_t FatFsCnt; /* FatFs counter for SD card operations (defined in stm32f4xx_it.c) */

// センサーデータ管理変数
#define MAX_DATA_POINTS 10
static SensorData_t data_buffer[1024];
static SensorData_t temp_buffer[1024];
static volatile uint32_t data_buffer_index = 0;
static volatile bool save_data_flag = false;
static volatile bool read_sensor_flag = false;
// DMA用バッファとフラグ
static uint8_t spi2_dma_buffer[4]; // MAX31855用（32bit = 4byte）
static uint8_t i2c1_dma_buffer[3]; // MCP3425用（24bit = 3byte）

// DMA転送完了フラグ
static volatile bool spi2_dma_complete = false;
static volatile bool i2c1_dma_complete = false;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_I2C1_Init(void);
static void MX_SPI1_Init(void);
static void MX_SPI2_Init(void);
static void MX_TIM3_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_UART5_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */
int _write(int file, char *ptr, int len);
void system_init(void);
void process_dma_sensor_data(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// printf関数をUART経由で出力するためのリダイレクト
int _write(int file, char *ptr, int len)
{
  (void)file; // 未使用パラメータの警告を抑制
  HAL_UART_Transmit(&huart5, (uint8_t *)ptr, len, HAL_MAX_DELAY);
  return len;
}

/**
 * @brief ユーザーシステム初期化
 */
void system_init(void)
{
  servo_init(&servo_state);
  solenoid_init(&solenoid_state);

  // 状態を明示的にリセット
  servo_state.valve_operation_active = false;
  servo_state.valve_operation_start_time = 0;
  solenoid_state.solenoid_operation_active = false;
  solenoid_state.solenoid_operation_start_time = 0;

  // UART1の受信割り込み開始
  HAL_UART_Receive_IT(&huart1, &cmd, 1);
  MCP3425_Init(&hi2c1); // MCP3425の初期化
  // SDカードファイルシステムの初期化
  f_mount(&fs, "", 1);

  // Vファイル番号の初期化（既存ファイルをスキャン）
  SD_init_valve_file_number();

#ifdef ENABLE_SD_FORMAT
  FRESULT mount_result = f_mount(&fs, "", 1);
  if (mount_result != FR_OK)
  {
    printf("SDカード初期マウント失敗: FRESULT=%d\r\n", mount_result);

    // フォーマットを試行
    BYTE work[_MAX_SS];
    FRESULT format_result = f_mkfs("", FM_FAT32, 0, work, sizeof(work));

    if (format_result == FR_OK)
    {
      printf("SDカードフォーマット成功\r\n");
      mount_result = f_mount(&fs, "", 1);
      if (mount_result == FR_OK)
      {
        printf("SDカード再マウント成功\r\n");
      }
      else
      {
        printf("SDカード再マウント失敗: FRESULT=%d\r\n", mount_result);
      }
    }
    else
    {
      printf("SDカードフォーマット失敗: FRESULT=%d\r\n", format_result);
    }
  }
  else
  {
    printf("SDカードマウント成功\r\n");
  }
#endif

#ifdef ENABLE_SD_FORMAT
  // SDカードの基本動作テスト
  FRESULT test_result = f_open(&fil, "test.txt", FA_CREATE_ALWAYS | FA_WRITE);
  if (test_result == FR_OK)
  {
    char buffer[64]; // ローカルバッファ
    sprintf(buffer, "SDカードテスト成功\r\n");
    f_write(&fil, buffer, strlen(buffer), &bw);
    f_close(&fil);
  }
#endif
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
  setbuf(stdout, NULL);
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
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_TIM3_Init();
  MX_USART2_UART_Init();
  MX_FATFS_Init();
  MX_UART5_Init();
  MX_USART1_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  system_init();
  // タイマー割り込み開始
  HAL_TIM_Base_Start_IT(&htim2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    uint32_t current_time = HAL_GetTick();
    if (solenoid_state.solenoid_operation_active)
    {
      // ソレノイド操作がアクティブな場合、開始時間が設定されていないならば現在時刻を設定
      if (solenoid_state.solenoid_operation_start_time == 0)
      {
        solenoid_state.solenoid_operation_start_time = current_time;
        solenoid_open();
      }
      else if (current_time - solenoid_state.solenoid_operation_start_time >= 15000)
      {
        // ソレノイド操作が15秒を超えた場合、ソレノイドを閉じる
        solenoid_close();
        solenoid_state.solenoid_operation_active = false;
        solenoid_state.solenoid_operation_start_time = 0;
      }
    }
    if (servo_state.valve_operation_active)
    {
      // サーボ操作がアクティブな場合、開始時間が設定されていないならば現在時刻を設定
      if (servo_state.valve_operation_start_time == 0)
      {
        servo_state.valve_operation_start_time = current_time;
        servo_open();
      }
      else if (current_time - servo_state.valve_operation_start_time >= 30000)
      {
        // サーボ操作が30秒を超えた場合、サーボを閉じる
        servo_close();
        servo_state.valve_operation_active = false;
        servo_state.valve_operation_start_time = 0;
      }
    }

    // DMA完了後のセンサーデータ処理
    process_dma_sensor_data();

    // SD保存処理（非同期実行）
    if (save_data_flag)
    {
      save_data_flag = false;
      __disable_irq();
      uint32_t save_count = data_buffer_index;
      memcpy(temp_buffer, data_buffer, save_count * sizeof(SensorData_t));
      data_buffer_index = 0;
      __enable_irq();
      sd_save_data(temp_buffer, save_count);
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
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

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
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
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
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES_RXONLY;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
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
 * @brief TIM2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
}

/**
 * @brief TIM3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 19;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 15999;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 500;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);
}

/**
 * @brief UART5 Initialization Function
 * @param None
 * @retval None
 */
static void MX_UART5_Init(void)
{

  /* USER CODE BEGIN UART5_Init 0 */

  /* USER CODE END UART5_Init 0 */

  /* USER CODE BEGIN UART5_Init 1 */

  /* USER CODE END UART5_Init 1 */
  huart5.Instance = UART5;
  huart5.Init.BaudRate = 115200;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart5) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART5_Init 2 */

  /* USER CODE END UART5_Init 2 */
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
 * Enable DMA controller clock
 */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
  /* DMA1_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);

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
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12 | GPIO_PIN_3, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);

  /*Configure GPIO pins : PA4 PA11 */
  GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB12 PB3 */
  GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PA9 */
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)
  {
    // 受信したデータで直接バルブ制御
    if (cmd == 'L')
    {
      servo_state.valve_operation_active = true;
    }
    else if (cmd == 'E')
    {
      solenoid_state.solenoid_operation_active = true;
    }
  }
  HAL_UART_Receive_IT(&huart1, &cmd, 1);
}

// SPI DMA完了コールバック関数
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
  if (hspi->Instance == SPI2)
  {
    spi2_dma_complete = true;
  }

}

// I2C DMA完了コールバック関数
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
  if (hi2c->Instance == I2C1)
  {
    i2c1_dma_complete = true;
  }
}

/**
 * @brief TIM2の周期経過コールバック（1秒間隔でDMAセンサーデータ取得開始）
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM2)
  {
    // DMAでのセンサーデータ取得を開始（エラーハンドリング付き）
    HAL_StatusTypeDef spi_status = MAX31855_Read_Temp_DMA(&hspi2, spi2_dma_buffer);
    HAL_StatusTypeDef i2c_status = MCP3425_Read_Pressure_DMA(&hi2c1, i2c1_dma_buffer);
    
    // DMA開始失敗時のエラーハンドリング
    if (spi_status != HAL_OK)
    {
      printf("SPI2 DMA開始エラー: %d\r\n", spi_status);
    }
    
    if (i2c_status != HAL_OK)
    {
      printf("I2C1 DMA開始エラー: %d\r\n", i2c_status);
    }

    // ポーリング方式は無効化（DMAを使用するため）
    // read_sensor_flag = true;
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

/* USER CODE BEGIN 0 */

/**
 * @brief DMA受信完了後のデータ処理
 */
void process_dma_sensor_data(void)
{
  static float temp_data = 0.0f;
  static float press_data = 0.0f;
  static bool temp_valid = false;
  static bool press_valid = false;

  // MAX31855 DMA完了処理
  if (spi2_dma_complete)
  {
    spi2_dma_complete = false;

    // CS信号をHighに戻す
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);

    // 32bitデータを結合
    uint32_t raw_data = (spi2_dma_buffer[0] << 24) |
                        (spi2_dma_buffer[1] << 16) |
                        (spi2_dma_buffer[2] << 8) |
                        spi2_dma_buffer[3];

    // 温度データに変換
    if (!(raw_data & 0x7)) // エラーチェック（D0-D2がすべて0）
    {
      int16_t temp_raw = (raw_data >> 18) & 0x3FFF;
      if (temp_raw & 0x2000)
        temp_raw |= 0xC000; // 符号拡張
      temp_data = temp_raw * 0.25f;
      temp_valid = true;
    }
    else
    {
      printf("MAX31855エラー: フォルトビット検出 (0x%08X)\r\n", raw_data);
    }
  }

  // MCP3425 DMA完了処理
  if (i2c1_dma_complete)
  {
    i2c1_dma_complete = false;

    // データレディビットをチェック（設定レジスタの最上位ビット）
    if (!(i2c1_dma_buffer[2] & 0x80)) // Ready bit check (inverted logic)
    {
      // 16bitデータを結合（12ビットADC）
      int16_t adc_value = (i2c1_dma_buffer[0] << 8) | i2c1_dma_buffer[1];
      
      // 電圧値に変換（12ビット、PGA=1倍、3倍分圧補正）
      float voltage = (float)adc_value * 0.001f * 3.0f;
      
      // MLH02kPSB06A圧力センサの変換式 (kPa単位)
      press_data = 3361.190f * voltage - 1335.857f;
      press_valid = true;
    }
    else
    {
      printf("MCP3425エラー: データ変換中\r\n");
    }
  }

  // 両方のデータが揃った場合にバッファに保存
  if (temp_valid && press_valid && data_buffer_index < MAX_DATA_POINTS)
  {
    data_buffer[data_buffer_index] = (SensorData_t){
        .timestamp = HAL_GetTick(),
        .temp_data = temp_data,
        .press_data = press_data,
        .is_nos_open = solenoid_state.solenoid_operation_active};

    data_buffer_index++;

    // データをリセット
    temp_valid = false;
    press_valid = false;

    if (data_buffer_index >= MAX_DATA_POINTS)
    {
      save_data_flag = true;
    }
  }
}

/* USER CODE END 0 */
