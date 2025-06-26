/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : config.h
 * @brief          : Application Configuration Header
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

#ifndef __CONFIG_H
#define __CONFIG_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>

/* Application Configuration --------------------------------------------------*/

/* データバッファ設定 */
#define MAX_DATA_POINTS 20
#define MAX_UART_BUFFER_SIZE 256

/* センサー設定 */
#define SENSOR_VALID_TEMP_MIN -100.0f
#define SENSOR_VALID_TEMP_MAX 1000.0f
#define SENSOR_VALID_PRESSURE_MIN 0.0f
#define SENSOR_VALID_PRESSURE_MAX 200000.0f
#define SENSOR_ERROR_THRESHOLD 5

/* タイミング設定 */
#define MAIN_LOOP_DELAY_MS 1000
#define BUTTON_DEBOUNCE_TIME_MS 100

/* ファイル設定 */
#define MAX_FILENAME_LENGTH 64
#define DATA_WRITE_BUFFER_SIZE 128

/* 通信設定 */
#define UART_TIMEOUT_MS 1000

    /* Note: エラーコードはapp_types.hで定義されています */

#ifdef __cplusplus
}
#endif

#endif /* __CONFIG_H */