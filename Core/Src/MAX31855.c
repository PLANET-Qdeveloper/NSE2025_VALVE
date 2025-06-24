/**
 ******************************************************************************
 * @file    MAX31855.c
 * @brief   MAX31855温度センサーのドライバー
 * @author  Your Name
 * @date    YYYY-MM-DD
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "MAX31855.h"
#include "main.h"
#include <stdio.h>
#include <stdint.h>

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define MAX31855_TEMP_SCALE 0.25f     // サーモカップル温度スケール（0.25°C/LSB）
#define MAX31855_REF_SCALE 0.0625f    // リファレンス温度スケール（0.0625°C/LSB）
#define MAX31855_TEMP_MIN_RAW -1080   // 最小温度（-270°C = -1080 * 0.25°C）
#define MAX31855_TEMP_MAX_RAW 7200    // 最大温度（1800°C = 7200 * 0.25°C）
#define MAX31855_TEMP_MASK 0x3FFF     // 温度データマスク（14ビット）
#define MAX31855_TEMP_SIGN_BIT 0x2000 // 温度符号ビット
#define MAX31855_TEMP_SIGN_EXT 0xC000 // 温度符号拡張
#define MAX31855_REF_MASK 0x0FFF      // リファレンス温度マスク（12ビット）
#define MAX31855_REF_SIGN_BIT 0x0800  // リファレンス符号ビット
#define MAX31855_REF_SIGN_EXT 0xF000  // リファレンス符号拡張

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions --------------------------------------------------------*/

/**
 * @brief MAX31855から温度を取得する関数
 * @param hspi SPIハンドルへのポインタ
 * @retval MAX31855_Data_t構造体（エラー時は温度に-999.0fが設定される）
 */
MAX31855_Data_t Max31855_Read_Temp(SPI_HandleTypeDef *hspi)
{
    MAX31855_Data_t result = {0};
    uint8_t rx[4] = {0};
    HAL_StatusTypeDef status;

    // CS LOW to start communication
    HAL_GPIO_WritePin(MAX31855_CS_PORT, MAX31855_CS_PIN, GPIO_PIN_RESET);

    // Small delay for CS setup time
    HAL_Delay(1);

    // SPI communication with 4 bytes (32-bit data) - receive only
    status = HAL_SPI_Receive(hspi, rx, 4, 10000);

    // CS HIGH to end communication
    HAL_GPIO_WritePin(MAX31855_CS_PORT, MAX31855_CS_PIN, GPIO_PIN_SET);

    // Check SPI communication status
    if (status != HAL_OK)
    {
        result.processed_data = -999.0f; // SPI communication error
        result.raw_data = 0;
        return result;
    }

    // 32ビットデータを組み立て
    uint32_t data = ((uint32_t)rx[0] << 24) | ((uint32_t)rx[1] << 16) |
                    ((uint32_t)rx[2] << 8) | rx[3];

    // フォルトビットのチェック（D16: 総合フォルト）
    if (data & THERMOCOUPLE_FAULT)
    {
        result.processed_data = -1.0f; // フォルトエラー
        result.raw_data = 0;
        return result;
    }

    // サーモカップル温度データの抽出（D31:D18の14ビット、符号付き、0.25°C単位）
    int16_t thermocouple_temp = (int16_t)((data >> 18) & MAX31855_TEMP_MASK);

    // 符号ビットの処理（14ビットの符号ビット）
    if (thermocouple_temp & MAX31855_TEMP_SIGN_BIT) // 負の値の場合
    {
        thermocouple_temp |= MAX31855_TEMP_SIGN_EXT; // 符号拡張（16ビットに拡張）
    }

    // リファレンス温度データの抽出（D15:D4の12ビット、符号付き、0.0625°C単位）
    int16_t reference_temp = (int16_t)((data >> 4) & MAX31855_REF_MASK);

    // 符号ビットの処理（12ビットの符号ビット）
    if (reference_temp & MAX31855_REF_SIGN_BIT) // 負の値の場合
    {
        reference_temp |= MAX31855_REF_SIGN_EXT; // 符号拡張（16ビットに拡張）
    }

    // 温度が有効範囲内かチェック（-270°C から +1800°C）
    if (thermocouple_temp < MAX31855_TEMP_MIN_RAW || thermocouple_temp > MAX31855_TEMP_MAX_RAW)
    {
        result.processed_data = -999.0f; // Invalid temperature data
        result.raw_data = thermocouple_temp;
        return result;
    }

    // 温度を0.25°C単位から実際の温度に変換
    result.processed_data = (float)thermocouple_temp * MAX31855_TEMP_SCALE;
    result.raw_data = thermocouple_temp;

    return result;
}
