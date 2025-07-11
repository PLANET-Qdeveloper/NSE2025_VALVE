/*************************************************************************************
 Title	 :  MAXIM Integrated MAX31855 Library for STM32 Using HAL Libraries
 Author  :  Bardia Alikhan Afshar <bardia.a.afshar@gmail.com>
 Software:  STM32CubeIDE
 Hardware:  Any STM32 device
*************************************************************************************/
#ifndef INC_MAX31855_H_
#define INC_MAX31855_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"

// ------------------------- Defines -------------------------
// GPIO設定（Chip Select用）
#define MAX31855_CS_PORT GPIOB
#define MAX31855_CS_PIN GPIO_PIN_12

// エラー状態の定義（データシート準拠）
#define THERMOCOUPLE_OPEN 0x01        // サーモカップルオープン (D0)
#define THERMOCOUPLE_SHORT_GND 0x02   // GNDショート (D1)
#define THERMOCOUPLE_SHORT_VCC 0x04   // VCCショート (D2)
#define THERMOCOUPLE_FAULT 0x00010000 // フォルトフラグ (D16)

// MAX31855用の定数とマクロ
#define MAX31855_TEMP_SCALE 0.25f
#define MAX31855_REF_SCALE 0.0625f
#define MAX31855_TEMP_MIN_RAW -1080
#define MAX31855_TEMP_MAX_RAW 7200
#define MAX31855_TEMP_MASK 0x3FFF
#define MAX31855_TEMP_SIGN_BIT 0x2000
#define MAX31855_TEMP_SIGN_EXT 0xC000
#define MAX31855_REF_MASK 0x0FFF
#define MAX31855_REF_SIGN_BIT 0x0800
#define MAX31855_REF_SIGN_EXT 0xF000

// ------------------------- Functions  ----------------------
float MAX31855_Read_Temp(SPI_HandleTypeDef *hspi);
HAL_StatusTypeDef MAX31855_Read_Temp_DMA(SPI_HandleTypeDef *hspi, uint8_t *buffer);

#endif
