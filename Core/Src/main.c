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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//モータによって変更する箇所



//変えなくていい箇所
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// 角度からパルス幅(us)を計算する関数
int compute_pulse_us_from_angle(int angle) {
  // 仕様や必要に応じてこれらの値を調整してください
  #define DEG_MIN 0
  #define DEG_MAX 270 // 仕様書では 180 or 270
  #define WIDTH_MIN_US 500
  #define WIDTH_MAX_US 2500
  #define DEG_RANGE (DEG_MAX - DEG_MIN)
  #define WIDTH_RANGE_US (WIDTH_MAX_US - WIDTH_MIN_US)

  int deg_con = angle - DEG_MIN;
  if (deg_con < 0) deg_con = 0;
  if (deg_con > DEG_RANGE) deg_con = DEG_RANGE;

  // 整数演算で計算
  uint32_t pulse_us = ((uint32_t)deg_con * WIDTH_RANGE_US / DEG_RANGE) + WIDTH_MIN_US;

  return pulse_us;
}

// パルス幅(us)からタイマーのコンペア値を計算する関数
int compute_compare_from_us(int pulse_us) {
  // Compare_Value = pulse_us * TIM_Clock / (Prescaler + 1) / 1,000,000
  // Compare_Value = pulse_us * 8,000,000 / (9 + 1) / 1,000,000
  // Compare_Value = pulse_us * 0.8
  // 整数演算 (精度を保つため * 8 / 10 とする)
  uint32_t compare_value = ((uint32_t)pulse_us * 8) / 10;

  // クランプ処理 (500us = 400, 2500us = 2000)
  #define COMPARE_MIN 400
  #define COMPARE_MAX 2000
  if (compare_value < COMPARE_MIN) {
    compare_value = COMPARE_MIN;
  } else if (compare_value > COMPARE_MAX) {
    compare_value = COMPARE_MAX;
  }
  return compare_value;
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
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  __HAL_RCC_TIM2_CLK_ENABLE();
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    // テストコード: 500us から 2500us まで段階的に変化させる
    __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, compute_compare_from_us(500));  // Min pulse
    HAL_Delay(500);
    __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, compute_compare_from_us(1000));
    HAL_Delay(500);
    __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, compute_compare_from_us(1500)); // Neutral pulse
    HAL_Delay(500);
    __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, compute_compare_from_us(2000));
    HAL_Delay(500);
    __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, compute_compare_from_us(2500)); // Max pulse
    HAL_Delay(500);

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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
