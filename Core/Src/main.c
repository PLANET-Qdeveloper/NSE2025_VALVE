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
#include <string.h>
#include <stdio.h>
#include "fatfs_sd.h"
#include "MAX31855.h"
#include "MCP3425.h"
#include "servo.h"
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

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart5;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
volatile uint8_t Timer1, Timer2; /* 1ms Timer Counter for SD card operations */
#define MAX_DATA_POINTS 20
typedef struct
{
  uint32_t timestamp;
  float temperature;
  float pressure;
} SensorData_t;

SensorData_t sensor_data[MAX_DATA_POINTS];
uint16_t data_count = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_SPI1_Init(void);
static void MX_SPI2_Init(void);
static void MX_TIM3_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM1_Init(void);
static void MX_RTC_Init(void);
static void MX_UART5_Init(void);
static void MX_I2C3_Init(void);
/* USER CODE BEGIN PFP */
int _write(int file, char *ptr, int len);
void get_datetime_filename(char *filename, size_t max_len);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
FATFS fs;
FIL fil;
FRESULT fresult;
char buffer[128];
UINT br, bw;
unsigned int servo_rise = 0;
FATFS *pfs;
uint32_t fre_clust, tot_size, fre_size;

// printf関数をUART経由で出力するためのリダイレクト
int _write(int file, char *ptr, int len)
{
  (void)file; // 未使用パラメータの警告を抑制
  HAL_UART_Transmit(&huart5, (uint8_t *)ptr, len, HAL_MAX_DELAY);
  return len;
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
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_FATFS_Init();
  MX_TIM1_Init();
  MX_RTC_Init();
  MX_UART5_Init();
  MX_I2C3_Init(); /* USER CODE BEGIN 2 */
  servo_init();

  // SDカードとファイルシステムの初期化
  printf("SDカード初期化を開始します...\r\n");

  DSTATUS disk_status = SD_disk_initialize(0);
  printf("SDカード初期化結果: %d\r\n", disk_status);

  if (disk_status != 0)
  {
    printf("SDカード初期化エラー - 状態: 0x%02X\r\n", disk_status);
    if (disk_status & STA_NOINIT)
      printf("- 初期化されていません\r\n");
    if (disk_status & STA_NODISK)
      printf("- SDカードが挿入されていません\r\n");
    if (disk_status & STA_PROTECT)
      printf("- 書き込み保護されています\r\n");
  }

  FRESULT mount_result = f_mount(&fs, "", 1);
  printf("ファイルシステムマウント結果: %d\r\n", mount_result);

  if (mount_result != FR_OK)
  {
    printf("マウントエラー詳細:\r\n");
    switch (mount_result)
    {
    case FR_INVALID_DRIVE:
      printf("- 無効なドライブ\r\n");
      break;
    case FR_NOT_READY:
      printf("- ドライブが準備できていません\r\n");
      break;
    case FR_NO_FILESYSTEM:
      printf("- ファイルシステムが見つかりません\r\n");
      break;
    default:
      printf("- その他のエラー: %d\r\n", mount_result);
      break;
    }
    // フォーマットを試行
    printf("SDカードのフォーマットを試行します...\r\n");
    BYTE work[_MAX_SS];
    FRESULT format_result = f_mkfs("", FM_FAT32, 0, work, sizeof(work));
    printf("フォーマット結果: %d\r\n", format_result);

    if (format_result == FR_OK)
    {
      mount_result = f_mount(&fs, "", 1);
      printf("再マウント結果: %d\r\n", mount_result);
    }
  }

  // RTCの状態確認と初期設定
  RTC_TimeTypeDef current_time;
  RTC_DateTypeDef current_date;
  HAL_RTC_GetTime(&hrtc, &current_time, RTC_FORMAT_BIN);
  HAL_RTC_GetDate(&hrtc, &current_date, RTC_FORMAT_BIN);

  printf("現在のRTC設定: %04d/%02d/%02d %02d:%02d:%02d\r\n",
         current_date.Year + 2000, current_date.Month, current_date.Date,
         current_time.Hours, current_time.Minutes, current_time.Seconds);

  // RTCが初期化されていない場合、デフォルト値を設定
  if (current_date.Year == 0 || current_date.Month == 0 || current_date.Date == 0)
  {
    printf("RTCが初期化されていません。デフォルト値を設定します。\r\n");
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
    printf("RTCをデフォルト値に設定しました: 2025/01/01 12:00:00\r\n");
  }

  // SDカードの基本動作テスト
  printf("SDカードの基本動作テストを実行します...\r\n");
  FRESULT test_result = f_open(&fil, "test.txt", FA_CREATE_ALWAYS | FA_WRITE);
  if (test_result == FR_OK)
  {
    sprintf(buffer, "SDカードテスト成功\r\n");
    f_write(&fil, buffer, strlen(buffer), &bw);
    f_close(&fil);
    printf("テストファイル作成成功\r\n");

    // テストファイルを削除
    f_unlink("test.txt");
    printf("テストファイル削除完了\r\n");
  }
  else
  {
    printf("テストファイル作成失敗: %d\r\n", test_result);
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */ while (1)
  {
    float temperature = Max31855_Read_Temp(&hspi2);
    float pressure = MCP3425_Read_Pressure(&hi2c1); // データを配列に保存
    if (data_count < MAX_DATA_POINTS)
    {
      sensor_data[data_count].timestamp = HAL_GetTick();
      sensor_data[data_count].temperature = temperature;
      sensor_data[data_count].pressure = pressure;
      data_count++;
    }
    if (data_count >= MAX_DATA_POINTS)
    {
      // ディスクの状態を確認
      DSTATUS disk_status = SD_disk_status(0);
      printf("ディスクステータス: 0x%02X\r\n", disk_status);

      if (disk_status != 0)
      {
        printf("ディスクエラーが発生しています\r\n");
        if (disk_status & STA_NOINIT)
          printf("- ディスクが初期化されていません\r\n");
        if (disk_status & STA_NODISK)
          printf("- ディスクが見つかりません\r\n");
        if (disk_status & STA_PROTECT)
          printf("- ディスクが書き込み保護されています\r\n");

        // ディスクの再初期化を試行
        printf("ディスクの再初期化を試行します...\r\n");
        DSTATUS reinit_status = SD_disk_initialize(0);
        printf("再初期化結果: 0x%02X\r\n", reinit_status);

        if (reinit_status == 0)
        {
          // ファイルシステムの再マウント
          f_mount(NULL, "", 0); // アンマウント
          FRESULT remount_result = f_mount(&fs, "", 1);
          printf("再マウント結果: %d\r\n", remount_result);
        }
      }

      // 日時ベースのファイル名を生成
      char filename[64];
      get_datetime_filename(filename, sizeof(filename));

      printf("ファイル名: %s (長さ: %d)\r\n", filename, strlen(filename));
      printf("ファイルオープンを試行します...\r\n");

      // まず簡単なファイル名でテスト
      FRESULT res = f_open(&fil, filename, FA_CREATE_ALWAYS | FA_WRITE);
      printf("ファイルオープン結果: %d\r\n", res);

      if (res == FR_OK)
      {
        printf("ファイルオープン成功、データ書き込み開始\r\n");

        sprintf(buffer, "時刻,温度(°C),圧力(Pa)\r\n");
        FRESULT write_result = f_write(&fil, buffer, strlen(buffer), &bw);
        printf("ヘッダー書き込み結果: %d (書き込みバイト数: %d)\r\n", write_result, bw);

        for (int i = 0; i < MAX_DATA_POINTS; i++)
        {
          sprintf(buffer, "%lu,%.2f,%.2f\r\n",
                  sensor_data[i].timestamp,
                  sensor_data[i].temperature,
                  sensor_data[i].pressure);

          write_result = f_write(&fil, buffer, strlen(buffer), &bw);
          if (write_result != FR_OK)
          {
            printf("データ書き込みエラー: %d (行: %d)\r\n", write_result, i);
            break;
          }
        }

        FRESULT close_result = f_close(&fil);
        printf("ファイルクローズ結果: %d\r\n", close_result);

        if (close_result == FR_OK && write_result == FR_OK)
        {
          printf("データファイル作成完了: %s\r\n", filename);
        }

        data_count = 0;
      }
      else
      {
        data_count = 0;
        printf("ファイルオープンエラー: %d\r\n", res);

        // エラーコードの詳細表示
        switch (res)
        {
        case FR_DISK_ERR:
          printf("- ディスクエラー\r\n");
          break;
        case FR_INT_ERR:
          printf("- 内部エラー\r\n");
          break;
        case FR_NOT_READY:
          printf("- ドライブが準備できていません\r\n");
          break;
        case FR_NO_FILE:
          printf("- ファイルが見つかりません\r\n");
          break;
        case FR_NO_PATH:
          printf("- パスが見つかりません\r\n");
          break;
        case FR_INVALID_NAME:
          printf("- 無効なファイル名\r\n");
          break;
        case FR_DENIED:
          printf("- アクセス拒否\r\n");
          break;
        case FR_EXIST:
          printf("- ファイルが既に存在します\r\n");
          break;
        case FR_INVALID_OBJECT:
          printf("- 無効なオブジェクト\r\n");
          break;
        case FR_WRITE_PROTECTED:
          printf("- 書き込み保護\r\n");
          break;
        case FR_INVALID_DRIVE:
          printf("- 無効なドライブ\r\n");
          break;
        case FR_NOT_ENABLED:
          printf("- ワークエリアが設定されていません\r\n");
          break;
        case FR_NO_FILESYSTEM:
          printf("- ファイルシステムなし\r\n");
          break;
        case FR_TIMEOUT:
          printf("- タイムアウト\r\n");
          break;
        default:
          printf("- 不明なエラー\r\n");
          break;
        }
      }
    }

    HAL_Delay(1000); // 1秒待機
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
 * @brief TIM1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
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
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_OC_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_TIMING;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_OC_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
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
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB12 */
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
// 日時を含むファイル名を生成する関数（8.3形式対応）
void get_datetime_filename(char *filename, size_t max_len)
{
  RTC_TimeTypeDef time;
  RTC_DateTypeDef date;
  static uint8_t file_counter = 0; // ファイル番号カウンタ

  // RTCから現在の日時を取得
  HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
  HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);

  // 8.3形式のファイル名を生成（例: DATA001.CSV）
  snprintf(filename, max_len, "D%02d%02d%02d%02d.CSV",
           date.Date, time.Hours, time.Minutes, time.Seconds);

  // ファイル名の重複を避けるためカウンタを追加
  file_counter++;
  if (file_counter > 99)
    file_counter = 1;

  // より短い形式: Dyymmdd.CSV (例: D250623.CSV)
  snprintf(filename, max_len, "D%02d%02d%02d.CSV",
           date.Year, date.Month, date.Date);
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