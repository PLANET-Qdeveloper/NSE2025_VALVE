/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : comm_manager.c
 * @brief          : Communication Management Functions Implementation
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
#include "comm_manager.h"
#include "app_types.h"
#include <stdio.h>

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/**
 * @brief CommData_tをUART経由で送信
 * @param data 送信するCommData_t構造体のポインタ
 * @param huart UARTハンドル
 * @retval app_error_t エラーコード
 */
app_error_t comm_send_data_via_uart(const CommData_t *data, UART_HandleTypeDef *huart)
{
    // パラメータ検証
    APP_ERROR_CHECK(data != NULL, APP_ERROR_INVALID_PARAM, "data is NULL");
    APP_ERROR_CHECK(huart != NULL, APP_ERROR_INVALID_PARAM, "huart is NULL");

    // UART経由でパケットを送信
    APP_HAL_CHECK(HAL_UART_Transmit(huart, (uint8_t *)data, sizeof(CommData_t), UART_TIMEOUT_MS),
                  "UART transmission failed");

    return APP_ERROR_NONE;
}

/**
 * @brief センサーデータを通信用データに変換
 * @param temperature 温度データ（生データ）
 * @param pressure 圧力データ（生データ）
 * @param comm_data 通信用データ構造体
 */
void comm_convert_sensor_data(int16_t temperature, uint16_t pressure, CommData_t *comm_data)
{
    if (comm_data != NULL)
    {
        comm_data->temperature = temperature;
        comm_data->pressure = pressure;
    }
}