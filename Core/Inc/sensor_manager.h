/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : sensor_manager.h
 * @brief          : Sensor Management Functions
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

#ifndef __SENSOR_MANAGER_H
#define __SENSOR_MANAGER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32f4xx_hal.h"
#include "config.h"
#include "app_types.h"
#include "MAX31855.h"
#include "MCP3425.h"

    /* Exported Functions --------------------------------------------------------*/

    /**
     * @brief センサーデータの読み取りと検証
     * @param hspi_temp 温度センサー用SPIハンドル
     * @param hi2c_press 圧力センサー用I2Cハンドル
     * @retval SensorValidationResult_t 検証結果
     */
    SensorValidationResult_t sensor_read_and_validate(SPI_HandleTypeDef *hspi_temp,
                                                      I2C_HandleTypeDef *hi2c_press);

    /**
     * @brief センサーデータの有効性チェック
     * @param temperature 温度データ
     * @param pressure 圧力データ
     * @retval bool 有効性（true: 有効, false: 無効）
     */
    bool sensor_validate_data(float temperature, float pressure);

#ifdef __cplusplus
}
#endif

#endif /* __SENSOR_MANAGER_H */