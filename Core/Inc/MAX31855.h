/*************************************************************************************
 Title	 :  MAXIM Integrated MAX31855 Library for STM32 Using HAL Libraries
 Author  :  Bardia Alikhan Afshar <bardia.a.afshar@gmail.com>
 Software:  STM32CubeIDE
 Hardware:  Any STM32 device
*************************************************************************************/
#ifndef INC_MAX31855_H_
#define INC_MAX31855_H_

#include <stdint.h>
#include "main.h" // SPI_HandleTypeDefの定義を含む

// ------------------------- Defines -------------------------
// GPIO設定（Chip Select用）
#define MAX31855_CS_PORT GPIOB
#define MAX31855_CS_PIN GPIO_PIN_12

// エラー状態の定義（データシート準拠）
#define THERMOCOUPLE_OPEN 0x01        // サーモカップルオープン (D0)
#define THERMOCOUPLE_SHORT_GND 0x02   // GNDショート (D1)
#define THERMOCOUPLE_SHORT_VCC 0x04   // VCCショート (D2)
#define THERMOCOUPLE_FAULT 0x00010000 // フォルトフラグ (D16)

// ------------------------- Types -------------------------
// MAX31855用の定数とマクロのみ定義

// ------------------------- Functions  ----------------------
float Max31855_Read_Temp(SPI_HandleTypeDef *hspi);

#endif
