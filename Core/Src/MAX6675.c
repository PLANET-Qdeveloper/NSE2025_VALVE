/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h>
#include "MAX6675.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions --------------------------------------------------------*/

/**
 * @brief  MAX6675から温度を読み取る
 * @param  None
 * @retval MAX6675_Temperature_t 温度値（整数部分と小数点部分を別々に返す）
 */
MAX6675_Temperature_t MAX6675_ReadTemperature(void)
{
    uint8_t rx[2];
    MAX6675_Temperature_t temp;

    /* CS LOW to start communication */
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);

    /* Small delay for CS setup time (increased for stability) */
    HAL_Delay(10);

    /* SPI communication */
    HAL_SPI_Receive(&hspi1, rx, 2, 1000);

    /* CS HIGH to end communication */
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);

    /* Debug: Print raw data */
    printf("SPI Status: %d, Raw Data: 0x%02X 0x%02X\r\n", HAL_SPI_GetState(&hspi1), rx[0], rx[1]);

    /* 温度の計算 */
    temp.integer = ((rx[0] << 8) | rx[1]) >> 4;
    temp.decimal = ((rx[1] & 0x0C) >> 2);

    /* サーモカップル接続状態の確認 (bit D2) */
    if (temp.integer & 0x0004)
    {
        printf("サーモカップルオープン検出\r\n");
        temp.integer = -1; /* Thermocouple open/disconnected */
        temp.decimal = 0;
    }

    return temp;
}