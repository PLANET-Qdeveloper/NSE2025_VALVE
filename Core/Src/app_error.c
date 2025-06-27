/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : app_error.c
 * @brief          : Application Error Handling Implementation
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
#include "app_types.h"
#include <stdio.h>
#include <string.h>

/* Private variables ---------------------------------------------------------*/
static app_error_handler_t error_handler = NULL;
static app_error_detail_t last_error_detail = {0};

/* External variables --------------------------------------------------------*/
extern __IO uint32_t uwTick; // HALティック（システム時刻用）

/* Private function prototypes -----------------------------------------------*/
static void default_error_handler(const app_error_detail_t *error_detail);

/* Exported functions --------------------------------------------------------*/

/**
 * @brief エラーコードを文字列に変換
 * @param error_code エラーコード
 * @return エラーメッセージ文字列
 */
const char *app_error_to_string(app_error_t error_code)
{
    switch (error_code)
    {
    case APP_ERROR_NONE:
        return "成功";
    case APP_ERROR_INVALID_PARAM:
        return "不正なパラメータ";
    case APP_ERROR_BUFFER_TOO_SMALL:
        return "バッファサイズ不足";
    case APP_ERROR_TIMEOUT:
        return "タイムアウト";
    case APP_ERROR_BUSY:
        return "リソースビジー";
    case APP_ERROR_NOT_READY:
        return "リソース未準備";
    case APP_ERROR_MEMORY:
        return "メモリエラー";
    case APP_ERROR_CHECKSUM:
        return "チェックサムエラー";
    case APP_ERROR_INVALID_FORMAT:
        return "不正なフォーマット";

    // センサー関連エラー
    case APP_ERROR_SENSOR_READ_FAILED:
        return "センサー読み取り失敗";
    case APP_ERROR_SENSOR_TIMEOUT:
        return "センサータイムアウト";
    case APP_ERROR_SENSOR_INVALID_DATA:
        return "センサーデータ不正";

    // SDカード関連エラー
    case APP_ERROR_SD_INIT_FAILED:
        return "SDカード初期化失敗";
    case APP_ERROR_SD_WRITE_FAILED:
        return "SDカード書き込み失敗";
    case APP_ERROR_SD_READ_FAILED:
        return "SDカード読み取り失敗";
    case APP_ERROR_SD_NOT_READY:
        return "SDカード未準備";

    // UART関連エラー
    case APP_ERROR_UART_INIT_FAILED:
        return "UART初期化失敗";
    case APP_ERROR_UART_SEND_FAILED:
        return "UART送信失敗";
    case APP_ERROR_UART_RECEIVE_FAILED:
        return "UART受信失敗";
    case APP_ERROR_UART_TIMEOUT:
        return "UARTタイムアウト";

    // 通信関連エラー
    case APP_ERROR_COMM_ENCODE_FAILED:
        return "通信エンコード失敗";
    case APP_ERROR_COMM_DECODE_FAILED:
        return "通信デコード失敗";
    case APP_ERROR_COMM_PROTOCOL_ERROR:
        return "通信プロトコルエラー";

    // システム関連エラー
    case APP_ERROR_SYSTEM_INIT_FAILED:
        return "システム初期化失敗";
    case APP_ERROR_INVALID_DATA:
        return "不正なデータ";
    case APP_ERROR_NOT_IMPLEMENTED:
        return "未実装機能";

    default:
        return "不明なエラー";
    }
}

/**
 * @brief HAL_StatusTypeDefをapp_error_tに変換
 * @param hal_status HALステータス
 * @return アプリケーションエラーコード
 */
app_error_t app_error_from_hal_status(HAL_StatusTypeDef hal_status)
{
    switch (hal_status)
    {
    case HAL_OK:
        return APP_ERROR_NONE;
    case HAL_ERROR:
        return APP_ERROR_SYSTEM_INIT_FAILED;
    case HAL_BUSY:
        return APP_ERROR_BUSY;
    case HAL_TIMEOUT:
        return APP_ERROR_TIMEOUT;
    default:
        return APP_ERROR_SYSTEM_INIT_FAILED;
    }
}

/**
 * @brief pq_com_format_result_tをapp_error_tに変換
 * @param comm_result 通信ライブラリ結果
 * @return アプリケーションエラーコード
 */
app_error_t app_error_from_comm_result(int comm_result)
{
    // pq_com_format_result_tの値を直接使用
    switch (comm_result)
    {
    case 0: // PQ_COM_FORMAT_SUCCESS
        return APP_ERROR_NONE;
    case -1: // PQ_COM_FORMAT_ERROR_INVALID_PARAM
        return APP_ERROR_INVALID_PARAM;
    case -2: // PQ_COM_FORMAT_ERROR_BUFFER_TOO_SMALL
        return APP_ERROR_BUFFER_TOO_SMALL;
    case -3: // PQ_COM_FORMAT_ERROR_INVALID_FORMAT
        return APP_ERROR_INVALID_FORMAT;
    case -4: // PQ_COM_FORMAT_ERROR_MEMORY
        return APP_ERROR_MEMORY;
    case -5: // PQ_COM_FORMAT_ERROR_CHECKSUM
        return APP_ERROR_CHECKSUM;
    default:
        return APP_ERROR_COMM_PROTOCOL_ERROR;
    }
}

/**
 * @brief エラーハンドラーを設定
 * @param handler エラーハンドラー関数
 */
void app_error_set_handler(app_error_handler_t handler)
{
    error_handler = handler;
}

/**
 * @brief エラーを報告
 * @param error_code エラーコード
 * @param file ファイル名
 * @param line 行番号
 * @param function 関数名
 * @param message エラーメッセージ
 */
void app_error_report(app_error_t error_code, const char *file, uint32_t line,
                      const char *function, const char *message)
{
    // エラー詳細情報を設定
    last_error_detail.error_code = error_code;
    last_error_detail.line = line;
    last_error_detail.file = file;
    last_error_detail.function = function;
    last_error_detail.message = message;
    last_error_detail.timestamp = uwTick;

    // エラーハンドラーを呼び出し
    if (error_handler != NULL)
    {
        error_handler(&last_error_detail);
    }
    else
    {
        default_error_handler(&last_error_detail);
    }
}

/**
 * @brief 最後のエラー詳細を取得
 * @return 最後のエラー詳細情報
 */
const app_error_detail_t *app_error_get_last_detail(void)
{
    return &last_error_detail;
}

/* Private functions ---------------------------------------------------------*/

/**
 * @brief デフォルトエラーハンドラー
 * @param error_detail エラー詳細情報
 */
static void default_error_handler(const app_error_detail_t *error_detail)
{
    // UARTまたはデバッガー経由でエラー情報を出力
    printf("[エラー] %s (%d)\r\n", app_error_to_string(error_detail->error_code), error_detail->error_code);
    printf("  ファイル: %s:%lu\r\n", error_detail->file, (unsigned long)error_detail->line);
    printf("  関数: %s\r\n", error_detail->function);
    printf("  メッセージ: %s\r\n", error_detail->message);
    printf("  タイムスタンプ: %lu ms\r\n", (unsigned long)error_detail->timestamp);

    // 重大なエラーの判定を改善
    bool is_critical_error = false;

    switch (error_detail->error_code)
    {
    // システム継続不可能な重要エラー
    case APP_ERROR_MEMORY:
    case APP_ERROR_INVALID_PARAM:
    case APP_ERROR_SYSTEM_INIT_FAILED:
        is_critical_error = true;
        break;

    // SDカード関連エラーは警告レベル（システム継続可能）
    case APP_ERROR_SD_INIT_FAILED:
    case APP_ERROR_SD_WRITE_FAILED:
    case APP_ERROR_SD_READ_FAILED:
    case APP_ERROR_SD_NOT_READY:
        printf("警告: SDカードエラーですが、システムは継続動作します。\r\n");
        break;

    // センサー関連エラーも警告レベル
    case APP_ERROR_SENSOR_READ_FAILED:
    case APP_ERROR_SENSOR_TIMEOUT:
    case APP_ERROR_SENSOR_INVALID_DATA:
        printf("警告: センサーエラーですが、システムは継続動作します。\r\n");
        break;

    // その他のエラーも警告レベル
    default:
        printf("警告: エラーが発生しましたが、システムは継続動作します。\r\n");
        break;
    }

    // 本当に重要なエラーのみシステム停止
    if (is_critical_error)
    {
        printf("重大なエラーが発生しました。システムを停止します。\r\n");
        // 実際の組み込みシステムでは、ここでシステムリセットやセーフモードに移行
        while (1)
        {
            // システム停止状態
        }
    }
}