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
#define MAX_FILENAME_LENGTH 64
#define DATA_WRITE_BUFFER_SIZE 256

/* Private variables ---------------------------------------------------------*/
extern FATFS fs;
extern FIL fil;
static uint32_t next_valve_file_number = 1;

/* Private function prototypes ----------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/**
 * @brief データをSDカードに保存
 * @param data_buffer データバッファ配列
 * @param data_count データ数
 * @retval bool 成功時true、失敗時false
 */
bool sd_save_data(const SensorData_t *data_buffer, uint32_t data_count)
{
    static bool file_opened = false;
    static char current_filename[MAX_FILENAME_LENGTH];
    static uint32_t total_data_count = 0;  // 総書き込みデータ数をカウント
    char buffer[DATA_WRITE_BUFFER_SIZE];
    UINT bw;
    FRESULT res;

    // パラメータ検証
    if (data_buffer == NULL || data_count == 0)
    {
        return false;
    }

    // SDカード状態チェック
    if (!sd_check_status())
    {
        return false;
    }

    // 初回のみファイルオープンとヘッダー書き込み
    if (!file_opened)
    {
        // 新しいファイルを作成
        SD_get_filename(current_filename, sizeof(current_filename));

        res = f_open(&fil, current_filename, FA_CREATE_ALWAYS | FA_WRITE);
        if (res != FR_OK)
        {
            return false;
        }
        file_opened = true;
        total_data_count = 0;  // カウンターをリセット

        // ヘッダー書き込み
        sprintf(buffer, "時刻,温度(°C),圧力(Pa),NOS開放\r\n");
        res = f_write(&fil, buffer, strlen(buffer), &bw);
        if (res != FR_OK || bw != strlen(buffer))
        {
            f_close(&fil);
            f_unlink(current_filename);
            file_opened = false;
            return false;
        }
    }

    // データ書き込み
    for (uint32_t i = 0; i < data_count; i++)
    {
        // センサーデータをCSV形式で文字列化
        sprintf(buffer, "%lu,%.2f,%.2f,%d\r\n",
                data_buffer[i].timestamp,
                data_buffer[i].temp_data,
                data_buffer[i].press_data,
                data_buffer[i].is_nos_open);

        res = f_write(&fil, buffer, strlen(buffer), &bw);
        if (res != FR_OK || bw != strlen(buffer))
        {
            f_close(&fil);
            f_unlink(current_filename);
            file_opened = false;
            total_data_count = 0;
            return false;
        }
        
        total_data_count++;  // 書き込み完了後にカウントアップ
    }

    // 定期的な同期（パフォーマンス向上）
    f_sync(&fil);

    // 2000個のデータが書き込まれたらファイルを閉じる
    if (total_data_count >= 2000)
    {
        f_close(&fil);
        file_opened = false;
        total_data_count = 0;
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

/* Vファイル番号を初期化（起動時に一度だけ実行） */
void SD_init_valve_file_number(void)
{
    DIR dir;
    FILINFO fno;
    FRESULT res;
    uint32_t max_num = 0;
    uint32_t current_num;

    /* ルートディレクトリを開く */
    res = f_opendir(&dir, "");
    if (res == FR_OK)
    {
        /* 既存のV_x.csvファイルを探して最大番号を取得 */
        for (;;)
        {
            res = f_readdir(&dir, &fno);
            if (res != FR_OK || fno.fname[0] == 0)
                break; /* ディレクトリの終端またはエラー */

            /* V_x.csvファイルかチェック */
            if ((fno.fattrib & AM_DIR) == 0) /* ディレクトリではない */
            {
                char *name = fno.fname;
                /* ファイル名がV_で始まり、.csvで終わるかチェック */
                if (strncmp(name, "V_", 2) == 0)
                {
                    char *dot = strrchr(name, '.');
                    if (dot != NULL && strcmp(dot, ".csv") == 0)
                    {
                        /* 番号部分を抽出 */
                        char *num_start = name + 2; /* "V_"の後 */
                        char *num_end = dot;
                        char num_str[16];

                        /* 番号部分をコピー */
                        size_t num_len = num_end - num_start;
                        if (num_len > 0 && num_len < sizeof(num_str))
                        {
                            strncpy(num_str, num_start, num_len);
                            num_str[num_len] = '\0';

                            /* 数値に変換 */
                            current_num = (uint32_t)atoi(num_str);
                            if (current_num > max_num)
                            {
                                max_num = current_num;
                            }
                        }
                    }
                }
            }
        }
        f_closedir(&dir);
    }

    /* 次の番号を設定 */
    next_valve_file_number = max_num + 1;

    printf("Vファイル番号初期化完了: 次のファイル番号=%lu\r\n", (unsigned long)next_valve_file_number);
}

/* Vファイル名を連番で生成（8.3形式対応） */
void SD_get_filename(char *filename, size_t max_len)
{
    /* パラメータ検証 */
    if (filename == NULL || max_len < 13)
    {
        if (filename != NULL && max_len > 0)
        {
            filename[0] = '\0'; /* エラー時は空文字列 */
        }
        return;
    }

    /* 8.3形式に適合するファイル名を生成 */
    /* 形式: V_x.csv（xは1から始まる連番） */
    snprintf(filename, max_len, "V_%lu.csv", (unsigned long)next_valve_file_number);
    

    /* 次回のために番号をインクリメント */
    next_valve_file_number++;
}