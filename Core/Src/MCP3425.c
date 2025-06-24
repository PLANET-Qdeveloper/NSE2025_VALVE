/**
 ******************************************************************************
 * @file    MCP3425.c
 * @brief   This file provides code for the configuration
 *          of the MCP3425 instances.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2021 SimpleMethod
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "MCP3425.h"
#include "main.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions --------------------------------------------------------*/

/**
 * @brief MCP3425から圧力を取得する関数
 * @param hi2c I2Cハンドル
 * @retval 圧力値（Pa）、エラー時は-999.0f
 */
MCP3425_Data_t MCP3425_Read_Pressure(I2C_HandleTypeDef *hi2c)
{
	uint16_t adc_value = 0;

	uint8_t mcp3425_addr = MCP3425_I2C_ADDR << 1; // HAL用に左シフト

	if (HAL_I2C_Master_Receive(hi2c, mcp3425_addr, (uint8_t *)&adc_value, 2, HAL_MAX_DELAY) != HAL_OK)
	{
		return (MCP3425_Data_t){.processed_data = -999.0f, .raw_data = 0};
	}

	// ビッグエンディアンからリトルエンディアンに変換
	adc_value = ((adc_value & 0xFF) << 8) | ((adc_value >> 8) & 0xFF);

	float pressure = (float)adc_value * 0.1f;
	return (MCP3425_Data_t){.processed_data = pressure, .raw_data = adc_value};
}
