/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : sdcard.c
 * @brief          : SDCard Management Functions Implementation
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "fatfs_sd.h"
#include "sdcard.h"
#include "types.h"
#include <stdio.h>
#include <string.h>

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
extern FATFS fs;
extern FIL fil;
static uint32_t next_file_number = 1;

/* Private function prototypes ----------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/**
 * @brief データをSDカードに保存
 * @param data_buffer データバッファ配列
 * @param data_count データ数
 * @retval bool 成功時true、失敗時false
 */
bool sd_save_data(SensorData_t *data_buffer)
{
    static bool file_opened = false;
    static char current_filename[16] = {0};
    static uint32_t save_count = 1;
    static char csv_buffer[2560] = {0};
    UINT bw;
    FRESULT res;
    char csv_line[128];

    // SDカード状態チェック
    if (!sd_check_status())
    {
        return false;
    }

    // 初回のみファイルオープンとヘッダー書き込み
    if (!file_opened)
    {
        snprintf(current_filename, sizeof(current_filename), "%lu.csv", (unsigned long)next_file_number);

        /* 次回のために番号をインクリメント */
        next_file_number++;

        res = f_open(&fil, current_filename, FA_CREATE_ALWAYS | FA_WRITE);
        if (res != FR_OK)
        {
            return false;
        }
        file_opened = true;
        save_count = 1;
        csv_buffer[0] = '\0'; // バッファをクリア

        // ヘッダー書き込み
        const char *header = "時刻,温度(°C),圧力(Pa),NOS開放\r\n";
        res = f_write(&fil, header, strlen(header), &bw);
        if (res != FR_OK || bw != strlen(header))
        {
            f_close(&fil);
            f_unlink(current_filename);
            file_opened = false;
            return false;
        }
    }

    snprintf(csv_line, sizeof(csv_line), "%lu,%.2f,%.2f,%d\r\n",
             HAL_GetTick() / 1000,
             data_buffer->temp_data,
             data_buffer->press_data,
             data_buffer->is_nos_open);

    strcat(csv_buffer, csv_line);
    save_count++;

    // 20個たまったら書き込み
    if (save_count % 20 == 0)
    {
        res = f_write(&fil, csv_buffer, strlen(csv_buffer), &bw);
        if (res != FR_OK || bw != strlen(csv_buffer))
        {
            f_close(&fil);
            f_unlink(current_filename);
            file_opened = false;
            return false;
        }
        csv_buffer[0] = '\0'; // バッファをクリア
    }

    // 10000個のデータが書き込まれたらファイルを閉じる
    if (save_count % 10000 == 0)
    {
        f_sync(&fil);
        f_close(&fil);
        file_opened = false;
    }

    return true;
}

/**
 * @brief SDカードの状態チェックと再初期化
 * @retval bool SDカードが使用可能かどうか
 */
bool sd_check_status(void)
{
    // ディスクの状態を確認
    DSTATUS disk_status = SD_disk_status(0);

    if (disk_status != 0)
    {
        // ディスクエラーの場合、再初期化を試行
        DSTATUS reinit_status = SD_disk_initialize(0);
        if (reinit_status == 0)
        {
            // ファイルシステムの再マウント
            f_mount(NULL, "", 0); // アンマウント
            f_mount(&fs, "", 1);
            return true;
        }
        return false;
    }

    return true;
}

/* ファイル番号を初期化（起動時に一度だけ実行） */
void SD_init_valve_file_number(void)
{
    DIR dir;
    FILINFO fno;
    FRESULT res;
    uint32_t max_num = 0;

    /* ルートディレクトリを開く */
    res = f_opendir(&dir, "");
    if (res != FR_OK)
    {
        next_file_number = 1;
        return;
    }

    /* 既存の*.csvファイルを探して最大番号を取得 */
    while (f_readdir(&dir, &fno) == FR_OK && fno.fname[0] != 0)
    {
        /* ディレクトリをスキップ */
        if (fno.fattrib & AM_DIR)
            continue;

        /* .csvファイルかチェック */
        char *dot = strrchr(fno.fname, '.');
        if (dot != NULL && strcmp(dot, ".csv") == 0)
        {
            /* ファイル名から番号を抽出 */
            *dot = '\0'; // 一時的に拡張子を削除
            uint32_t num = (uint32_t)atoi(fno.fname);
            if (num > max_num)
            {
                max_num = num;
            }
        }
    }
    f_closedir(&dir);

    /* 次の番号を設定 */
    next_file_number = max_num + 1;
}
