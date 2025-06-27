/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : sensor_manager.c
 * @brief          : Sensor Management Functions Implementation
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
#include "sensor_manager.h"
#include <stdio.h>

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/**
 * @brief センサーデータの読み取りと検証
 * @param hspi_temp 温度センサー用SPIハンドル
 * @param hi2c_press 圧力センサー用I2Cハンドル
 * @retval SensorValidationResult_t 検証結果
 */
SensorValidationResult_t sensor_read_and_validate(SPI_HandleTypeDef *hspi_temp,
                                                  I2C_HandleTypeDef *hi2c_press)
{
    SensorValidationResult_t result = {0};

    // センサーデータ読み取り
    MAX31855_Data_t temperature_data = Max31855_Read_Temp(hspi_temp);
    MCP3425_Data_t pressure_data = MCP3425_Read_Pressure(hi2c_press);

    // データの有効性チェック
    result.temperature = temperature_data.processed_data;
    result.pressure = pressure_data.processed_data;
    result.is_valid = sensor_validate_data(result.temperature, result.pressure);

    if (!result.is_valid)
    {
        result.validation_error = APP_ERROR_SENSOR_INVALID_DATA;
    }
    else
    {
        result.validation_error = APP_ERROR_NONE;
    }

    return result;
}

/**
 * @brief センサーデータの有効性チェック
 * @param temperature 温度データ
 * @param pressure 圧力データ
 * @retval bool 有効性（true: 有効, false: 無効）
 */
bool sensor_validate_data(float temperature, float pressure)
{
    // 温度の範囲チェック
    if (temperature < SENSOR_VALID_TEMP_MIN || temperature > SENSOR_VALID_TEMP_MAX)
    {
        return false;
    }

    // 圧力の範囲チェック
    if (pressure < SENSOR_VALID_PRESSURE_MIN || pressure > SENSOR_VALID_PRESSURE_MAX)
    {
        return false;
    }

    return true;
}