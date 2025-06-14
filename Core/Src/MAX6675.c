/**
 ******************************************************************************
 * @file    MAX6675.c
 * @brief   MAX6675温度センサーのドライバー
 * @author  Your Name
 * @date    YYYY-MM-DD
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "MAX6675.h"
#include <stdio.h>
#include <stdint.h>

/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions --------------------------------------------------------*/

MAX6675_Temp_t Max6675_Read_Temp(SPI_HandleTypeDef *hspi)
{
    uint8_t rx[2] = {0};
    uint8_t dummy[2] = {0x00, 0x00};
    HAL_StatusTypeDef status;
    MAX6675_Temp_t result = {0, 0};

    // CS LOW to start communication
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);

    // Small delay for CS setup time (increased for stability)
    HAL_Delay(10);

    // SPI communication
    status = HAL_SPI_TransmitReceive(hspi, dummy, rx, 2, 1000);

    // CS HIGH to end communication
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);

    // Debug: Print raw data
    printf("SPI Status: %d, Raw Data: 0x%02X 0x%02X\r\n", status, rx[0], rx[1]);

    result.temp_int = ((rx[0] << 8) | rx[1]) >> 4; // 12bitデータ取得
    result.temp_dec = (rx[1] & 0x0C) >> 2;         // 小数部（0.25の倍数）

    printf("Integer Part: %d, Decimal Part: %d\r\n", result.temp_int, result.temp_dec);

    // Check SPI communication status
    if (status != HAL_OK)
    {
        printf("SPI通信失敗 Status: %d\r\n", status);
        result.temp_int = -999;
        result.temp_dec = 0;
        return result;
    }

    // Check thermocouple connection (bit D2)
    if ((rx[1] & 0x04))
    {
        printf("サーモカップルオープン検出\r\n");
        result.temp_int = -1;
        result.temp_dec = 0;
        return result;
    }

    return result;
}