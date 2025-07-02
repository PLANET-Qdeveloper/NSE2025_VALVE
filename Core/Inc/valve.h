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
#ifndef __VALVE_H
#define __VALVE_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "types.h"
#include <stdbool.h> // bool型のサポート

    /* Exported types ------------------------------------------------------------*/
    /* ServoControl_t moved to app_types.h for better organization */

/* Exported constants --------------------------------------------------------*/
// バルブ動作時間
#define SERVO_OPEN_TIME_MS 30000 // バルブ開放時間（30秒）

    /* Exported macro ------------------------------------------------------------*/

    /* Exported functions prototypes ---------------------------------------------*/

    void servo_init(ServoControl_t *servo_state);
    void solenoid_init(SolenoidControl_t *solenoid_state);

    void servo_open(ServoControl_t *servo_state);
    void servo_close(ServoControl_t *servo_state);

    void solenoid_open(SolenoidControl_t *solenoid_state);
    void solenoid_close(SolenoidControl_t *solenoid_state);

#ifdef __cplusplus
}
#endif

#endif /* __VALVE_H */