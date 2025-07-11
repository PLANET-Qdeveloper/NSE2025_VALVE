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
	if (HAL_I2C_Master_Receive(hi2c, mcp3425_addr, data, 3, HAL_MAX_DELAY) != HAL_OK)
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
	// data sheetより圧力の変化はratiometric
	// 出力電圧範囲: 0.5-4.5V, 圧力範囲: 50-2000 PSI (344.738-13789.5 kPa)
	// P = (V - 0.5) / (4.5 - 0.5) * (13789.5 - 344.738) + 344.738
	// P = 3361.190 * V - 1335.857 (kPa単位)
	float pressure = 3361.190f * voltage - 1335.857f;

	return pressure;
}

/**
 * @brief DMAを使用してMCP3425から圧力データを取得
 * @param hi2c I2Cハンドルへのポインタ
 * @param buffer DMA受信用のバッファ（3バイト）
 * @retval HAL_StatusTypeDef DMA転送の開始ステータス
 */
HAL_StatusTypeDef MCP3425_Read_Pressure_DMA(I2C_HandleTypeDef *hi2c, uint8_t *buffer)
{
	// MCP3425のアドレス（7bit）
	uint8_t device_addr = MCP3425_I2C_ADDR; // 0x68
	// I2C状態の詳細チェック
	HAL_I2C_StateTypeDef i2c_state = HAL_I2C_GetState(hi2c);
	
	if (i2c_state != HAL_I2C_STATE_READY)
	{
		printf("MCP3425 DMA: I2C not ready (state: %d)\r\n", i2c_state);
		return HAL_BUSY;
	}
	// DMAハンドルの状態確認
	if (hi2c->hdmarx != NULL)
	{
		HAL_DMA_StateTypeDef dma_state = HAL_DMA_GetState(hi2c->hdmarx);
		
		if (dma_state != HAL_DMA_STATE_READY)
		{
			printf("MCP3425 DMA: DMA not ready\r\n");
			HAL_DMA_Abort(hi2c->hdmarx);
		}
	}
	else
	{
		printf("MCP3425 DMA: DMA handle is NULL\r\n");
		return HAL_ERROR;
	}

	// DMA受信開始
	HAL_StatusTypeDef status = HAL_I2C_Master_Receive_DMA(hi2c, device_addr << 1, buffer, 3);

	return status;
}

/**
 * @brief MCP3425の動作確認用テスト関数
 * @param hi2c I2Cハンドルへのポインタ
 * @retval uint8_t デバイスの応答があれば1、なければ0
 */
uint8_t MCP3425_Test_Connection(I2C_HandleTypeDef *hi2c)
{
	uint8_t device_addr = MCP3425_I2C_ADDR << 1;
	uint8_t test_data[3] = {0};
	
	printf("=== MCP3425接続テスト開始 ===\r\n");
	
	// I2Cデバイスの存在確認
	HAL_StatusTypeDef status = HAL_I2C_IsDeviceReady(hi2c, device_addr, 3, 100);
	printf("MCP3425デバイス確認: %s (ステータス: %d)\r\n", 
	       (status == HAL_OK) ? "接続OK" : "接続NG", status);
	
	if (status != HAL_OK)
	{
		return 0;
	}
	
	// 同期受信テスト
	status = HAL_I2C_Master_Receive(hi2c, device_addr, test_data, 3, 1000);
	printf("同期受信テスト: %s (ステータス: %d)\r\n", 
	       (status == HAL_OK) ? "成功" : "失敗", status);
	
	if (status == HAL_OK)
	{
		printf("受信データ: 0x%02X 0x%02X 0x%02X\r\n", 
		       test_data[0], test_data[1], test_data[2]);
	}
	
	printf("=== MCP3425接続テスト完了 ===\r\n");
	return (status == HAL_OK) ? 1 : 0;
}
