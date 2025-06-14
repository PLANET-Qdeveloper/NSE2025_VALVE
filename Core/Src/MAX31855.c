/**
 ******************************************************************************
 * @file    MAX31855.c
 * @brief   This file provides code for the configuration
 *          of the MAX31855 instances.
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
#include "MAX31855.h"
#include "main.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions --------------------------------------------------------*/

/**
 * @brief  MAX31855の初期化
 * @param  MAX31855 MAX31855の状態ハンドル
 * @param  hspi SPIハンドル
 * @param  GPIOx NSSピンのGPIOポート
 * @param  GPIO_Pin NSSピンの番号
 * @retval HAL_StatusTypeDef 初期化結果
 */
HAL_StatusTypeDef MAX31855_Init(MAX31855_StateHandle *MAX31855, SPI_HandleTypeDef *hspi, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	if (MAX31855 == NULL || hspi == NULL || GPIOx == NULL)
	{
		return HAL_ERROR;
	}

	MAX31855->hspi = hspi;
	MAX31855->nss.pin = GPIO_Pin;
	MAX31855->nss.port = GPIOx;
	MAX31855_SetNSSState(MAX31855, GPIO_PIN_SET);

	return HAL_OK;
}

/**
 * @brief  故障状態の取得
 * @param  MAX31855 MAX31855の状態ハンドル
 * @retval uint8_t 故障コード
 */
uint8_t MAX31855_GetFault(MAX31855_StateHandle *MAX31855)
{
	if (MAX31855 == NULL)
	{
		return MAX31855_ERROR_COMM;
	}

	if (MAX31855->ocFault)
	{
		return MAX31855_ERROR_OC;
	}
	else if (MAX31855->scgFault)
	{
		return MAX31855_ERROR_SCG;
	}
	else if (MAX31855->scvFault)
	{
		return MAX31855_ERROR_SCV;
	}

	return MAX31855_OK;
}

/**
 * @brief  データの読み取り
 * @param  MAX31855 MAX31855の状態ハンドル
 * @retval HAL_StatusTypeDef 読み取り結果
 */
HAL_StatusTypeDef MAX31855_ReadData(MAX31855_StateHandle *MAX31855)
{
	if (MAX31855 == NULL)
	{
		return HAL_ERROR;
	}

	uint8_t payload[4];
	int32_t frame;

	MAX31855_SetNSSState(MAX31855, GPIO_PIN_RESET);
	HAL_StatusTypeDef status = HAL_SPI_Receive(MAX31855->hspi, payload, 4, 1000);
	MAX31855_SetNSSState(MAX31855, GPIO_PIN_SET);

	if (status != HAL_OK)
	{
		return status;
	}

	// 故障フラグのリセット
	MAX31855->scvFault = 0;
	MAX31855->scgFault = 0;
	MAX31855->ocFault = 0;
	MAX31855->fault = 0;

	// データの組み立て
	frame = ((int32_t)payload[0] << 24) |
			((int32_t)payload[1] << 16) |
			((int32_t)payload[2] << 8) |
			((int32_t)payload[3]);

	// 故障フラグのチェック
	if (frame & 0x00000004)
		MAX31855->scvFault = 1;
	if (frame & 0x00000002)
		MAX31855->scgFault = 1;
	if (frame & 0x00000001)
		MAX31855->ocFault = 1;
	if (frame & 0x00010000)
		MAX31855->fault = 1;

	return HAL_OK;
}

/**
 * @brief  NSSピンの状態設定
 * @param  MAX31855 MAX31855の状態ハンドル
 * @param  state 設定する状態
 * @retval None
 */
void MAX31855_SetNSSState(MAX31855_StateHandle *MAX31855, GPIO_PinState state)
{
	if (MAX31855 != NULL)
	{
		HAL_GPIO_WritePin(MAX31855->nss.port, MAX31855->nss.pin, state);
	}
}

/**
 * @brief  MAX31855からのデータ読み取り（I2C経由）
 * @param  hi2c I2Cハンドル
 * @retval float 読み取った電圧値（V）、エラー時は-1.0
 */
float MAX31855_ReadData(I2C_HandleTypeDef *hi2c)
{
	if (hi2c == NULL)
	{
		return -1.0f;
	}

	uint8_t config = 0x90; // ワンショット | 16ビット | ゲイン=1
	uint8_t data[2];
	int16_t raw;
	float voltage;

	if (HAL_I2C_Master_Transmit(hi2c, 0x68 << 1, &config, 1, HAL_MAX_DELAY) != HAL_OK)
	{
		return -1.0f;
	}

	HAL_Delay(100); // 変換完了待ち

	if (HAL_I2C_Master_Receive(hi2c, 0x68 << 1, data, 2, HAL_MAX_DELAY) == HAL_OK)
	{
		raw = (int16_t)((data[0] << 8) | data[1]);
		voltage = 2.047f * raw / 32767.0f * 2; // ±2.048V フルスケール（ゲイン = 1）
		return voltage;
	}

	return -1.0f; // 読み取り失敗
}
