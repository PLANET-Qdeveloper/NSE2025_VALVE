/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    fatfs_sd.h
 * @brief   Header for fatfs_sd.c module
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __fatfs_sd_H
#define __fatfs_sd_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "ff.h"
#include "ff_gen_drv.h"
#include "stm32f4xx_hal.h"

    /* Exported types ------------------------------------------------------------*/
    /* Exported constants --------------------------------------------------------*/
    /* Exported functions ------------------------------------------------------- */
    extern Diskio_drvTypeDef SD_Driver;

#ifdef __cplusplus
}
#endif

#endif /* __fatfs_sd_H */