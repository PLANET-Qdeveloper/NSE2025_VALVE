/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : servo.c
 * @brief          : Servo control functions
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
#include "valve.h"
#include "main.h"

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
// サーボ角度定義
#define SERVO_MIN_PULSE_US 500  // 最小パルス幅（マイクロ秒）- 0度位置
#define SERVO_MAX_PULSE_US 2500 // 最大パルス幅（マイクロ秒）- 270度位置
#define SERVO_MIN_ANGLE 0       // 最小角度（度）
#define SERVO_MAX_ANGLE 270     // 最大角度（度）
#define SERVO_OPEN_ANGLE 245    // バルブ開放角度（度）
#define SERVO_CLOSE_ANGLE 0     // バルブ閉鎖角度（度）

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim3;
/* Private function prototypes -----------------------------------------------*/
static uint32_t compute_pulse_us_from_angle(uint16_t angle);
static uint32_t compute_compare_from_us(uint32_t pulse_us);

/* Private user code ---------------------------------------------------------*/

/**
 * @brief  角度からパルス幅（マイクロ秒）を計算（内部関数）
 * @param  angle: 角度（0-270度）
 * @retval パルス幅（マイクロ秒）
 */
static uint32_t compute_pulse_us_from_angle(uint16_t angle)
{
  // 角度の範囲チェック
  if (angle > SERVO_MAX_ANGLE)
  {
    angle = SERVO_MAX_ANGLE;
  }

  // 線形補間でパルス幅を計算
  // 0度 → SERVO_MIN_PULSE_US、SERVO_MAX_ANGLE度 → SERVO_MAX_PULSE_US
  uint32_t pulse_us = SERVO_MIN_PULSE_US +
                      ((uint32_t)angle * (SERVO_MAX_PULSE_US - SERVO_MIN_PULSE_US)) / SERVO_MAX_ANGLE;

  return pulse_us;
}

/**
 * @brief  パルス幅（マイクロ秒）からタイマーコンペア値を計算（内部関数）
 * @param  pulse_us: パルス幅（マイクロ秒）
 * @retval タイマーコンペア値
 */
static uint32_t compute_compare_from_us(uint32_t pulse_us)
{
  // 動作していた計算方法に戻す
  // Compare_Value = pulse_us * 8 / 10 = pulse_us * 0.8
  uint32_t compare_value = ((uint32_t)pulse_us * 8) / 10;

// クランプ処理 (500us = 400, 2500us = 2000)
#define COMPARE_MIN 400
#define COMPARE_MAX 2000
  if (compare_value < COMPARE_MIN)
  {
    compare_value = COMPARE_MIN;
  }
  else if (compare_value > COMPARE_MAX)
  {
    compare_value = COMPARE_MAX;
  }

  return compare_value;
}

/**
 * @brief  サーボモーターの初期化
 * @retval None
 */
void servo_init(ServoControl_t *servo_state)
{
  __HAL_TIM_ENABLE(&htim3);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  servo_close(servo_state);
}

void solenoid_init(SolenoidControl_t *solenoid_state)
{
  solenoid_close(solenoid_state);
}

/**
 * @brief  バルブを開く
 * @retval None
 */
void servo_open()
{
  // 角度からパルス幅を計算
  uint32_t pulse_us = compute_pulse_us_from_angle(SERVO_OPEN_ANGLE);
  uint32_t compare_value = compute_compare_from_us(pulse_us);

  __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, compare_value);
}

/**
 * @brief  バルブを閉じる
 * @retval None
 */
void servo_close()
{
  uint32_t pulse_us = compute_pulse_us_from_angle(SERVO_CLOSE_ANGLE);
  uint32_t compare_value = compute_compare_from_us(pulse_us);

  __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, compare_value);
}

void solenoid_open()
{
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET); // NOS電磁弁OPEN
}

void solenoid_close()
{
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET); // NOS電磁弁CLOSE
}