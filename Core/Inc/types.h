/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : types.h
 * @brief          : Application Types and Structures
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

#ifndef __TYPES_H
#define __TYPES_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

    /**
     * @brief データバッファ構造体
     */
    typedef struct
    {
        uint32_t timestamp;
        float temp_data;  // 処理済み温度（摂氏）
        float press_data; // 処理済み圧力（Pa）
    } SensorData_t;

    /**
     * @brief 通信データ構造体
     */
    typedef struct
    {
        int16_t temperature;
        uint16_t pressure;
    } CommData_t;

    /**
     * @brief バルブ制御状態管理構造体
     */
    typedef struct
    {
        bool valve_operation_active;
        uint32_t valve_operation_start_time;
    } ServoControl_t;

    typedef struct
    {
        bool solenoid_operation_active;
        uint32_t solenoid_operation_start_time;
    } SolenoidControl_t;

#ifdef __cplusplus
}
#endif

#endif /* __TYPES_H */