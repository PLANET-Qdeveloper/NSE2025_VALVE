/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : state_manager.h
 * @brief          : System State Management Header
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

#ifndef __STATE_MANAGER_H
#define __STATE_MANAGER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "app_types.h"
#include "stm32f4xx_hal.h"

    /* State Manager Configuration -----------------------------------------------*/


    /**
     * @brief 拡張システム状態構造体
     */
    typedef struct
    {
        uint32_t data_count;
        bool sd_card_ready;
        bool uart_comm_ready;
        ValveControl_t valve_control_state;
    } system_state_t;

    /* Exported Functions --------------------------------------------------------*/

    /**
     * @brief State Managerの初期化
     * @return app_error_t エラーコード
     */
    app_error_t state_manager_init(void);

    /**
     * @brief システム状態の取得（読み取り専用）
     * @return const system_state_t* システム状態への読み取り専用ポインタ
     */
    const system_state_t *state_manager_get_state(void);

    /**
     * @brief システム状態の更新（ループ内で定期実行）
     * @return app_error_t エラーコード
     */
    app_error_t state_manager_update(void);

    /**
     * @brief データカウントの更新
     * @param new_count 新しいデータ数
     * @return app_error_t エラーコード
     */
    app_error_t state_manager_set_data_count(uint32_t new_count);

    /**
     * @brief データカウントの取得
     * @return uint32_t 現在のデータ数
     */
    uint32_t state_manager_get_data_count(void);

    /**
     * @brief エラーカウントの増加
     * @param error_code 発生したエラーコード
     * @return app_error_t エラーコード
     */
    app_error_t state_manager_increment_error_count(app_error_t error_code);

    /**
     * @brief SDカード状態の設定
     * @param ready SDカードが使用可能かどうか
     * @return app_error_t エラーコード
     */
    app_error_t state_manager_set_sd_card_ready(bool ready);

    /**
     * @brief UART通信状態の設定
     * @param ready UART通信が使用可能かどうか
     * @return app_error_t エラーコード
     */
    app_error_t state_manager_set_uart_comm_ready(bool ready);

    /**
     * @brief バルブ制御状態の設定
     * @param valve_state バルブ制御状態
     * @return app_error_t エラーコード
     */
    app_error_t state_manager_set_valve_state(const ValveControl_t *valve_state);

    /**
     * @brief バルブ制御状態の取得
     * @return const ValveControl_t* バルブ制御状態への読み取り専用ポインタ
     */
    const ValveControl_t *state_manager_get_valve_state(void);

    /**
     * @brief システム状態のリセット
     * @return app_error_t エラーコード
     */
    app_error_t state_manager_reset(void);

    /**
     * @brief システム状態のダンプ（デバッグ用）
     */
    void state_manager_dump_state(void);

    /**
     * @brief システム健全性チェック
     * @return bool システムが健全かどうか
     */
    bool state_manager_is_system_healthy(void);

/* State Manager Macros ------------------------------------------------------*/

/**
 * @brief 安全な状態取得マクロ
 */
#define STATE_GET() state_manager_get_state()

#ifdef __cplusplus
}
#endif

#endif /* __STATE_MANAGER_H */