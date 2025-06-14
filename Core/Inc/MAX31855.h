/**
 ******************************************************************************
 * @file    MAX31855.h
 * @brief   This file contains all the function prototypes for
 *          the MAX31855.c file
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

#ifndef INC_MAX31855_H_
#define INC_MAX31855_H_

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported defines ----------------------------------------------------------*/
#define MAX31855_OK 0
#define MAX31855_ERROR_OC 1
#define MAX31855_ERROR_SCG 2
#define MAX31855_ERROR_SCV 3
#define MAX31855_ERROR_COMM 4

	/* Exported types ------------------------------------------------------------*/
	typedef struct
	{
		uint16_t pin;
		GPIO_TypeDef *port;
	} MAX31855_GPIO;

	typedef struct
	{
		MAX31855_GPIO nss;
		SPI_HandleTypeDef *hspi;
		uint8_t fault;
		uint8_t ocFault;
		uint8_t scgFault;
		uint8_t scvFault;
	} MAX31855_StateHandle;

	/* Exported variables --------------------------------------------------------*/
	extern MAX31855_StateHandle MAX31855_Handle;

	/* Exported functions prototypes ---------------------------------------------*/
	HAL_StatusTypeDef MAX31855_Init(MAX31855_StateHandle *MAX31855, SPI_HandleTypeDef *hspi, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
	uint8_t MAX31855_GetFault(MAX31855_StateHandle *MAX31855);
	HAL_StatusTypeDef MAX31855_ReadData(MAX31855_StateHandle *MAX31855);
	void MAX31855_SetNSSState(MAX31855_StateHandle *MAX31855, GPIO_PinState state);
	float MAX31855_ReadData(I2C_HandleTypeDef *hi2c);

#ifdef __cplusplus
}
#endif

#endif /* INC_MAX31855_H_ */
