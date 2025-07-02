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
#include "types.h"
#include <stdio.h>
#include <string.h>

/* Private define ------------------------------------------------------------*/
#define MAX_FILENAME_LENGTH 64
#define DATA_WRITE_BUFFER_SIZE 256

/* Private variables ---------------------------------------------------------*/
extern FATFS fs;
extern FIL fil;

/* Private function prototypes -----------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/**
 * @brief データをSDカードに保存
 * @param data_buffer データバッファ配列
 * @param data_count データ数
 * @param hrtc RTCハンドル
 * @retval bool 成功時true、失敗時false
 */
bool sd_save_data(const SensorData_t *data_buffer, uint32_t data_count,
                  RTC_HandleTypeDef *hrtc)
{
    char filename[MAX_FILENAME_LENGTH];
    char buffer[DATA_WRITE_BUFFER_SIZE];
    UINT bw;

    // パラメータ検証
    if (data_buffer == NULL)
    {
        return false;
    }
    if (hrtc == NULL)
    {
        return false;
    }
    if (data_count == 0)
    {
        return false;
    }

    // SDカード状態チェック
    if (!sd_check_status())
    {
        return false;
    }

    // 詳細なSDカード状態チェック
    DSTATUS disk_status = SD_disk_status(0);
    if (disk_status != 0)
    {
        return false;
    }

    // 日時ベースのファイル名を生成
    SD_get_datetime_filename(filename, sizeof(filename), hrtc);

    // ファイルオープン（リトライ機能付き）
    FRESULT res;
    int retry_count = 0;
    const int max_retries = 3;

    do
    {
        res = f_open(&fil, filename, FA_CREATE_ALWAYS | FA_WRITE);
        if (res == FR_OK)
        {
            break; // 成功
        }

        // リトライ前にSDカード状態を再確認
        HAL_Delay(100); // 短い待機

        // SDカードの再初期化を試行
        if (retry_count < max_retries - 1)
        {
            sd_check_status();
        }

        retry_count++;
    } while (retry_count < max_retries);

    if (res != FR_OK)
    {
        return false;
    }

    // ヘッダー書き込み
    sprintf(buffer, "時刻,温度(°C),圧力(Pa)\r\n");
    FRESULT write_result = f_write(&fil, buffer, strlen(buffer), &bw);
    if (write_result != FR_OK)
    {
        f_close(&fil);
        f_unlink(filename);
        return false;
    }

    // データ書き込み
    for (uint32_t i = 0; i < data_count; i++)
    {
        // floatデータの詳細診断を追加
        float temp_val = data_buffer[i].temp_data;
        float press_val = data_buffer[i].press_data;

        // float値のバイナリ表現を確認
        uint32_t temp_hex = *(uint32_t *)&temp_val;
        uint32_t press_hex = *(uint32_t *)&press_val;

        printf("データ診断[%lu]: 温度=%.6f (0x%08lX), 圧力=%.6f (0x%08lX)\r\n",
               i, temp_val, temp_hex, press_val, press_hex);

        // NaN や無限大の値をチェック
        if (temp_val != temp_val)
        { // NaN check
            printf("警告: 温度値がNaNです\r\n");
            temp_val = 0.0f;
        }
        if (press_val != press_val)
        { // NaN check
            printf("警告: 圧力値がNaNです\r\n");
            press_val = 0.0f;
        }

        sprintf(buffer, "%lu,%.2f,%.2f\r\n",
                (unsigned long)data_buffer[i].timestamp,
                temp_val,
                press_val);

        // デバッグ: 書き込み予定のデータを出力
        printf("書き込みデータ[%lu]: %s", i, buffer);

        write_result = f_write(&fil, buffer, strlen(buffer), &bw);
        if (write_result != FR_OK)
        {
            f_close(&fil);
            f_unlink(filename);
            return false;
        }

        // デバッグ: 実際に書き込まれたバイト数を確認
        if (bw != strlen(buffer))
        {
            printf("警告: 書き込みバイト数不一致 - 期待:%zu, 実際:%u\r\n", strlen(buffer), bw);
        }
    }

    // データを確実にSDカードに書き込む
    FRESULT sync_result = f_sync(&fil);
    if (sync_result != FR_OK)
    {
        printf("警告: f_sync失敗: FRESULT=%d\r\n", sync_result);
    }

    f_close(&fil);

    // 保存完了の詳細ログ
    printf("SDカード保存完了: ファイル名=%s, データ件数=%lu\r\n", filename, data_count);

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