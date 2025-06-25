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

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private user code ---------------------------------------------------------*/

/**
 * @brief  サーボモーターの初期化
 * @retval None
 */
void servo_init(TIM_HandleTypeDef *htim, uint32_t channel)
{
  // タイマークロックを有効化
  __HAL_RCC_TIM3_CLK_ENABLE();

  // PWM出力を開始
  HAL_TIM_PWM_Start(htim, channel);

  // 初期位置を0度に設定
  servo_set_angle(htim, channel, 0);
}

/**
 * @brief  サーボモーターの角度を設定
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
  uint32_t pulse_us = servo_compute_pulse_us_from_angle(
      angle, SERVO_MIN_PULSE_US, SERVO_MAX_PULSE_US, SERVO_MAX_ANGLE);

  // パルス幅からコンペア値を計算
  uint32_t compare_value = servo_compute_compare_from_us(
      pulse_us, 1, 1); // prescaler, periodは実際のタイマー設定値に合わせて修正

  // タイマーのコンペア値を設定
  __HAL_TIM_SetCompare(htim, channel, compare_value);
}

/**
 * @brief  角度からパルス幅（マイクロ秒）を計算
 * @param  angle: 角度（0-270度）
 * @retval パルス幅（マイクロ秒）
 */
uint32_t servo_compute_pulse_us_from_angle(uint16_t angle, uint32_t min_pulse_us, uint32_t max_pulse_us, uint16_t max_angle)
{
  // 角度の範囲チェック
  if (angle > max_angle)
  {
    angle = max_angle;
  }

  // 線形補間でパルス幅を計算
  // 0度 → 1000us, 270度 → 2000us
  uint32_t pulse_us = min_pulse_us +
                      ((uint32_t)angle * (max_pulse_us - min_pulse_us)) / max_angle;

  return pulse_us;
}

/**
 * @brief  パルス幅（マイクロ秒）からタイマーコンペア値を計算
 * @param  pulse_us: パルス幅（マイクロ秒）
 * @retval タイマーコンペア値
 */

/**
 * @brief  サーボモーターを指定角度まで徐々に移動
 * @param  target_angle: 目標角度（0-270度）
 * @param  step_delay: ステップ間の遅延（ミリ秒）
 * @retval None
 */
void servo_move_to_angle_smooth(TIM_HandleTypeDef *htim, uint32_t channel, uint16_t target_angle, uint32_t step_delay)
{
  // 現在の角度を取得（簡易実装のため、前回設定値を保持する変数が必要）
  static uint16_t current_angle = 0;

  // 目標角度の範囲チェック
  if (target_angle > SERVO_MAX_ANGLE)
  {
    target_angle = SERVO_MAX_ANGLE;
  }

  // 現在角度から目標角度まで徐々に移動
  if (current_angle < target_angle)
  {
    // 角度を増加
    for (uint16_t angle = current_angle; angle <= target_angle; angle += 5)
    {
      servo_set_angle(htim, channel, angle);
      HAL_Delay(step_delay);
    }
  }
  else if (current_angle > target_angle)
  {
    // 角度を減少
    for (uint16_t angle = current_angle; angle >= target_angle; angle -= 5)
    {
      servo_set_angle(htim, channel, angle);
      HAL_Delay(step_delay);
      if (angle == 0)
        break; // アンダーフロー防止
    }
  }

  // 最終的に正確な目標角度に設定
  servo_set_angle(htim, channel, target_angle);
  current_angle = target_angle;
}