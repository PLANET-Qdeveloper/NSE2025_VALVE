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
#include <string.h>

/* Private variables ---------------------------------------------------------*/
static uint8_t uart_tx_buffer[MAX_UART_BUFFER_SIZE];

/* Private function prototypes -----------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/**
 * @brief 通信プロトコルの初期化
 * @retval app_error_t エラーコード
 */
app_error_t comm_init(void)
{
    // シンプルな初期化（バッファクリア）
    memset(uart_tx_buffer, 0, sizeof(uart_tx_buffer));
    return APP_ERROR_NONE;
}

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

    // シンプルなJSON形式でデータを送信
    int len = snprintf((char *)uart_tx_buffer, sizeof(uart_tx_buffer),
                       "{\"temperature\":%d,\"pressure\":%u}\r\n",
                       data->temperature, data->pressure);

    if (len < 0 || len >= (int)sizeof(uart_tx_buffer))
    {
        return APP_ERROR_BUFFER_TOO_SMALL;
    }

    // UART経由でデータを送信
    APP_HAL_CHECK(HAL_UART_Transmit(huart, uart_tx_buffer, (uint16_t)len, UART_TIMEOUT_MS),
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