/*************************************************************************************
 Title	 :  MAXIM Integrated MAX6675 Library for STM32 Using HAL Libraries
 Author  :  Bardia Alikhan Afshar <bardia.a.afshar@gmail.com>
 Software:  STM32CubeIDE
 Hardware:  Any STM32 device
*************************************************************************************/
#ifndef INC_MAX6675_H_
#define INC_MAX6675_H_

#include <stdint.h>
#include "main.h" // SPI_HandleTypeDefの定義を含む

// ------------------------- Defines -------------------------
// ------------------------- Types -------------------------
typedef struct MAX6675_Temp
{
    int16_t temp_int; // 整数部
    uint8_t temp_dec; // 小数部（0.25の倍数）
} MAX6675_Temp_t;

// ------------------------- Functions  ----------------------
MAX6675_Temp_t Max6675_Read_Temp(SPI_HandleTypeDef *hspi);

#endif
