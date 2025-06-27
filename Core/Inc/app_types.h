/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : app_types.h
 * @brief          : Application Types and Structures
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

#ifndef __APP_TYPES_H
#define __APP_TYPES_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

    /* Application Types ----------------------------------------------------------*/

    /**
     * @brief 統一エラーハンドリング用のエラーコード
     */
    typedef enum
    {
        APP_ERROR_NONE = 0,              ///< 成功
        APP_ERROR_INVALID_PARAM = -1,    ///< 不正なパラメータ
        APP_ERROR_BUFFER_TOO_SMALL = -2, ///< バッファサイズ不足
        APP_ERROR_TIMEOUT = -3,          ///< タイムアウト
        APP_ERROR_BUSY = -4,             ///< リソースビジー
        APP_ERROR_NOT_READY = -5,        ///< リソース未準備
        APP_ERROR_MEMORY = -6,           ///< メモリエラー
        APP_ERROR_CHECKSUM = -7,         ///< チェックサムエラー
        APP_ERROR_INVALID_FORMAT = -8,   ///< 不正なフォーマット

        // 機能別エラーコード
        APP_ERROR_SENSOR_READ_FAILED = -100,  ///< センサー読み取り失敗
        APP_ERROR_SENSOR_TIMEOUT = -101,      ///< センサータイムアウト
        APP_ERROR_SENSOR_INVALID_DATA = -102, ///< センサーデータ不正

        APP_ERROR_SD_INIT_FAILED = -200,  ///< SDカード初期化失敗
        APP_ERROR_SD_WRITE_FAILED = -201, ///< SDカード書き込み失敗
        APP_ERROR_SD_READ_FAILED = -202,  ///< SDカード読み取り失敗
        APP_ERROR_SD_NOT_READY = -203,    ///< SDカード未準備

        APP_ERROR_UART_INIT_FAILED = -300,    ///< UART初期化失敗
        APP_ERROR_UART_SEND_FAILED = -301,    ///< UART送信失敗
        APP_ERROR_UART_RECEIVE_FAILED = -302, ///< UART受信失敗
        APP_ERROR_UART_TIMEOUT = -303,        ///< UARTタイムアウト

        APP_ERROR_COMM_ENCODE_FAILED = -400,  ///< 通信エンコード失敗
        APP_ERROR_COMM_DECODE_FAILED = -401,  ///< 通信デコード失敗
        APP_ERROR_COMM_PROTOCOL_ERROR = -402, ///< 通信プロトコルエラー

        APP_ERROR_SYSTEM_INIT_FAILED = -500, ///< システム初期化失敗
        APP_ERROR_INVALID_DATA = -501,       ///< 不正なデータ
        APP_ERROR_NOT_IMPLEMENTED = -502     ///< 未実装機能
    } app_error_t;

    /**
     * @brief エラー詳細情報構造体
     */
    typedef struct
    {
        app_error_t error_code; ///< エラーコード
        uint32_t line;          ///< エラー発生行
        const char *file;       ///< エラー発生ファイル
        const char *function;   ///< エラー発生関数
        const char *message;    ///< エラーメッセージ
        uint32_t timestamp;     ///< エラー発生時刻（システムティック）
    } app_error_detail_t;

    /**
     * @brief エラーハンドラー関数ポインタ型
     */
    typedef void (*app_error_handler_t)(const app_error_detail_t *error_detail);

    /**
     * @brief データバッファ構造体
     */
    typedef struct
    {
        uint32_t timestamp;
        float temp_processed_data;  // 処理済み温度（摂氏）
        float press_processed_data; // 処理済み圧力（Pa）
    } DataBuffer_t;

    /**
     * @brief 通信データ構造体
     */
    typedef struct
    {
        int16_t temperature;
        uint16_t pressure;
    } CommData_t;

    /**
     * @brief バルブ制御状態管理構造体
     */
    typedef struct
    {
        bool valve_operation_active;
        uint32_t valve_operation_start_time;
        bool button_pressed_last;
    } ValveControl_t;

    /**
     * @brief センサーデータ検証結果構造体
     */
    typedef struct
    {
        bool is_valid;
        float temperature;
        float pressure;
        app_error_t validation_error; ///< 検証エラー詳細
    } SensorValidationResult_t;

    /* Exported Functions --------------------------------------------------------*/

    /**
     * @brief エラーコードを文字列に変換
     * @param error_code エラーコード
     * @return エラーメッセージ文字列
     */
    const char *app_error_to_string(app_error_t error_code);

    /**
     * @brief HAL_StatusTypeDefをapp_error_tに変換
     * @param hal_status HALステータス
     * @return アプリケーションエラーコード
     */
    app_error_t app_error_from_hal_status(HAL_StatusTypeDef hal_status);

    /**
     * @brief エラーハンドラーを設定
     * @param handler エラーハンドラー関数
     */
    void app_error_set_handler(app_error_handler_t handler);

    /**
     * @brief エラーを報告
     * @param error_code エラーコード
     * @param file ファイル名
     * @param line 行番号
     * @param function 関数名
     * @param message エラーメッセージ
     */
    void app_error_report(app_error_t error_code, const char *file, uint32_t line,
                          const char *function, const char *message);

    /**
     * @brief 最後のエラー詳細を取得
     * @return 最後のエラー詳細情報
     */
    const app_error_detail_t *app_error_get_last_detail(void);

/* Error Handling Macros -----------------------------------------------------*/

/**
 * @brief エラー報告マクロ
 */
#define APP_ERROR_REPORT(error_code, message) \
    app_error_report((error_code), __FILE__, __LINE__, __func__, (message))

/**
 * @brief エラーチェックマクロ（falseの場合エラー報告）
 */
#define APP_ERROR_CHECK(condition, error_code, message) \
    do                                                  \
    {                                                   \
        if (!(condition))                               \
        {                                               \
            APP_ERROR_REPORT((error_code), (message));  \
            return (error_code);                        \
        }                                               \
    } while (0)

/**
 * @brief HALステータスチェックマクロ
 */
#define APP_HAL_CHECK(hal_call, message)                                       \
    do                                                                         \
    {                                                                          \
        HAL_StatusTypeDef __hal_status = (hal_call);                           \
        if (__hal_status != HAL_OK)                                            \
        {                                                                      \
            app_error_t __app_error = app_error_from_hal_status(__hal_status); \
            APP_ERROR_REPORT(__app_error, (message));                          \
            return __app_error;                                                \
        }                                                                      \
    } while (0)

#ifdef __cplusplus
}
#endif

#endif /* __APP_TYPES_H */