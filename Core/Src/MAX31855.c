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

/**
 * @brief MAX31855から温度を取得する関数
 * @param hspi SPIハンドルへのポインタ
 * @retval float 温度値（摂氏）、エラー時は-999.0f
 */
float MAX31855_Read_Temp(SPI_HandleTypeDef *hspi)
{
    uint8_t rx[4] = {0};
    HAL_StatusTypeDef status;

    // CS LOW to start communication
    HAL_GPIO_WritePin(MAX31855_CS_PORT, MAX31855_CS_PIN, GPIO_PIN_RESET);

    // SPI communication with 4 bytes (32-bit data) - receive only mode
    status = HAL_SPI_Receive(hspi, rx, 4, 100);
    // CS HIGH to end communication
    HAL_GPIO_WritePin(MAX31855_CS_PORT, MAX31855_CS_PIN, GPIO_PIN_SET);

    // Check SPI communication status
    if (status != HAL_OK)
    {
        return -999.0f; // SPI communication error
    }

    // 32ビットデータを組み立て
    uint32_t data = ((uint32_t)rx[0] << 24) | ((uint32_t)rx[1] << 16) |
                    ((uint32_t)rx[2] << 8) | rx[3];

    // フォルトビットのチェック（D16: 総合フォルト）
    if (data & THERMOCOUPLE_FAULT)
    {
        return -1.0f; // フォルトエラー
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
        return -999.0f; // Invalid temperature data
    }

    // 温度を0.25°C単位から実際の温度に変換
    return (float)thermocouple_temp * MAX31855_TEMP_SCALE;
}
