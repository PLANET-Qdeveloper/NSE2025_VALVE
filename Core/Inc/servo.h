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

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
#define SERVO_MIN_PULSE_US 1000 // 最小パルス幅（マイクロ秒）
#define SERVO_MAX_PULSE_US 2000 // 最大パルス幅（マイクロ秒）
#define SERVO_MIN_ANGLE 0       // 最小角度（度）
#define SERVO_MAX_ANGLE 270     // 最大角度（度）

    /* Exported macro ------------------------------------------------------------*/

    /* Exported functions prototypes ---------------------------------------------*/
    void servo_init(void);
    void servo_set_angle(uint16_t angle);
    void servo_move_to_angle_smooth(uint16_t target_angle, uint32_t step_delay);
    uint32_t servo_compute_pulse_us_from_angle(uint16_t angle);
    uint32_t servo_compute_compare_from_us(uint32_t pulse_us);

#ifdef __cplusplus
}
#endif

#endif /* __SERVO_H */