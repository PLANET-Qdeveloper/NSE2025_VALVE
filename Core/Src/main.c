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
#include "config.h"
#include "app_types.h"
#include "state_manager.h"
#include "sensor_manager.h"
#include "data_manager.h"
#include "comm_manager.h"
#include "servo.h"
#include "fatfs.h"
#include "diskio.h"
#include "ff.h"
#include "fatfs_sd.h"
#include "MAX31855.h"
#include "MCP3425.h"
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
I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c3;

RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart5;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
// SDカード用タイマー変数
volatile uint16_t Timer1, Timer2; /* 1ms Timer Counter for SD card operations */
extern volatile uint8_t FatFsCnt; /* FatFs counter for SD card operations (defined in stm32f4xx_it.c) */

// データバッファ（State Manager管理のため、システム状態は削除）
static DataBuffer_t data_buffer[MAX_DATA_POINTS];

// センサーデータ管理用変数
static uint32_t data_buffer_index = 0;
static uint32_t last_sensor_read_time = 0;
static uint32_t last_data_save_time = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_SPI1_Init(void);
static void MX_SPI2_Init(void);
static void MX_TIM3_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_RTC_Init(void);
static void MX_UART5_Init(void);
static void MX_I2C3_Init(void);
/* USER CODE BEGIN PFP */
int _write(int file, char *ptr, int len);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
FATFS fs;
FIL fil;
UINT bw;

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
void system_user_init(void)
{
  // State Manager初期化
  state_manager_init();

  // サーボモーター初期化（PWM開始と初期位置設定も含む）
  servo_init(&htim3, TIM_CHANNEL_1);

  // UART通信プロトコル初期化
  comm_init();

  // SDカードファイルシステムの初期化
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
        state_manager_set_sd_card_ready(true);
      }
      else
      {
        printf("SDカード再マウント失敗: FRESULT=%d\r\n", mount_result);
        state_manager_set_sd_card_ready(false);
      }
    }
    else
    {
      printf("SDカードフォーマット失敗: FRESULT=%d\r\n", format_result);
      state_manager_set_sd_card_ready(false);
    }
  }
  else
  {
    printf("SDカードマウント成功\r\n");
    state_manager_set_sd_card_ready(true);
  }

  state_manager_set_uart_comm_ready(true);

  // RTCの状態確認と初期設定
  RTC_TimeTypeDef current_time;
  RTC_DateTypeDef current_date;
  HAL_RTC_GetTime(&hrtc, &current_time, RTC_FORMAT_BIN);
  HAL_RTC_GetDate(&hrtc, &current_date, RTC_FORMAT_BIN);

  // RTCが初期化されていない場合、デフォルト値を設定
  if (current_date.Year == 0 || current_date.Month == 0 || current_date.Date == 0)
  {
    RTC_TimeTypeDef default_time = {0};
    RTC_DateTypeDef default_date = {0};

    default_time.Hours = 12;
    default_time.Minutes = 0;
    default_time.Seconds = 0;
    default_time.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    default_time.StoreOperation = RTC_STOREOPERATION_RESET;

    default_date.WeekDay = RTC_WEEKDAY_MONDAY;
    default_date.Month = 1;
    default_date.Date = 1;
    default_date.Year = 25; // 2025年

    HAL_RTC_SetTime(&hrtc, &default_time, RTC_FORMAT_BIN);
    HAL_RTC_SetDate(&hrtc, &default_date, RTC_FORMAT_BIN);
  }

  // SDカードの基本動作テスト
  FRESULT test_result = f_open(&fil, "test.txt", FA_CREATE_ALWAYS | FA_WRITE);
  if (test_result == FR_OK)
  {
    char buffer[64]; // ローカルバッファ
    sprintf(buffer, "SDカードテスト成功\r\n");
    f_write(&fil, buffer, strlen(buffer), &bw);
    f_close(&fil);

    // テストファイルを削除
    f_unlink("test.txt");
  }

  // ファイル名生成テスト
  char test_filename[32];
  SD_get_datetime_filename(test_filename, sizeof(test_filename), &hrtc);
  printf("生成されたファイル名: %s\r\n", test_filename);

  // 8.3形式の検証（ddhhmmss.csv = 12文字）
  if (strlen(test_filename) <= 12) // "filename.ext" = 最大12文字
  {
    printf("ファイル名は8.3形式に適合しています\r\n");
  }
  else
  {
    printf("警告: ファイル名が8.3形式の制限を超えています\r\n");
  }

  // Float演算基本テスト
  printf("=== Float演算テスト開始 ===\r\n");
  float test_temp = 25.67f;
  float test_press = 101325.45f;
  char float_buffer[64];

  printf("テスト値: 温度=%.2f°C, 圧力=%.2f Pa\r\n", test_temp, test_press);
  printf("16進表現: 温度=0x%08lX, 圧力=0x%08lX\r\n",
         *(uint32_t *)&test_temp, *(uint32_t *)&test_press);

  sprintf(float_buffer, "%.2f,%.2f", test_temp, test_press);
  printf("sprintf結果: '%s'\r\n", float_buffer);

  // 算術演算テスト
  float result = test_temp + test_press;
  printf("加算テスト: %.2f + %.2f = %.6f\r\n", test_temp, test_press, result);

  // DataBuffer_t構造体への格納テスト
  DataBuffer_t test_buffer;
  test_buffer.timestamp = HAL_GetTick();
  test_buffer.temp_processed_data = test_temp;
  test_buffer.press_processed_data = test_press;

  printf("構造体格納テスト: timestamp=%lu, temp=%.2f, press=%.2f\r\n",
         test_buffer.timestamp, test_buffer.temp_processed_data, test_buffer.press_processed_data);
  printf("=== Float演算テスト完了 ===\r\n");
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
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_TIM3_Init();
  MX_USART2_UART_Init();
  MX_FATFS_Init();
  MX_RTC_Init();
  MX_UART5_Init();
  MX_I2C3_Init();
  /* USER CODE BEGIN 2 */
  system_user_init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    // PA9信号によるNOS電磁弁制御（PA9がHIGHの時にOPEN）
    nos_solenoid_control(GPIOA, GPIO_PIN_11);

    // PA10信号によるサーボバルブ制御（PA10がHIGHになったら30秒間OPEN）
    const ValveControl_t *valve_state = state_manager_get_valve_state();
    ValveControl_t current_valve_state = *valve_state; // コピーして操作
    servo_valve_control(&htim3, TIM_CHANNEL_1, &current_valve_state);
    state_manager_set_valve_state(&current_valve_state);

    // センサーデータ読み取り処理（1秒間隔）
    uint32_t current_time = HAL_GetTick();
    if (current_time - last_sensor_read_time >= 1000) // 1秒間隔
    {
      last_sensor_read_time = current_time;

      // センサーデータ読み取りと検証
      SensorValidationResult_t sensor_result = sensor_read_and_validate(&hspi1, &hi2c1);

      if (sensor_result.is_valid)
      {
        // 有効なデータをバッファに追加
        data_buffer[data_buffer_index].timestamp = current_time;
        data_buffer[data_buffer_index].temp_processed_data = sensor_result.temperature;
        data_buffer[data_buffer_index].press_processed_data = sensor_result.pressure;

        // デバッグ: センサー値の詳細出力
        printf("センサー値詳細:\r\n");
        printf("  - 温度: raw=%.6f, formatted=%.2f, hex=0x%08lX\r\n",
               sensor_result.temperature, sensor_result.temperature,
               *(uint32_t *)&sensor_result.temperature);
        printf("  - 圧力: raw=%.6f, formatted=%.2f, hex=0x%08lX\r\n",
               sensor_result.pressure, sensor_result.pressure,
               *(uint32_t *)&sensor_result.pressure);
        printf("  - バッファ格納後: temp=%.2f (0x%08lX), press=%.2f (0x%08lX)\r\n",
               data_buffer[data_buffer_index].temp_processed_data,
               *(uint32_t *)&data_buffer[data_buffer_index].temp_processed_data,
               data_buffer[data_buffer_index].press_processed_data,
               *(uint32_t *)&data_buffer[data_buffer_index].press_processed_data);

        data_buffer_index++;

        printf("センサーデータ取得: 温度=%.2f°C, 圧力=%.2f Pa (バッファ: %lu/%d)\r\n",
               sensor_result.temperature, sensor_result.pressure,
               data_buffer_index, MAX_DATA_POINTS);

        // バッファが満杯の場合、SDカードに保存
        if (data_buffer_index >= MAX_DATA_POINTS)
        {
          printf("バッファ満杯のため、SDカードに保存開始\r\n");
          app_error_t save_result = data_save_to_sd(data_buffer, data_buffer_index, &hrtc);

          if (save_result == APP_ERROR_NONE)
          {
            printf("SDカード保存成功: %lu件のデータ\r\n", data_buffer_index);
            data_buffer_index = 0; // バッファリセット
            last_data_save_time = current_time;
          }
          else
          {
            printf("SDカード保存失敗: エラーコード=%d\r\n", save_result);
          }
        }
      }
      else
      {
        printf("センサーデータ無効: エラーコード=%d\r\n", sensor_result.validation_error);
      }
    }

    // 定期保存処理（5分間隔、バッファにデータがある場合）
    if (data_buffer_index > 0 && (current_time - last_data_save_time >= 300000)) // 5分間隔
    {
      printf("定期保存開始: %lu件のデータ\r\n", data_buffer_index);
      app_error_t save_result = data_save_to_sd(data_buffer, data_buffer_index, &hrtc);

      if (save_result == APP_ERROR_NONE)
      {
        printf("定期保存成功: %lu件のデータ\r\n", data_buffer_index);
        data_buffer_index = 0; // バッファリセット
        last_data_save_time = current_time;
      }
      else
      {
        printf("定期保存失敗: エラーコード=%d\r\n", save_result);
      }
    }

    // メインループ遅延
    HAL_Delay(10);

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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
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
 * @brief I2C3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C3_Init(void)
{

  /* USER CODE BEGIN I2C3_Init 0 */

  /* USER CODE END I2C3_Init 0 */

  /* USER CODE BEGIN I2C3_Init 1 */

  /* USER CODE END I2C3_Init 1 */
  hi2c3.Instance = I2C3;
  hi2c3.Init.ClockSpeed = 100000;
  hi2c3.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c3.Init.OwnAddress1 = 0;
  hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c3.Init.OwnAddress2 = 0;
  hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C3_Init 2 */

  /* USER CODE END I2C3_Init 2 */
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
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */
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
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
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
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
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
  htim3.Init.Prescaler = 9;
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
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4 | GPIO_PIN_9 | GPIO_PIN_10, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12 | GPIO_PIN_3, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA4 PA9 PA10 */
  GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_9 | GPIO_PIN_10;
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
