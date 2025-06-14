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
#include "MAX6675.h"
#include "stm32f4xx_hal_rtc.h"
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
I2C_HandleTypeDef hi2c2;
I2C_HandleTypeDef hi2c3;

RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;
SPI_HandleTypeDef hspi3;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart5;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
volatile uint8_t Timer1, Timer2; /* 1ms Timer Counter for SD card operations */
char buffer[1024];               /* Buffer for file operations */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2C2_Init(void);
static void MX_I2C3_Init(void);
static void MX_SPI1_Init(void);
static void MX_SPI2_Init(void);
static void MX_SPI3_Init(void);
static void MX_TIM3_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
static void MX_RTC_Init(void);
static void MX_UART5_Init(void);
/* USER CODE BEGIN PFP */
int _write(int file, char *ptr, int len);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
FATFS fs;
FIL fil;
FRESULT fresult;
UINT br, bw;
unsigned int servo_rise = 0;
FATFS *pfs;
uint32_t fre_clust, tot_size, fre_size;

int _write(int file, char *ptr, int len)
{
  (void)file; // 未使用パラメータの警告を抑制
  HAL_UART_Transmit(&huart5, (uint8_t *)ptr, len, HAL_MAX_DELAY);
  return len;
}

void debug_sector_content(DWORD sector)
{
  BYTE sector_buffer[512];
  FRESULT res;

  printf("\r\n=== Sector %lu Debug Info ===\r\n", sector);

  // ファイルを開く
  res = f_open(&fil, "0:/", FA_READ);
  if (res != FR_OK)
  {
    printf("Error opening file: %d\r\n", res);
    return;
  }

  // 指定されたセクタに移動
  res = f_lseek(&fil, sector * 512);
  if (res != FR_OK)
  {
    printf("Error seeking to sector %lu: %d\r\n", sector, res);
    f_close(&fil);
    return;
  }

  // セクタを読み込む
  res = f_read(&fil, sector_buffer, 512, &br);
  if (res != FR_OK || br != 512)
  {
    printf("Error reading sector %lu: %d\r\n", sector, res);
    f_close(&fil);
    return;
  }

  f_close(&fil);

  // ブートシグネチャチェック (0x55AA at offset 510-511)
  WORD signature = (sector_buffer[511] << 8) | sector_buffer[510];
  printf("Boot signature (0x55AA): 0x%04X %s\r\n", signature,
         (signature == 0xAA55) ? "OK" : "INVALID");

  // ジャンプ命令チェック
  printf("Jump instruction: 0x%02X 0x%02X 0x%02X\r\n",
         sector_buffer[0], sector_buffer[1], sector_buffer[2]);

  // ファイルシステムタイプ文字列チェック (offset 54)
  printf("FS Type (54): ");
  for (int i = 54; i < 62; i++)
  {
    printf("%c", (sector_buffer[i] >= 32 && sector_buffer[i] < 127) ? sector_buffer[i] : '.');
  }
  printf("\r\n");

  // FAT32ファイルシステムタイプ文字列チェック (offset 82)
  printf("FS Type32 (82): ");
  for (int i = 82; i < 90; i++)
  {
    printf("%c", (sector_buffer[i] >= 32 && sector_buffer[i] < 127) ? sector_buffer[i] : '.');
  }
  printf("\r\n");

  // MBRパーティションテーブル情報 (sector 0の場合)
  if (sector == 0)
  {
    printf("\r\n--- MBR Partition Table ---\r\n");
    for (int i = 0; i < 4; i++)
    {
      BYTE *pte = sector_buffer + 446 + (i * 16);
      BYTE system_id = pte[4];
      DWORD start_lba = (pte[11] << 24) | (pte[10] << 16) | (pte[9] << 8) | pte[8];
      DWORD size_sectors = (pte[15] << 24) | (pte[14] << 16) | (pte[13] << 8) | pte[12];

      printf("Partition %d: System=0x%02X, Start=%lu, Size=%lu\r\n",
             i + 1, system_id, start_lba, size_sectors);
    }
  }

  // 最初の32バイトをhex dump
  printf("\r\nFirst 32 bytes (hex):\r\n");
  for (int i = 0; i < 32; i++)
  {
    if (i % 16 == 0)
      printf("%04X: ", i);
    printf("%02X ", sector_buffer[i]);
    if (i % 16 == 15)
      printf("\r\n");
  }
  printf("\r\n");
}

// check_fs関数の動作をシミュレートする関数
BYTE debug_check_fs(DWORD sector)
{
  BYTE sector_buffer[512];
  FRESULT res;
  FIL fil;
  UINT br;

  // ファイルを開く
  res = f_open(&fil, "0:/", FA_READ);
  if (res != FR_OK)
  {
    printf("Error opening file: %d\r\n", res);
    return 4; // Disk error
  }

  // 指定されたセクタに移動
  res = f_lseek(&fil, sector * 512);
  if (res != FR_OK)
  {
    printf("Error seeking to sector %lu: %d\r\n", sector, res);
    f_close(&fil);
    return 4; // Disk error
  }

  // セクタを読み込む
  res = f_read(&fil, sector_buffer, 512, &br);
  if (res != FR_OK || br != 512)
  {
    printf("Error reading sector %lu: %d\r\n", sector, res);
    f_close(&fil);
    return 4; // Disk error
  }

  f_close(&fil);

  // ブートレコードシグネチャチェック
  WORD signature = (sector_buffer[511] << 8) | sector_buffer[510];
  if (signature != 0xAA55)
  {
    printf("Invalid boot signature: 0x%04X\r\n", signature);
    return 3; // Not a BS
  }

  // ジャンプ命令チェック
  if (sector_buffer[0] == 0xE9 || (sector_buffer[0] == 0xEB && sector_buffer[2] == 0x90))
  {
    // FAT文字列チェック (offset 54)
    DWORD fat_str = (sector_buffer[56] << 16) | (sector_buffer[55] << 8) | sector_buffer[54];
    if ((fat_str & 0xFFFFFF) == 0x544146)
    { // "FAT"
      printf("Found FAT filesystem at sector %lu\r\n", sector);
      return 0; // FAT
    }

    // FAT32文字列チェック (offset 82)
    DWORD fat32_str = (sector_buffer[85] << 24) | (sector_buffer[84] << 16) |
                      (sector_buffer[83] << 8) | sector_buffer[82];
    if (fat32_str == 0x33544146)
    { // "FAT3"
      printf("Found FAT32 filesystem at sector %lu\r\n", sector);
      return 0; // FAT
    }
  }

  printf("Valid boot sector but not FAT at sector %lu\r\n", sector);
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
  MX_I2C2_Init();
  MX_I2C3_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_SPI3_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_FATFS_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_RTC_Init();
  MX_UART5_Init();
  /* USER CODE BEGIN 2 */
  // 初期設定
  servo_init();
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);

  // SDカードの詳細デバッグ情報を出力
  printf("\r\n=== SD Card Debug Analysis ===\r\n");

  // SDカード初期化状態確認
  DSTATUS sd_status = SD_disk_status(0);
  printf("SD Status: 0x%02X\r\n", sd_status);

  if (!(sd_status & STA_NOINIT))
  {
    // セクター0 (MBR) の内容を確認
    debug_sector_content(0);

    // check_fs関数の動作をシミュレート
    printf("\r\n=== check_fs simulation ===\r\n");
    BYTE fmt0 = debug_check_fs(0);
    printf("check_fs(0) result: %d\r\n", fmt0);

    // パーティションテーブルから各パーティションの開始セクターを確認
    BYTE sector_buffer[512];
    if (SD_disk_read(0, sector_buffer, 0, 1) == RES_OK)
    {
      printf("\r\n=== Checking partitions ===\r\n");
      for (int i = 0; i < 4; i++)
      {
        BYTE *pte = sector_buffer + 446 + (i * 16);
        BYTE system_id = pte[4];
        DWORD start_lba = (pte[11] << 24) | (pte[10] << 16) | (pte[9] << 8) | pte[8];

        if (system_id != 0 && start_lba != 0)
        {
          printf("Checking partition %d at sector %lu:\r\n", i + 1, start_lba);
          debug_sector_content(start_lba);
          BYTE fmt_part = debug_check_fs(start_lba);
          printf("check_fs(%lu) result: %d\r\n", start_lba, fmt_part);
        }
      }
    }
  }
  else
  {
    printf("SD card not initialized properly\r\n");
  }

  printf("\r\n=== End SD Debug ===\r\n");

  // SDカードを明示的に初期化
  printf("Initializing SD card...\r\n");
  if (SD_disk_initialize(0) != 0)
  {
    printf("SD card initialization failed\r\n");
    Error_Handler();
  }
  printf("SD card initialized successfully\r\n");

  // FATFSのマウント
  printf("Mounting FATFS...\r\n");
  FRESULT mount_result = f_mount(&fs, "/", 0);
  if (mount_result != FR_OK)
  {
    printf("FATFS mount failed with error: %d\r\n", mount_result);
    Error_Handler();
  }
  printf("FATFS mounted successfully\r\n");

  // 空き容量の取得
  if (f_getfree("", &fre_clust, &pfs) == FR_OK)
  {
    tot_size = (pfs->n_fatent - 2) * pfs->csize * 0.5;
    fre_size = fre_clust * pfs->csize * 0.5;
    printf("Total size: %lu KB, Free size: %lu KB\r\n", tot_size, fre_size);
  }
  else
  {
    printf("Error getting free space\r\n");
    Error_Handler();
  }

  // テストファイルの作成と書き込み
  if (f_open(&fil, "test.txt", FA_CREATE_ALWAYS | FA_WRITE) == FR_OK)
  {
    const char *test_data = "SD Card Test\r\n";
    f_write(&fil, test_data, strlen(test_data), &bw);
    f_close(&fil);
    printf("Test file created successfully\r\n");
  }
  else
  {
    printf("Error creating test file\r\n");
    Error_Handler();
  }

  // テストファイルの読み込み
  if (f_open(&fil, "test.txt", FA_READ) == FR_OK)
  {
    f_read(&fil, buffer, f_size(&fil), &br);
    buffer[br] = '\0';
    printf("Read from test file: %s", buffer);
    f_close(&fil);
  }
  else
  {
    printf("Error reading test file\r\n");
    Error_Handler();
  }

  printf("\r\n=== SD Card Setup Complete ===\r\n");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    MAX6675_Temperature_t temp = MAX6675_ReadTemperature();
    FIL dataFile;
    UINT bytesWritten;
    float actual_temp = temp.integer + temp.decimal * 0.25f;
    RTC_TimeTypeDef time;
    RTC_DateTypeDef date;
    char dataBuffer[64];

    // 現在時刻を取得
    HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);

    // ファイル名を生成（YYYYMMDD_temperature.csv形式）
    char fileName[32];
    sprintf(fileName, "%04d%02d%02d_temperature.csv",
            date.Year + 2000, date.Month, date.Date);
    sprintf(dataBuffer, "%02d%02d%02d_%02d,%.02f\r\n",
            date.Month, date.Date,
            time.Hours, time.Minutes,
            actual_temp);

    // デバッグ表示
    printf("温度: %d.%d (0x%04X 0x%02X) = %.2f度\r\n",
           temp.integer, temp.decimal,
           (uint16_t)temp.integer, temp.decimal,
           actual_temp);

    // SDカードにデータを追記モードで保存
    if (f_open(&dataFile, fileName, FA_OPEN_ALWAYS | FA_WRITE) == FR_OK)
    {
      // ファイルが新規作成された場合（サイズが0の場合）はヘッダーを書き込む
      if (f_size(&dataFile) == 0)
      {
        const char *header = "Timestamp,Temperature(℃)\r\n";
        f_write(&dataFile, header, strlen(header), &bytesWritten);
      }
      // ファイルの末尾に移動
      f_lseek(&dataFile, f_size(&dataFile));
      // データを書き込み
      f_write(&dataFile, dataBuffer, strlen(dataBuffer), &bytesWritten);
      f_close(&dataFile);
    }

    // 1秒待機
    HAL_Delay(1000);

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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
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
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

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
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
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
  hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi3.Init.NSS = SPI_NSS_SOFT;
  hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi3.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI3_Init 2 */

  /* USER CODE END SPI3_Init 2 */

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
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 4294967295;
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

  /*Configure GPIO pin : PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

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
     ex: printf("Wrong parameters value: file %s on line %d\r\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
