/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : servo.h
 * @brief          : Header for servo.c file.
 *                   This file contains the servo control functions.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SERVO_H
#define __SERVO_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdbool.h> // bool型のサポート

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
// サーボモーター PWM パルス幅設定
#define SERVO_MIN_PULSE_US 1000 // 最小パルス幅（マイクロ秒）- 0度位置
#define SERVO_MAX_PULSE_US 2000 // 最大パルス幅（マイクロ秒）- 270度位置

// サーボモーター角度設定
#define SERVO_MIN_ANGLE 0   // 最小角度（度）
#define SERVO_MAX_ANGLE 270 // 最大角度（度）

// バルブ制御用角度定義
#define SERVO_OPEN_ANGLE 245 // バルブ開放角度（度）
#define SERVO_CLOSE_ANGLE 0  // バルブ閉鎖角度（度）

// バルブ動作時間
#define SERVO_OPEN_TIME_MS 30000 // バルブ開放時間（30秒）

    /* Exported macro ------------------------------------------------------------*/

    /* Exported functions prototypes ---------------------------------------------*/

    // 基本制御関数
    void servo_init(TIM_HandleTypeDef *htim, uint32_t channel);
    void servo_set_angle(TIM_HandleTypeDef *htim, uint32_t channel, uint16_t angle);
    uint16_t servo_get_current_angle(void);

    // バルブ制御専用関数
    void servo_open_valve(TIM_HandleTypeDef *htim, uint32_t channel);
    void servo_close_valve(TIM_HandleTypeDef *htim, uint32_t channel);

#ifdef __cplusplus
}
#endif

#endif /* __SERVO_H */