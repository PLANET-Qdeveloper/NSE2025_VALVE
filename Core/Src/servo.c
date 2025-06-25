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
#include "servo.h"
#include "main.h"

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
// TIM3設定値（main.cのMX_TIM3_Init()から取得）
#define TIM3_PRESCALER 9         // Prescaler = 9
#define TIM3_PERIOD 15999        // Period = 15999
#define TIM3_CLOCK_FREQ 16000000 // HSI = 16MHz

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static uint16_t current_servo_angle = 0; // 現在のサーボ角度を保存

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
  // タイマー周波数の計算
  // Timer frequency = TIM3_CLOCK_FREQ / (TIM3_PRESCALER + 1)
  uint32_t timer_freq = TIM3_CLOCK_FREQ / (TIM3_PRESCALER + 1);

  // コンペア値の計算
  // Compare = (pulse_us * timer_freq) / 1000000
  uint32_t compare_value = (pulse_us * timer_freq) / 1000000;

  return compare_value;
}

/**
 * @brief  サーボモーターの初期化
 * @param  htim: タイマーハンドル
 * @param  channel: PWMチャンネル
 * @retval None
 */
void servo_init(TIM_HandleTypeDef *htim, uint32_t channel)
{
  // PWM出力を開始
  HAL_TIM_PWM_Start(htim, channel);

  // 初期位置を0度（バルブクローズ位置）に設定
  servo_set_angle(htim, channel, SERVO_CLOSE_ANGLE);
}

/**
 * @brief  サーボモーターの角度を設定
 * @param  htim: タイマーハンドル
 * @param  channel: PWMチャンネル
 * @param  angle: 設定する角度（0-270度）
 * @retval None
 */
void servo_set_angle(TIM_HandleTypeDef *htim, uint32_t channel, uint16_t angle)
{
  // 角度の範囲チェック
  if (angle > SERVO_MAX_ANGLE)
  {
    angle = SERVO_MAX_ANGLE;
  }

  // 角度からパルス幅を計算
  uint32_t pulse_us = compute_pulse_us_from_angle(angle);

  // パルス幅からコンペア値を計算
  uint32_t compare_value = compute_compare_from_us(pulse_us);

  // タイマーのコンペア値を設定
  __HAL_TIM_SetCompare(htim, channel, compare_value);

  // 現在の角度を更新
  current_servo_angle = angle;
}

/**
 * @brief  現在のサーボ角度を取得
 * @retval 現在の角度（度）
 */
uint16_t servo_get_current_angle(void)
{
  return current_servo_angle;
}

/**
 * @brief  バルブを開く
 * @param  htim: タイマーハンドル
 * @param  channel: PWMチャンネル
 * @retval None
 */
void servo_open_valve(TIM_HandleTypeDef *htim, uint32_t channel)
{
  servo_set_angle(htim, channel, SERVO_OPEN_ANGLE);
}

/**
 * @brief  バルブを閉じる
 * @param  htim: タイマーハンドル
 * @param  channel: PWMチャンネル
 * @retval None
 */
void servo_close_valve(TIM_HandleTypeDef *htim, uint32_t channel)
{
  servo_set_angle(htim, channel, SERVO_CLOSE_ANGLE);
}