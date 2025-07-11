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
#define MCP3425_I2C_ADDR 0x68     // MCP3425のI2Cアドレス
#define MCP3425_LSB_SIZE 0.001f   // 12-bit, PGA = 1 の場合
#define MCP3425_FULL_SCALE 2.048f // フルスケール電圧

// 設定レジスタビット定義
#define MCP3425_RDY_BIT (1 << 7) // データレディフラグ
#define MCP3425_OC_BIT (1 << 4)  // 変換モードビット (1=連続変換, 0=ワンショット)
#define MCP3425_S1_BIT (1 << 3)  // サンプルレート選択ビット1
#define MCP3425_S0_BIT (1 << 2)  // サンプルレート選択ビット0
#define MCP3425_G1_BIT (1 << 1)  // PGAゲイン選択ビット1
#define MCP3425_G0_BIT (1 << 0)  // PGAゲイン選択ビット0

// サンプルレート設定 (S1-S0ビット)
#define MCP3425_SAMPLE_RATE_240SPS 0x00 // 00 = 240 SPS (12ビット)
#define MCP3425_SAMPLE_RATE_60SPS 0x04  // 01 = 60 SPS (14ビット)
#define MCP3425_SAMPLE_RATE_15SPS 0x08  // 10 = 15 SPS (16ビット)

// PGAゲイン設定 (G1-G0ビット)
#define MCP3425_GAIN_1X 0x00 // 00 = x1
#define MCP3425_GAIN_2X 0x01 // 01 = x2
#define MCP3425_GAIN_4X 0x02 // 10 = x4
#define MCP3425_GAIN_8X 0x03 // 11 = x8

// デフォルト設定（12ビット、連続変換モード、ゲイン1倍）
#define MCP3425_DEFAULT_CONFIG (MCP3425_OC_BIT | MCP3425_SAMPLE_RATE_240SPS | MCP3425_GAIN_1X)

    /* Exported functions prototypes ---------------------------------------------*/
    void MCP3425_Init(I2C_HandleTypeDef *hi2c);
    float MCP3425_Read_Pressure(I2C_HandleTypeDef *hi2c);
    HAL_StatusTypeDef MCP3425_Read_Pressure_DMA(I2C_HandleTypeDef *hi2c, uint8_t *buffer);
    uint8_t MCP3425_Test_Connection(I2C_HandleTypeDef *hi2c);

#ifdef __cplusplus
}
#endif

#endif /* INC_MCP3425_H_ */
