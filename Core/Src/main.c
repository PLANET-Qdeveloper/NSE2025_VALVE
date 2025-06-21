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
#include <stdio.h>
#include <string.h>
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

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart5;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
volatile uint8_t Timer1, Timer2; /* 1ms Timer Counter for SD card operations */
#define MAX_DATA_POINTS 10
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

// SDカードのセクター内容をデバッグ出力する関数
void debug_sector_content(DWORD sector)
{
  BYTE sector_buffer[512];
  DRESULT res;

  printf("\n=== Sector %lu Debug Info ===\n", sector);

  res = SD_disk_read(0, sector_buffer, sector, 1);
  if (res != RES_OK)
  {
    printf("Error reading sector %lu: %d\n", sector, res);
    return;
  }

  // ブートシグネチャチェック (0x55AA at offset 510-511)
  WORD signature = (sector_buffer[511] << 8) | sector_buffer[510];
  printf("Boot signature (0x55AA): 0x%04X %s\n", signature,
         (signature == 0xAA55) ? "OK" : "INVALID");

  // ジャンプ命令チェック
  printf("Jump instruction: 0x%02X 0x%02X 0x%02X\n",
         sector_buffer[0], sector_buffer[1], sector_buffer[2]);

  // ファイルシステムタイプ文字列チェック (offset 54)
  printf("FS Type (54): ");
  for (int i = 54; i < 62; i++)
  {
    printf("%c", (sector_buffer[i] >= 32 && sector_buffer[i] < 127) ? sector_buffer[i] : '.');
  }
  printf("\n");

  // FAT32ファイルシステムタイプ文字列チェック (offset 82)
  printf("FS Type32 (82): ");
  for (int i = 82; i < 90; i++)
  {
    printf("%c", (sector_buffer[i] >= 32 && sector_buffer[i] < 127) ? sector_buffer[i] : '.');
  }
  printf("\n");

  // MBRパーティションテーブル情報 (sector 0の場合)
  if (sector == 0)
  {
    printf("\n--- MBR Partition Table ---\n");
    for (int i = 0; i < 4; i++)
    {
      BYTE *pte = sector_buffer + 446 + (i * 16);
      BYTE system_id = pte[4];
      DWORD start_lba = (pte[11] << 24) | (pte[10] << 16) | (pte[9] << 8) | pte[8];
      DWORD size_sectors = (pte[15] << 24) | (pte[14] << 16) | (pte[13] << 8) | pte[12];

      printf("Partition %d: System=0x%02X, Start=%lu, Size=%lu\n",
             i + 1, system_id, start_lba, size_sectors);
    }
  }

  // 最初の32バイトをhex dump
  printf("\nFirst 32 bytes (hex):\n");
  for (int i = 0; i < 32; i++)
  {
    if (i % 16 == 0)
      printf("%04X: ", i);
    printf("%02X ", sector_buffer[i]);
    if (i % 16 == 15)
      printf("\n");
  }
  printf("\n");
}

// check_fs関数の動作をシミュレートする関数
BYTE debug_check_fs(DWORD sector)
{
  BYTE sector_buffer[512];
  DRESULT res;

  res = SD_disk_read(0, sector_buffer, sector, 1);
  if (res != RES_OK)
  {
    printf("Disk error reading sector %lu\n", sector);
    return 4; // Disk error
  }

  // ブートレコードシグネチャチェック
  WORD signature = (sector_buffer[511] << 8) | sector_buffer[510];
  if (signature != 0xAA55)
  {
    printf("Invalid boot signature: 0x%04X\n", signature);
    return 3; // Not a BS
  }

  // ジャンプ命令チェック
  if (sector_buffer[0] == 0xE9 || (sector_buffer[0] == 0xEB && sector_buffer[2] == 0x90))
  {
    // FAT文字列チェック (offset 54)
    DWORD fat_str = (sector_buffer[56] << 16) | (sector_buffer[55] << 8) | sector_buffer[54];
    if ((fat_str & 0xFFFFFF) == 0x544146)
    { // "FAT"
      printf("Found FAT filesystem at sector %lu\n", sector);
      return 0; // FAT
    }

    // FAT32文字列チェック (offset 82)
    DWORD fat32_str = (sector_buffer[85] << 24) | (sector_buffer[84] << 16) |
                      (sector_buffer[83] << 8) | sector_buffer[82];
    if (fat32_str == 0x33544146)
    { // "FAT3"
      printf("Found FAT32 filesystem at sector %lu\n", sector);
      return 0; // FAT
    }
  }

  printf("Valid boot sector but not FAT at sector %lu\n", sector);
  return 2; // Valid BS but not FAT
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
  MX_I2C3_Init();
  /* USER CODE BEGIN 2 */
  // 初期設定
  servo_init();

  /*
  //NOS動作用プログラム5秒間開く（動作確認済み）
  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_RESET);
  HAL_Delay(3000);
  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_SET);
  */

  // SDカードの詳細デバッグ情報を出力
  printf("\n=== SD Card Debug Analysis ===\n");

  // SDカード初期化状態確認
  DSTATUS sd_status = SD_disk_status(0);
  printf("SD Status: 0x%02X\n", sd_status);

  if (!(sd_status & STA_NOINIT))
  {
    // セクター0 (MBR) の内容を確認
    debug_sector_content(0);

    // check_fs関数の動作をシミュレート
    printf("\n=== check_fs simulation ===\n");
    BYTE fmt0 = debug_check_fs(0);
    printf("check_fs(0) result: %d\n", fmt0);

    // パーティションテーブルから各パーティションの開始セクターを確認
    BYTE sector_buffer[512];
    if (SD_disk_read(0, sector_buffer, 0, 1) == RES_OK)
    {
      printf("\n=== Checking partitions ===\n");
      for (int i = 0; i < 4; i++)
      {
        BYTE *pte = sector_buffer + 446 + (i * 16);
        BYTE system_id = pte[4];
        DWORD start_lba = (pte[11] << 24) | (pte[10] << 16) | (pte[9] << 8) | pte[8];

        if (system_id != 0 && start_lba != 0)
        {
          printf("Checking partition %d at sector %lu:\n", i + 1, start_lba);
          debug_sector_content(start_lba);
          BYTE fmt_part = debug_check_fs(start_lba);
          printf("check_fs(%lu) result: %d\n", start_lba, fmt_part);
        }
      }
    }
  }
  else
  {
    printf("SD card not initialized properly\n");
  }

  printf("\n=== End SD Debug ===\n");

  // SDカードを明示的に初期化
  printf("Initializing SD card...\n");
  DSTATUS init_result = SD_disk_initialize(0);
  printf("SD initialization result: 0x%02X\n", init_result);

  if (init_result == 0)
  {
    printf("SD card initialized successfully\n");
  }
  else
  {
    printf("SD card initialization failed\n");
  }

  fresult = f_mount(&fs, "/", 0);
  if (fresult == FR_OK)
  {
    printf("SD CARD mounted successfully.\n");
  }
  else
  {
    printf("ERROR : SD CARD mounted failed.\n");
  }

  if (f_getfree("", &fre_clust, &pfs) == FR_OK)
  {
    tot_size = (pfs->n_fatent - 2) * pfs->csize * 0.5;
    printf("Total size: %15lu B\n", tot_size * 1024);
    fre_size = fre_clust * pfs->csize * 0.5;
    printf("Free size: %15lu B\n", fre_size * 1024);
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    float temperature = Max31855_Read_Temp(&hspi2);
    float pressure = MCP3425_Read_Pressure(&hi2c1);

    // データを配列に保存
    if (data_count < MAX_DATA_POINTS)
    {
      sensor_data[data_count].timestamp = HAL_GetTick();
      sensor_data[data_count].temperature = temperature;
      sensor_data[data_count].pressure = pressure;
      data_count++;
    }

    // 10回のデータが集まったら保存
    if (data_count >= MAX_DATA_POINTS)
    {
      // 日時を含むファイル名を生成
      // char filename[32];
      // get_datetime_filename(filename, sizeof(filename));

      // ファイルにセンサーデータを書き込む
      FRESULT res = f_open(&fil, "sensor_data.csv", FA_CREATE_ALWAYS | FA_WRITE);
      printf("ファイルを開けました (エラーコード: %d)\r\n", res);
      if (res == FR_OK)
      {
        // CSVヘッダーを書き込む
        sprintf(buffer, "時刻,温度(°C),圧力(Pa)\r\n");
        if (f_write(&fil, buffer, strlen(buffer), &bw) != FR_OK)
        {
          printf("ヘッダー書き込みエラー\r\n");
        }

        // 10個のデータをCSV形式で書き込む
        for (int i = 0; i < MAX_DATA_POINTS; i++)
        {
          sprintf(buffer, "%lu,%.2f,%.2f\r\n",
                  sensor_data[i].timestamp,
                  sensor_data[i].temperature,
                  sensor_data[i].pressure);

          if (f_write(&fil, buffer, strlen(buffer), &bw) != FR_OK)
          {
            printf("データ書き込みエラー\r\n");
            break;
          }
        }

        // ファイルを閉じる
        f_close(&fil);
        printf("10個のセンサーデータを保存しました\r\n");

        // データカウントをリセット
        data_count = 0;
      }
      else
      {
        printf("ファイルを開けませんでした (エラーコード: %d)\r\n", res);
        // printf("ファイル名: %s\r\n", filename);
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
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }

  // MAX31855用のCSピンを初期状態（HIGH）に設定
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
  printf("SPI2初期化完了 - CSピンをHIGHに設定\r\n");
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
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);

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
// 日時を含むファイル名を生成する関数
void get_datetime_filename(char *filename, size_t max_len)
{
  RTC_TimeTypeDef time;
  RTC_DateTypeDef date;
  char datetime_str[32];

  // RTCから現在の日時を取得
  HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
  HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);

  snprintf(datetime_str, sizeof(datetime_str), "%04d%02d%02d_%02d%02d%02d.csv",
           date.Year + 2000, date.Month, date.Date,
           time.Hours, time.Minutes, time.Seconds);

  strncpy(filename, datetime_str, max_len - 1);
  filename[max_len - 1] = '\0';
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
