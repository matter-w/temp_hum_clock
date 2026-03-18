#ifndef INIT_DHT11_H
#define INIT_DHT11_H

#include "gpio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "init_ds1302z.h"
#include "common.h"

#define DHT11_DATA_H HAL_GPIO_WritePin(DHT11_DATA_GPIO_Port, DHT11_DATA_Pin, GPIO_PIN_SET)
#define DHT11_DATA_L HAL_GPIO_WritePin(DHT11_DATA_GPIO_Port, DHT11_DATA_Pin, GPIO_PIN_RESET)
#define DHT11_DATA_Read HAL_GPIO_ReadPin(DHT11_DATA_GPIO_Port, DHT11_DATA_Pin)
/**
 * @brief  DHT11上电后需要延时1秒钟才能正常工作
 * @param  None
 */
void DHT11_Init(void);

/**
 * @brief  获取温湿度数据
 * @param  temp: 温度值，单位为摄氏度
 * @param  humi: 湿度值，单位为百分比
 */
void DHT11_Get_Temp_Humi(int8_t *temperature, int8_t * humidity);


#endif  /* INIT_DHT11_H */
