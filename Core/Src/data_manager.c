/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : data_manager.c
 * @brief          : Data Management Functions Implementation
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
#include "data_manager.h"
#include "fatfs_sd.h"
#include <stdio.h>
#include <string.h>

/* Private variables ---------------------------------------------------------*/
extern FATFS fs;
extern FIL fil;

/* Private function prototypes -----------------------------------------------*/
static const char *fresult_to_string(FRESULT fr_result);

/* Exported functions --------------------------------------------------------*/

/**
 * @brief FRESULTエラーコードを文字列に変換
 * @param fr_result FRESULTエラーコード
 * @return エラーメッセージ文字列
 */
static const char *fresult_to_string(FRESULT fr_result)
{
    switch (fr_result)
    {
    case FR_OK:
        return "成功";
    case FR_DISK_ERR:
        return "ディスクI/Oエラー";
    case FR_INT_ERR:
        return "内部エラー";
    case FR_NOT_READY:
        return "ドライブ未準備";
    case FR_NO_FILE:
        return "ファイルが見つかりません";
    case FR_NO_PATH:
        return "パスが見つかりません";
    case FR_INVALID_NAME:
        return "無効なファイル名";
    case FR_DENIED:
        return "アクセス拒否";
    case FR_EXIST:
        return "ファイル既存";
    case FR_INVALID_OBJECT:
        return "無効なオブジェクト";
    case FR_WRITE_PROTECTED:
        return "書き込み保護";
    case FR_INVALID_DRIVE:
        return "無効なドライブ";
    case FR_NOT_ENABLED:
        return "ワークエリア未設定";
    case FR_NO_FILESYSTEM:
        return "有効なFATボリュームなし";
    case FR_MKFS_ABORTED:
        return "f_mkfs中断";
    case FR_TIMEOUT:
        return "アクセスタイムアウト";
    case FR_LOCKED:
        return "ファイルロック中";
    case FR_NOT_ENOUGH_CORE:
        return "LFNワークバッファ不足";
    case FR_TOO_MANY_OPEN_FILES:
        return "オープンファイル数上限";
    case FR_INVALID_PARAMETER:
        return "不正なパラメータ";
    default:
        return "不明なFRESULTエラー";
    }
}

/**
 * @brief データをSDカードに保存
 * @param data_buffer データバッファ配列
 * @param data_count データ数
 * @param hrtc RTCハンドル
 * @retval app_error_t エラーコード
 */
app_error_t data_save_to_sd(const DataBuffer_t *data_buffer, uint32_t data_count,
                            RTC_HandleTypeDef *hrtc)
{
    char filename[MAX_FILENAME_LENGTH];
    char buffer[DATA_WRITE_BUFFER_SIZE];
    UINT bw;

    // パラメータ検証
    APP_ERROR_CHECK(data_buffer != NULL, APP_ERROR_INVALID_PARAM, "data_buffer is NULL");
    APP_ERROR_CHECK(hrtc != NULL, APP_ERROR_INVALID_PARAM, "hrtc is NULL");
    APP_ERROR_CHECK(data_count > 0, APP_ERROR_INVALID_PARAM, "data_count is zero");

    // SDカード状態チェック
    if (!data_check_sd_status())
    {
        APP_ERROR_REPORT(APP_ERROR_SD_NOT_READY, "SD card is not ready");
        return APP_ERROR_SD_NOT_READY;
    }

    // 詳細なSDカード状態チェック
    DSTATUS disk_status = SD_disk_status(0);
    if (disk_status != 0)
    {
        char status_msg[64];
        snprintf(status_msg, sizeof(status_msg), "SD card disk status error: 0x%02X", disk_status);
        APP_ERROR_REPORT(APP_ERROR_SD_NOT_READY, status_msg);
        return APP_ERROR_SD_NOT_READY;
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
            data_check_sd_status();
        }

        retry_count++;
    } while (retry_count < max_retries);

    if (res != FR_OK)
    {
        // より詳細なエラー情報を提供
        char error_msg[192];
        snprintf(error_msg, sizeof(error_msg),
                 "Failed to open file after %d retries: %s (FRESULT: %d - %s)",
                 max_retries, filename, res, fresult_to_string(res));
        APP_ERROR_REPORT(APP_ERROR_SD_WRITE_FAILED, error_msg);
        return APP_ERROR_SD_WRITE_FAILED;
    }

    // ヘッダー書き込み
    sprintf(buffer, "時刻,温度(°C),圧力(Pa)\r\n");
    FRESULT write_result = f_write(&fil, buffer, strlen(buffer), &bw);
    if (write_result != FR_OK)
    {
        f_close(&fil);
        f_unlink(filename);
        APP_ERROR_REPORT(APP_ERROR_SD_WRITE_FAILED, "Failed to write header to file");
        return APP_ERROR_SD_WRITE_FAILED;
    }

    // データ書き込み
    for (uint32_t i = 0; i < data_count; i++)
    {
        sprintf(buffer, "%lu,%.2f,%.2f\r\n",
                (unsigned long)data_buffer[i].timestamp,
                data_buffer[i].temp_processed_data,
                data_buffer[i].press_processed_data);

        write_result = f_write(&fil, buffer, strlen(buffer), &bw);
        if (write_result != FR_OK)
        {
            f_close(&fil);
            f_unlink(filename);
            APP_ERROR_REPORT(APP_ERROR_SD_WRITE_FAILED, "Failed to write data to file");
            return APP_ERROR_SD_WRITE_FAILED;
        }
    }

    f_close(&fil);
    return APP_ERROR_NONE;
}

/**
 * @brief SDカードの状態チェックと再初期化
 * @retval bool SDカードが使用可能かどうか
 */
bool data_check_sd_status(void)
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