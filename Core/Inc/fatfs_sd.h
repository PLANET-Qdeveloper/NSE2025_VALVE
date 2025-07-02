/*
 * fatfs_sd.h
 *
 *  Created on: Jul 2, 2020
 *      Author: meh
 */

#ifndef INC_FATFS_SD_H_
#define INC_FATFS_SD_H_

#ifndef __FATFS_SD_H
#define __FATFS_SD_H

#include "diskio.h"
#include "main.h"
#include "types.h"
#include <stddef.h>

/* Definitions for MMC/SDC command */
#define CMD0 (0x40 + 0)   /* GO_IDLE_STATE */
#define CMD1 (0x40 + 1)   /* SEND_OP_COND */
#define CMD8 (0x40 + 8)   /* SEND_IF_COND */
#define CMD9 (0x40 + 9)   /* SEND_CSD */
#define CMD10 (0x40 + 10) /* SEND_CID */
#define CMD12 (0x40 + 12) /* STOP_TRANSMISSION */
#define CMD16 (0x40 + 16) /* SET_BLOCKLEN */
#define CMD17 (0x40 + 17) /* READ_SINGLE_BLOCK */
#define CMD18 (0x40 + 18) /* READ_MULTIPLE_BLOCK */
#define CMD23 (0x40 + 23) /* SET_BLOCK_COUNT */
#define CMD24 (0x40 + 24) /* WRITE_BLOCK */
#define CMD25 (0x40 + 25) /* WRITE_MULTIPLE_BLOCK */
#define CMD41 (0x40 + 41) /* SEND_OP_COND (ACMD) */
#define CMD55 (0x40 + 55) /* APP_CMD */
#define CMD58 (0x40 + 58) /* READ_OCR */

#define TRUE 1
#define FALSE 0
#ifndef bool
#define bool BYTE
#endif
#define SD_CS_GPIO_Port GPIOA
#define SD_CS_Pin GPIO_PIN_4

DSTATUS SD_disk_initialize(BYTE pdrv);
DSTATUS SD_disk_status(BYTE pdrv);
DRESULT SD_disk_read(BYTE pdrv, BYTE *buff, DWORD sector, UINT count);
DRESULT SD_disk_write(BYTE pdrv, const BYTE *buff, DWORD sector, UINT count);
DRESULT SD_disk_ioctl(BYTE pdrv, BYTE cmd, void *buff);
void SD_get_datetime_filename(char *filename, size_t max_len, RTC_HandleTypeDef *hrtc);

#define SPI_TIMEOUT 1000

/* SDカード応答コード */
#define SD_RESPONSE_IDLE 0x01
#define SD_RESPONSE_SUCCESS 0x00
#define SD_RESPONSE_ERROR 0xFF

/* SDカードトークン */
#define SD_TOKEN_START_BLOCK 0xFE
#define SD_TOKEN_START_MULTI_WRITE 0xFC
#define SD_TOKEN_STOP_MULTI_WRITE 0xFD
#define SD_TOKEN_DUMMY 0xFF

/* CRC値 */
#define SD_CRC_CMD0 0x95
#define SD_CRC_CMD8 0x87

/* SDカード電圧範囲 */
#define SD_VOLTAGE_RANGE_2V7_3V6 0x1AA

/* SDカードタイプ */
#define SD_CARD_TYPE_MMC 1
#define SD_CARD_TYPE_SDC_V1 2
#define SD_CARD_TYPE_SDC_V2 6

/* SDカード操作タイムアウト */
#define SD_READY_TIMEOUT_MS 500
#define SD_INIT_TIMEOUT_MS 1000
#define SD_DATA_TIMEOUT_MS 100

/* SDカードブロックサイズ */
#define SD_BLOCK_SIZE 512

/* SDカード初期化時のクロック数 */
#define SD_INIT_CLOCK_CYCLES 10

/* データ応答マスク */
#define SD_DATA_RESPONSE_MASK 0x1F
#define SD_DATA_RESPONSE_ACCEPTED 0x05

/* 再試行回数 */
#define SD_MAX_RETRY_COUNT 64
#define SD_CMD_RETRY_COUNT 10

/* SDカード管理関数 */
bool sd_save_data(const SensorData_t *data_buffer, uint32_t data_count, RTC_HandleTypeDef *hrtc);
bool sd_check_status(void);

#endif

#endif /* INC_FATFS_SD_H_ */
