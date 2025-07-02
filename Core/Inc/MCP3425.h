/**
 ******************************************************************************
 * @file    MCP3425.h
 * @brief   This file contains all the function prototypes for
 *          the MCP3425.c file
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

#ifndef INC_MCP3425_H_
#define INC_MCP3425_H_

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "stm32f4xx_hal.h"

/* Exported defines ----------------------------------------------------------*/
#define MCP3425_OK 0
#define MCP3425_ERROR_OC 1
#define MCP3425_ERROR_SCG 2
#define MCP3425_ERROR_SCV 3
#define MCP3425_ERROR_COMM 4

#define MCP3425_I2C_ADDR 0x68		// MCP3425のI2Cアドレス
#define MCP3425_LSB_SIZE 0.0000625f // 16-bit, PGA = 1 の場合
#define MCP3425_FULL_SCALE 2.048f	// フルスケール電圧

// MCP3425設定レジスタビット定義
#define MCP3425_RDY_BIT     0x80    // Ready bit (0: 変換中, 1: 変換完了)
#define MCP3425_CHANNEL_1   0x00    // チャンネル1選択
#define MCP3425_CHANNEL_2   0x20    // チャンネル2選択
#define MCP3425_CONTINUOUS  0x10    // 連続変換モード
#define MCP3425_ONE_SHOT    0x00    // 単発変換モード
#define MCP3425_16_BIT      0x08    // 16bit分解能
#define MCP3425_14_BIT      0x04    // 14bit分解能
#define MCP3425_12_BIT      0x00    // 12bit分解能
#define MCP3425_PGA_1       0x00    // PGA = 1
#define MCP3425_PGA_2       0x01    // PGA = 2
#define MCP3425_PGA_4       0x02    // PGA = 4
#define MCP3425_PGA_8       0x03    // PGA = 8

// デフォルト設定: 16bit, PGA=1, 単発変換, チャンネル1
#define MCP3425_CONFIG_DEFAULT (MCP3425_RDY_BIT | MCP3425_CHANNEL_1 | MCP3425_ONE_SHOT | MCP3425_16_BIT | MCP3425_PGA_1)

	/* Exported functions prototypes ---------------------------------------------*/
	float MCP3425_Read_Pressure(I2C_HandleTypeDef *hi2c);

#ifdef __cplusplus
}
#endif

#endif /* INC_MCP3425_H_ */
