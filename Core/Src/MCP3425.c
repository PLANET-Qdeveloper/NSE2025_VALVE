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

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions --------------------------------------------------------*/

/**
 * @brief MCP3425から圧力を取得する関数
 * @param hi2c I2Cハンドル
 * @retval float 圧力値（Pa）、エラー時は-999.0f
 */
float MCP3425_Read_Pressure(I2C_HandleTypeDef *hi2c)
{
	uint8_t config = MCP3425_CONFIG_DEFAULT;
	uint8_t data[3];
	uint16_t adc_value = 0;
	
	uint8_t mcp3425_addr = MCP3425_I2C_ADDR << 1; // HAL用に左シフト

	// 1. 設定を書き込んで変換を開始
	if (HAL_I2C_Master_Transmit(hi2c, mcp3425_addr, &config, 1, 100) != HAL_OK)
	{
		return -999.0f;
	}

	// 2. 変換完了まで待機（16bit変換の場合、最大67ms）
	HAL_Delay(70);

	// 3. データを読み取り（3バイト: データ上位、データ下位、設定）
	if (HAL_I2C_Master_Receive(hi2c, mcp3425_addr, data, 3, 100) != HAL_OK)
	{
		return -999.0f;
	}

	// 4. 変換完了フラグを確認
	if (!(data[2] & MCP3425_RDY_BIT))
	{
		// まだ変換中の場合
		return -999.0f;
	}

	// 5. 16bitデータを結合（ビッグエンディアン）
	adc_value = (data[0] << 8) | data[1];

	// 6. 電圧に変換（16bit, PGA=1の場合）
	float voltage = (float)((int16_t)adc_value) * MCP3425_LSB_SIZE;

	// 7. 圧力に変換（センサー固有の変換式を使用）
	// 注意：この変換式は使用する圧力センサーの仕様に合わせて調整が必要
	float pressure = voltage * 100.0f; // 例：100Pa/V

	return pressure;
}
