/*************************************************************************************
 Title	 :  MAXIM Integrated MAX6675 Library for STM32 Using HAL Libraries
 Author  :  Bardia Alikhan Afshar <bardia.a.afshar@gmail.com>
 Software:  STM32CubeIDE
 Hardware:  Any STM32 device
*************************************************************************************/
#ifndef __MAX6675_H__
#define __MAX6675_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"

    typedef struct
    {
        int16_t integer; // 整数部分
        uint8_t decimal; // 小数点部分（0-3）
    } MAX6675_Temperature_t;

    MAX6675_Temperature_t MAX6675_ReadTemperature(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAX6675_H__ */
