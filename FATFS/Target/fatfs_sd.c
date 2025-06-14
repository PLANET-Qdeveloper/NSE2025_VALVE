/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    fatfs_sd.c
 * @brief   This file includes a diskio driver skeleton to be completed by the user.
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

/* Includes ------------------------------------------------------------------*/
#include "fatfs_sd.h"
#include "main.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_sd.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SD_TIMEOUT 1000

/* Private variables ---------------------------------------------------------*/
/* Disk status */
static volatile DSTATUS Stat = STA_NOINIT;
static SD_HandleTypeDef hsd;

/* Private function prototypes -----------------------------------------------*/
DSTATUS SD_disk_initialize(BYTE pdrv);
DSTATUS SD_disk_status(BYTE pdrv);
DRESULT SD_disk_read(BYTE pdrv, BYTE *buff, DWORD sector, UINT count);
DRESULT SD_disk_write(BYTE pdrv, const BYTE *buff, DWORD sector, UINT count);
DRESULT SD_disk_ioctl(BYTE pdrv, BYTE cmd, void *buff);

Diskio_drvTypeDef SD_Driver =
    {
        SD_disk_initialize,
        SD_disk_status,
        SD_disk_read,
        SD_disk_write,
        SD_disk_ioctl,
};

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Initializes a Drive
 * @param  pdrv: Physical drive number (0..)
 * @retval DSTATUS: Operation status
 */
DSTATUS SD_disk_initialize(BYTE pdrv)
{
    Stat = STA_NOINIT;

    // SDカードの初期化パラメータを設定
    hsd.Instance = SDIO;
    hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
    hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
    hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
    hsd.Init.BusWide = SDIO_BUS_WIDE_1B;
    hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
    hsd.Init.ClockDiv = SDIO_INIT_CLK_DIV;

    // SDカードの初期化
    if (HAL_SD_Init(&hsd) != HAL_OK)
    {
        return Stat;
    }

    // カード情報の取得
    HAL_SD_CardInfoTypeDef CardInfo;
    if (HAL_SD_GetCardInfo(&hsd, &CardInfo) != HAL_OK)
    {
        return Stat;
    }

    // カードの種類に応じてバス幅を設定
    if (CardInfo.CardType == CARD_SDHC_SDXC)
    {
        hsd.Init.BusWide = SDIO_BUS_WIDE_4B;
    }

    // 高速モードの設定
    if (HAL_SD_ConfigWideBusOperation(&hsd, SDIO_BUS_WIDE_4B) != HAL_OK)
    {
        return Stat;
    }

    Stat &= ~STA_NOINIT;
    return Stat;
}

/**
 * @brief  Gets Disk Status
 * @param  pdrv: Physical drive number (0..)
 * @retval DSTATUS: Operation status
 */
DSTATUS SD_disk_status(BYTE pdrv)
{
    return Stat;
}

/**
 * @brief  Reads Sector(s)
 * @param  pdrv: Physical drive number (0..)
 * @param  *buff: Data buffer to store read data
 * @param  sector: Sector address (LBA)
 * @param  count: Number of sectors to read (1..128)
 * @retval DRESULT: Operation result
 */
DRESULT SD_disk_read(BYTE pdrv, BYTE *buff, DWORD sector, UINT count)
{
    DRESULT res = RES_ERROR;

    if (HAL_SD_ReadBlocks(&hsd, (uint32_t *)buff, sector, count, SD_TIMEOUT) == HAL_OK)
    {
        res = RES_OK;
    }

    return res;
}

/**
 * @brief  Writes Sector(s)
 * @param  pdrv: Physical drive number (0..)
 * @param  *buff: Data to be written
 * @param  sector: Sector address (LBA)
 * @param  count: Number of sectors to write (1..128)
 * @retval DRESULT: Operation result
 */
DRESULT SD_disk_write(BYTE pdrv, const BYTE *buff, DWORD sector, UINT count)
{
    DRESULT res = RES_ERROR;

    if (HAL_SD_WriteBlocks(&hsd, (uint32_t *)buff, sector, count, SD_TIMEOUT) == HAL_OK)
    {
        res = RES_OK;
    }

    return res;
}

/**
 * @brief  I/O control operation
 * @param  pdrv: Physical drive number (0..)
 * @param  cmd: Control code
 * @param  *buff: Buffer to send/receive control data
 * @retval DRESULT: Operation result
 */
DRESULT SD_disk_ioctl(BYTE pdrv, BYTE cmd, void *buff)
{
    DRESULT res = RES_ERROR;
    HAL_SD_CardInfoTypeDef CardInfo;

    if (Stat & STA_NOINIT)
        return RES_NOTRDY;

    switch (cmd)
    {
    case CTRL_POWER:
        res = RES_OK;
        break;

    case CTRL_SYNC:
        res = RES_OK;
        break;

    case GET_SECTOR_COUNT:
        HAL_SD_GetCardInfo(&hsd, &CardInfo);
        *(DWORD *)buff = CardInfo.LogBlockNbr;
        res = RES_OK;
        break;

    case GET_SECTOR_SIZE:
        HAL_SD_GetCardInfo(&hsd, &CardInfo);
        *(WORD *)buff = CardInfo.LogBlockSize;
        res = RES_OK;
        break;

    case GET_BLOCK_SIZE:
        HAL_SD_GetCardInfo(&hsd, &CardInfo);
        *(DWORD *)buff = CardInfo.LogBlockSize / 512;
        res = RES_OK;
        break;

    default:
        res = RES_PARERR;
    }

    return res;
}