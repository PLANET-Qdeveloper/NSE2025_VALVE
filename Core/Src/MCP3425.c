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
 * @brief MCP3425を初期化する関数（デフォルト設定で初期化）
 * @param hi2c I2Cハンドル
 * @retval None
 */
void MCP3425_Init(I2C_HandleTypeDef *hi2c)
{
	uint8_t config = MCP3425_DEFAULT_CONFIG;
	uint8_t mcp3425_addr = MCP3425_I2C_ADDR << 1;
	HAL_I2C_Master_Transmit(hi2c, mcp3425_addr, &config, 1, 10);
}

/**
 * @brief MCP3425から圧力を取得する関数
 * @param hi2c I2Cハンドル
 * @retval float 圧力値（Pa）、エラー時は-999.0f
 */
float MCP3425_Read_Pressure(I2C_HandleTypeDef *hi2c)
{
	uint8_t data[3] = {0}; // データ2バイト + 設定1バイト
	int16_t adc_value = 0;

	uint8_t mcp3425_addr = MCP3425_I2C_ADDR << 1; // HAL用に左シフト

	// データを読み取り（データ2バイト + 設定1バイト）
	if (HAL_I2C_Master_Receive(hi2c, mcp3425_addr, data, 3, 10) != HAL_OK)
	{
		return -999.0f;
	}

	// データレディビットをチェック（設定レジスタの最上位ビット）
	if (data[2] & MCP3425_RDY_BIT)
	{
		return -999.0f; // まだ変換中
	}

	// 12ビットデータを組み立て（ビッグエンディアン）
	// TABLE 4-3: 12ビットモードの範囲は -2048 ～ 2047
	adc_value = (data[0] << 8) | data[1];

	// 電圧値に変換（EQUATION 4-3およびTABLE 4-3に従って）
	// LSB = (2 × VREF) / 2^N = (2 × 2.048V) / 2^12 = 0.001V (12ビット、PGA=1倍)
	// 有効範囲: -2048 ～ 2047 (TABLE 4-3)
	// MCP3425が既に符号拡張済みのため、直接符号付き値として使用可能
	// 抵抗で1/3に分圧しているため、電圧は3倍にする
	float voltage = (float)adc_value * MCP3425_LSB_SIZE * 3;

	// MLH02kPSB06A圧力センサの変換式
	// 出力電圧範囲: 0.5-4.5V, 圧力範囲: 50-2000 PSI (344.738-13789.5 kPa)
	// P = (V - 0.5) / (4.5 - 0.5) * (13789.5 - 344.738) + 344.738
	// P = 3361.190 * V - 1335.857 (kPa単位)
	float pressure = 3361.190f * voltage - 1335.857f;

	return pressure;
}
