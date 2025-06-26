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
#include "config.h"
#include <stdio.h>

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
// TIM3設定値（main.cのMX_TIM3_Init()から取得）
#define TIM3_PRESCALER 9         // Prescaler = 9
#define TIM3_PERIOD 15999        // Period = 15999
#define TIM3_CLOCK_FREQ 16000000 // AHB = 16MHz (現在の設定に合わせる)

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
 * @param  htim: タイマーハンドル
 * @param  channel: PWMチャンネル
 * @retval None
 */
void servo_init(TIM_HandleTypeDef *htim, uint32_t channel)
{
  // サーボ電源制御 (PB3をHIGHにしてサーボに電源供給)
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
  printf("サーボ電源ON (PB3 = HIGH)\r\n");

  // PWM設定情報を表示
  uint32_t timer_freq = TIM3_CLOCK_FREQ / (TIM3_PRESCALER + 1);
  uint32_t pwm_freq = timer_freq / (TIM3_PERIOD + 1);
  uint32_t period_ms = 1000 / pwm_freq;

  printf("PWM設定情報:\r\n");
  printf("  プリスケーラー: %d\r\n", TIM3_PRESCALER);
  printf("  ピリオド: %d\r\n", TIM3_PERIOD);
  printf("  タイマー周波数: %lu Hz\r\n", timer_freq);
  printf("  PWM周波数: %lu Hz (%lu ms周期)\r\n", pwm_freq, period_ms);

  // タイマーを明示的に有効化
  __HAL_TIM_ENABLE(htim);
  printf("TIM3タイマー有効化\r\n");

  // PWM出力を開始
  HAL_StatusTypeDef pwm_status = HAL_TIM_PWM_Start(htim, channel);

  if (pwm_status == HAL_OK)
  {
    printf("サーボ初期化: PWM開始成功\r\n");
  }
  else
  {
    printf("サーボ初期化: PWM開始失敗 (ステータス: %d)\r\n", pwm_status);
  }

  // タイマーレジスタの状態を確認
  printf("TIM3レジスタ確認:\r\n");
  printf("  CR1: 0x%08lX\r\n", htim->Instance->CR1);
  printf("  PSC: %lu\r\n", htim->Instance->PSC);
  printf("  ARR: %lu\r\n", htim->Instance->ARR);
  printf("  CCR1: %lu\r\n", htim->Instance->CCR1);
  printf("  CCER: 0x%08lX\r\n", htim->Instance->CCER);

  // 初期位置を0度（バルブクローズ位置）に設定
  servo_set_angle(htim, channel, SERVO_CLOSE_ANGLE);

  printf("サーボ初期化完了: 初期角度 %d度に設定\r\n", SERVO_CLOSE_ANGLE);

  // テスト用：50%デューティ比で固定PWMを出力
  uint32_t test_compare = (TIM3_PERIOD + 1) / 2; // 50%デューティ比
  __HAL_TIM_SetCompare(htim, channel, test_compare);
  // GPIO設定確認（PB4の設定を確認）
  printf("PB4 GPIO設定確認:\r\n");
  printf("  MODER: 0x%08lX (PB4 = bits[9:8])\r\n", GPIOB->MODER);
  printf("  OSPEEDR: 0x%08lX (PB4 = bits[9:8])\r\n", GPIOB->OSPEEDR);
  printf("  AFRL: 0x%08lX (PB4 = bits[19:16])\r\n", GPIOB->AFR[0]);
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

  // デバッグ出力：PWM設定値を確認
  printf("サーボ角度設定: %d度 -> パルス幅: %luμs -> コンペア値: %lu\r\n",
         angle, pulse_us, compare_value);

  // PB3の状態も確認
  GPIO_PinState pb3_state = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3);
  printf("PB3状態: %s\r\n", (pb3_state == GPIO_PIN_SET) ? "HIGH (電源ON)" : "LOW (電源OFF)");

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

/**
 * @brief チャタリング防止付きボタン読み取り
 * @param button_port GPIOポート
 * @param button_pin GPIOピン
 * @param button_pressed_last 前回のボタン状態（参照渡し）
 * @retval ボタンが押されたかどうか
 */
bool read_button_with_debounce(GPIO_TypeDef *button_port, uint16_t button_pin,
                               bool *button_pressed_last)
{
  static uint32_t last_read_time = 0;
  uint32_t current_time = HAL_GetTick();

  // デバウンス時間をチェック
  if (current_time - last_read_time < BUTTON_DEBOUNCE_TIME_MS)
  {
    return false;
  }

  // ボタン状態を読み取り（プルアップなので押下時はLOW）
  bool button_current = (HAL_GPIO_ReadPin(button_port, button_pin) == GPIO_PIN_RESET);

  // 前回の状態と比較して立ち上がりエッジを検出
  if (button_current && !(*button_pressed_last))
  {
    last_read_time = current_time;
    *button_pressed_last = button_current;
    return true;
  }

  *button_pressed_last = button_current;
  return false;
}

/**
 * @brief バルブ制御処理
 * @param htim タイマーハンドル
 * @param channel PWMチャンネル
 * @param valve_state バルブ状態管理構造体
 * @param nos_port NOSバルブのGPIOポート
 * @param nos_pin NOSバルブのGPIOピン
 * @param button_port ボタンのGPIOポート
 * @param button_pin ボタンのGPIOピン
 */
void valve_control_process(TIM_HandleTypeDef *htim, uint32_t channel,
                           ValveControl_t *valve_state,
                           GPIO_TypeDef *nos_port, uint16_t nos_pin,
                           GPIO_TypeDef *button_port, uint16_t button_pin)
{
  uint32_t current_time = HAL_GetTick();

  // バルブ動作中の処理
  if (valve_state->valve_operation_active)
  {
    // 30秒経過をチェック
    if (current_time - valve_state->valve_operation_start_time >= SERVO_OPEN_TIME_MS)
    {
      // バルブを閉じる
      servo_close_valve(htim, channel);

      // NOS バルブも閉じる
      HAL_GPIO_WritePin(nos_port, nos_pin, GPIO_PIN_RESET);

      valve_state->valve_operation_active = false;
    }
    return; // 動作中は新しい入力を受け付けない
  }

  // ボタン入力チェック
  if (read_button_with_debounce(button_port, button_pin, &valve_state->button_pressed_last))
  {
    // バルブを開く
    servo_open_valve(htim, channel);

    // NOS バルブも開く
    HAL_GPIO_WritePin(nos_port, nos_pin, GPIO_PIN_SET);

    valve_state->valve_operation_start_time = current_time;
    valve_state->valve_operation_active = true;

    printf("Valve opened - will close in 30 seconds\r\n");
  }
}