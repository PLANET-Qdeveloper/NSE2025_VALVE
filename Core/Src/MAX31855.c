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

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions --------------------------------------------------------*/

/**
 * @brief MAX31855から温度を取得する関数
 * @param hspi SPIハンドルへのポインタ
 * @retval 温度値（°C）、エラー時は-999.0fまたは-1.0f
 */
float Max31855_Read_Temp(SPI_HandleTypeDef *hspi)
{
    uint8_t rx[4] = {0};
    uint8_t dummy[4] = {0x00, 0x00, 0x00, 0x00};
    HAL_StatusTypeDef status;

    // CS LOW to start communication
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);

    // Small delay for CS setup time
    HAL_Delay(1);

    // SPI communication with 4 bytes (32-bit data)
    status = HAL_SPI_TransmitReceive(hspi, dummy, rx, 4, 1000);

    // CS HIGH to end communication
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);

    // Check SPI communication status
    if (status != HAL_OK)
    {
        printf("SPI通信失敗 Status: %d\r\n", status);
        return -999.0f; // SPI communication error
    }

    // Debug: Print raw data (4 bytes)
    printf("Raw Data: 0x%02X%02X%02X%02X\r\n", rx[0], rx[1], rx[2], rx[3]);

    // 32ビットデータを組み立て
    uint32_t data = ((uint32_t)rx[0] << 24) | ((uint32_t)rx[1] << 16) |
                    ((uint32_t)rx[2] << 8) | rx[3];

    // デバッグ: 32ビットデータの詳細表示
    printf("32-bit Data: 0x%08lX\r\n", (unsigned long)data);
    printf("D31:D18 (Thermocouple): 0x%04lX\r\n", (unsigned long)((data >> 18) & 0x3FFF));
    printf("D17 (Reserved): %ld\r\n", (unsigned long)((data >> 17) & 0x01));
    printf("D16 (Fault): %ld\r\n", (unsigned long)((data >> 16) & 0x01));
    printf("D15:D4 (Reference): 0x%03lX\r\n", (unsigned long)((data >> 4) & 0x0FFF));
    printf("D3 (Reserved): %ld\r\n", (unsigned long)((data >> 3) & 0x01));
    printf("D2 (SCV): %ld, D1 (SCG): %ld, D0 (OC): %ld\r\n",
           (unsigned long)((data >> 2) & 0x01), (unsigned long)((data >> 1) & 0x01), (unsigned long)(data & 0x01));

    // フォルトビットのチェック（D16: 総合フォルト）
    if (data & 0x00010000) // D16: フォルトビット
    {
        printf("フォルト検出:");
        if (data & 0x04) printf(" SCV(Short to VCC)");  // D2
        if (data & 0x02) printf(" SCG(Short to GND)");  // D1
        if (data & 0x01) printf(" OC(Open Circuit)");   // D0
        printf("\r\n");
        
        return -1.0f; // フォルトエラー
    }

    // サーモカップル温度データの抽出（D31:D18の14ビット、符号付き、0.25°C単位）
    int16_t thermocouple_temp = (int16_t)((data >> 18) & 0x3FFF);
    
    // 符号ビットの処理（14ビットの符号ビット）
    if (thermocouple_temp & 0x2000) // 負の値の場合
    {
        thermocouple_temp |= 0xC000; // 符号拡張（16ビットに拡張）
    }

    // リファレンス温度データの抽出（D15:D4の12ビット、符号付き、0.0625°C単位）
    int16_t reference_temp = (int16_t)((data >> 4) & 0x0FFF);
    
    // 符号ビットの処理（12ビットの符号ビット）
    if (reference_temp & 0x0800) // 負の値の場合
    {
        reference_temp |= 0xF000; // 符号拡張（16ビットに拡張）
    }

    // 温度が有効範囲内かチェック（-270°C から +1800°C）
    if (thermocouple_temp < -1080 || thermocouple_temp > 7200) // 0.25°C単位での範囲チェック
    {
        printf("無効な温度データ検出: %d (範囲外)\r\n", thermocouple_temp);
        return -999.0f; // Invalid temperature data
    }

    // 温度を0.25°C単位から実際の温度に変換
    float temperature = (float)thermocouple_temp * 0.25f;
    float ref_temperature = (float)reference_temp * 0.0625f;

    // デバッグ出力
    printf("Thermocouple: %.2f°C, Reference: %.4f°C\r\n", temperature, ref_temperature);

    return temperature;
}
