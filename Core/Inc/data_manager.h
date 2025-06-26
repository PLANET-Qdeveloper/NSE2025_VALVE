/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : data_manager.h
 * @brief          : Data Management Functions
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

#ifndef __DATA_MANAGER_H
#define __DATA_MANAGER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32f4xx_hal.h"
#include "app_types.h"
#include "ff.h"

    /* Exported Functions --------------------------------------------------------*/

    /**
     * @brief データをSDカードに保存
     * @param data_buffer データバッファ配列
     * @param data_count データ数
     * @param hrtc RTCハンドル
     * @retval app_error_t エラーコード
     */
    app_error_t data_save_to_sd(const DataBuffer_t *data_buffer, uint32_t data_count,
                                RTC_HandleTypeDef *hrtc);

    /**
     * @brief SDカードの状態チェックと再初期化
     * @retval bool SDカードが使用可能かどうか
     */
    bool data_check_sd_status(void);

#ifdef __cplusplus
}
#endif

#endif /* __DATA_MANAGER_H */