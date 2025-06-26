/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : state_manager.c
 * @brief          : System State Management Implementation
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
#include "state_manager.h"
#include <stdio.h>
#include <string.h>

/* Private variables ---------------------------------------------------------*/
static system_state_t system_state;
static bool initialized = false;

/* External variables --------------------------------------------------------*/
extern __IO uint32_t uwTick; // HALティック

/* Private function prototypes -----------------------------------------------*/

/* Critical Section Macros ---------------------------------------------------*/
#define ENTER_CRITICAL() __disable_irq()
#define EXIT_CRITICAL() __enable_irq()

/* Exported functions --------------------------------------------------------*/

/**
 * @brief State Managerの初期化
 * @return app_error_t エラーコード
 */
app_error_t state_manager_init(void)
{
    // システム状態のクリア
    memset(&system_state, 0, sizeof(system_state_t));

    // 初期値設定（削除されたフィールドの初期化コードを除去）

    initialized = true;

    return APP_ERROR_NONE;
}

/**
 * @brief システム状態の取得（読み取り専用）
 * @return const system_state_t* システム状態への読み取り専用ポインタ
 */
const system_state_t *state_manager_get_state(void)
{
    if (!initialized)
    {
        return NULL;
    }
    return &system_state;
}

/**
 * @brief システム状態の更新（ループ内で定期実行）
 * @return app_error_t エラーコード
 */
app_error_t state_manager_update(void)
{
    APP_ERROR_CHECK(initialized, APP_ERROR_NOT_READY, "State manager not initialized");

    // 基本的な状態更新処理（削除されたフィールドの更新コードを除去）

    return APP_ERROR_NONE;
}

/**
 * @brief データカウントの更新
 * @param new_count 新しいデータ数
 * @return app_error_t エラーコード
 */
app_error_t state_manager_set_data_count(uint32_t new_count)
{
    APP_ERROR_CHECK(initialized, APP_ERROR_NOT_READY, "State manager not initialized");

    ENTER_CRITICAL();
    system_state.data_count = new_count;
    EXIT_CRITICAL();

    return APP_ERROR_NONE;
}

/**
 * @brief データカウントの取得
 * @return uint32_t 現在のデータ数
 */
uint32_t state_manager_get_data_count(void)
{
    if (!initialized)
    {
        return 0;
    }
    return system_state.data_count;
}

/**
 * @brief SDカード状態の設定
 * @param ready SDカードが使用可能かどうか
 * @return app_error_t エラーコード
 */
app_error_t state_manager_set_sd_card_ready(bool ready)
{
    APP_ERROR_CHECK(initialized, APP_ERROR_NOT_READY, "State manager not initialized");

    ENTER_CRITICAL();
    system_state.sd_card_ready = ready;
    EXIT_CRITICAL();

    return APP_ERROR_NONE;
}

/**
 * @brief UART通信状態の設定
 * @param ready UART通信が使用可能かどうか
 * @return app_error_t エラーコード
 */
app_error_t state_manager_set_uart_comm_ready(bool ready)
{
    APP_ERROR_CHECK(initialized, APP_ERROR_NOT_READY, "State manager not initialized");

    ENTER_CRITICAL();
    system_state.uart_comm_ready = ready;
    EXIT_CRITICAL();

    return APP_ERROR_NONE;
}

/**
 * @brief バルブ制御状態の設定
 * @param valve_state バルブ制御状態
 * @return app_error_t エラーコード
 */
app_error_t state_manager_set_valve_state(const ValveControl_t *valve_state)
{
    APP_ERROR_CHECK(initialized, APP_ERROR_NOT_READY, "State manager not initialized");
    APP_ERROR_CHECK(valve_state != NULL, APP_ERROR_INVALID_PARAM, "valve_state is NULL");

    ENTER_CRITICAL();
    memcpy(&system_state.valve_control_state, valve_state, sizeof(ValveControl_t));
    EXIT_CRITICAL();

    return APP_ERROR_NONE;
}

/**
 * @brief バルブ制御状態の取得
 * @return const ValveControl_t* バルブ制御状態への読み取り専用ポインタ
 */
const ValveControl_t *state_manager_get_valve_state(void)
{
    if (!initialized)
    {
        return NULL;
    }
    return &system_state.valve_control_state;
}

/**
 * @brief システム状態のリセット
 * @return app_error_t エラーコード
 */
app_error_t state_manager_reset(void)
{
    return state_manager_init();
}

/**
 * @brief システム状態のダンプ（デバッグ用）
 */
void state_manager_dump_state(void)
{
    if (!initialized)
    {
        printf("State Manager not initialized\r\n");
        return;
    }

    printf("\n=== システム状態ダンプ ===\r\n");
    printf("データ数: %lu\r\n", (unsigned long)system_state.data_count);
    printf("SDカード: %s\r\n", system_state.sd_card_ready ? "Ready" : "Not Ready");
    printf("UART通信: %s\r\n", system_state.uart_comm_ready ? "Ready" : "Not Ready");

    printf("========================\r\n\n");
}

/**
 * @brief システム健全性チェック
 * @return bool システムが健全かどうか
 */
bool state_manager_is_system_healthy(void)
{
    if (!initialized)
    {
        return false;
    }

    // 基本的なヘルスチェック（エラー関連は削除）

    return true;
}

/* Private functions ---------------------------------------------------------*/
