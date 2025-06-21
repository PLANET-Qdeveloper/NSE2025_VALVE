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
#include "main.h"

/* Exported defines ----------------------------------------------------------*/
#define MCP3425_OK 0
#define MCP3425_ERROR_OC 1
#define MCP3425_ERROR_SCG 2
#define MCP3425_ERROR_SCV 3
#define MCP3425_ERROR_COMM 4

#define MCP3425_I2C_ADDR 0x68		// MCP3425のI2Cアドレス
#define MCP3425_LSB_SIZE 0.0000625f // 16-bit, PGA = 1 の場合
#define MCP3425_FULL_SCALE 2.048f	// フルスケール電圧

	/* Exported functions prototypes ---------------------------------------------*/
	float MCP3425_Read_Pressure(I2C_HandleTypeDef *hi2c);

#ifdef __cplusplus
}
#endif

#endif /* INC_MCP3425_H_ */
