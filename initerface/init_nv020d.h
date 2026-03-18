#ifndef INIT_NV020D_H
#define INIT_NV020D_H

#include "FreeRTOS.h"
#include "task.h"
#include "gpio.h"
#include "init_ds1302z.h"

#define NV020D_CLK_H HAL_GPIO_WritePin(NVD_CLK_GPIO_Port, NVD_CLK_Pin, GPIO_PIN_SET)
#define NV020D_CLK_L HAL_GPIO_WritePin(NVD_CLK_GPIO_Port, NVD_CLK_Pin, GPIO_PIN_RESET)
#define NV020D_DATA_H HAL_GPIO_WritePin(NVD_SDA_GPIO_Port, NVD_SDA_Pin, GPIO_PIN_SET)
#define NV020D_DATA_L HAL_GPIO_WritePin(NVD_SDA_GPIO_Port, NVD_SDA_Pin, GPIO_PIN_RESET)
#define NV020D_DATA_Read HAL_GPIO_ReadPin(NVD_SDA_GPIO_Port, NVD_SDA_Pin)

#define NV020D_CTR_H HAL_GPIO_WritePin(NVD_CLK_GPIO_Port, NVD_CLK_Pin, GPIO_PIN_SET)
#define NV020D_CTR_L HAL_GPIO_WritePin(NVD_CLK_GPIO_Port, NVD_CLK_Pin, GPIO_PIN_RESET)
/**
 * @brief  初始化延时
 */
void NV020D_Init(void);

void NV020D_send_byte(uint8_t byte);

/**
 * @brief  发送命令给NV020D模块
 * @param  cmd: 要发送的命令
 */
void NV020D_send_cmd(uint8_t cmd);

/**
 * @brief  设置NV020D模块的音量
 * @param  volume: 音量值，范围为0-7
 */
void nv020d_set_volume(uint8_t volume);

/**
 * @brief  停止NV020D模块的播放
 */
void nv020d_stop(void);

#endif  /* INIT_NV020D_H */

