/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : comm_manager.h
 * @brief          : Communication Management Functions
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

#ifndef __COMM_MANAGER_H
#define __COMM_MANAGER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32f4xx_hal.h"
#include "config.h"
#include "app_types.h"

    /* Exported Functions --------------------------------------------------------*/

    /**
     * @brief 通信プロトコルの初期化
     * @retval app_error_t エラーコード
     */
    app_error_t comm_init(void);

    /**
     * @brief CommData_tをUART経由で送信
     * @param data 送信するCommData_t構造体のポインタ
     * @param huart UARTハンドル
     * @retval app_error_t エラーコード
     */
    app_error_t comm_send_data_via_uart(const CommData_t *data, UART_HandleTypeDef *huart);

#ifdef __cplusplus
}
#endif

#endif /* __COMM_MANAGER_H */