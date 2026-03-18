#ifndef INIT_DS1302Z_H
#define INIT_DS1302Z_H

#include "gpio.h"

#define DS1302Z_SECOND_REX 0X80
#define DS1302Z_MINUTE_REX 0X82
#define DS1302Z_HOUR_REX 0X84
#define DS1302Z_DATE_REX 0X86
#define DS1302Z_MONTH_REX 0X88
#define DS1302Z_WEEK_REX 0X8A
#define DS1302Z_YEAR_REX 0X8C

#define DS1302Z_READ_REX 0X01
#define DS1302Z_WRITE_REX 0X00
#define DS1302Z_CONTRAL_REX 0X8E

//控制引脚
#define DS_CLK_H HAL_GPIO_WritePin(DS_CLK_GPIO_Port, DS_CLK_Pin, GPIO_PIN_SET)
#define DS_CLK_L HAL_GPIO_WritePin(DS_CLK_GPIO_Port, DS_CLK_Pin, GPIO_PIN_RESET)
#define DS_IO_H HAL_GPIO_WritePin(DS_IO_GPIO_Port, DS_IO_Pin, GPIO_PIN_SET)
#define DS_IO_L HAL_GPIO_WritePin(DS_IO_GPIO_Port, DS_IO_Pin, GPIO_PIN_RESET)
#define DS_RST_H HAL_GPIO_WritePin(DS_RST_GPIO_Port, DS_RST_Pin, GPIO_PIN_SET)
#define DS_RST_L HAL_GPIO_WritePin(DS_RST_GPIO_Port, DS_RST_Pin, GPIO_PIN_RESET)
#define DS_IO_READ HAL_GPIO_ReadPin(DS_IO_GPIO_Port, DS_IO_Pin)




/**
 * @brief DS1302Z的延时函数，单位为微秒
 * @param us 延时时间，单位为微秒
 */
void init_delay(uint32_t us);
/**
 * @brief 读取DS1302Z的寄存器值
 * @param rex_addr 寄存器地址
 * @return uint8_t
 */
uint8_t ds1302z_read(uint8_t reg_addr);

/**
 * @brief 向DS1302Z的寄存器写入数据
 * @param rex_addr 寄存器地址
 * @param data 要写入的数据
 */
void ds1302z_write(uint8_t reg_addr, uint8_t data);


#endif /* INIT_DS1302Z_H */
