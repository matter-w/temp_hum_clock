#ifndef __INF_LED__
#define __INF_LED__

#include "gpio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "APP_show.h"
#include "APP_freertos.h"

#define LED_CLK_H HAL_GPIO_WritePin(LED_CLK_GPIO_Port, LED_CLK_Pin, GPIO_PIN_SET)
#define LED_CLK_L HAL_GPIO_WritePin(LED_CLK_GPIO_Port, LED_CLK_Pin, GPIO_PIN_RESET)

#define LED_SDI_H HAL_GPIO_WritePin(LED_SDI_GPIO_Port, LED_SDI_Pin, GPIO_PIN_SET)
#define LED_SDI_L HAL_GPIO_WritePin(LED_SDI_GPIO_Port, LED_SDI_Pin, GPIO_PIN_RESET)


#define LED_LE_H HAL_GPIO_WritePin(LED_LE_GPIO_Port, LED_LE_Pin, GPIO_PIN_SET)
#define LED_LE_L HAL_GPIO_WritePin(LED_LE_GPIO_Port, LED_LE_Pin, GPIO_PIN_RESET)


#define LED_OEA_H HAL_GPIO_WritePin(LED_OEA_GPIO_Port, LED_OEA_Pin, GPIO_PIN_SET)
#define LED_OEA_L HAL_GPIO_WritePin(LED_OEA_GPIO_Port, LED_OEA_Pin, GPIO_PIN_RESET)

#define LED_OEB_H HAL_GPIO_WritePin(LED_OEB_GPIO_Port, LED_OEB_Pin, GPIO_PIN_SET)
#define LED_OEB_L HAL_GPIO_WritePin(LED_OEB_GPIO_Port, LED_OEB_Pin, GPIO_PIN_RESET)

extern uint16_t led_clk_h_s[3] ;
extern uint16_t led_clk_h_g[10];
extern uint16_t led_clk_m_s[10] ;
extern uint16_t led_clk_m_g[10];

/**
 * @brief 控制总开关
 *
 */
void Inf_led_start(void);

/**
 * @brief 关闭总开关
 *
 */
void Inf_led_stop(void);

/**
 * @brief 写入数据
 * 
 * @param p00 VCCP00 电源控制
 * @param p01 VCCP01 电源控制
 * @param p02 VCCP02 电源控制
 * @param data 两个驱动芯片的数据
 */
void Inf_led_write(uint8_t p00,uint8_t p01,uint8_t p02,uint32_t data);


/**
 * @brief 设置时钟
 * 
 * @param h 小时
 * @param m 分钟
 * @param clock_flag 时钟闪烁标志
 */
void Inf_led_set_clock(uint8_t h,uint8_t m,uint8_t clock_flag);

/**
 * @brief 把各项数据显示在led上
 */
void Inf_led_set_all(uint8_t h,int8_t m,int8_t temperatu,int8_t humidity,uint8_t clock_flag,show_type_struct* show_type);

/**
 * @brief 进入设置时间页面led显示的内容
 * @param time_set 显示不同页面
 * @param value 显示的具体值
 */
void Inf_led_time_set(show_time_set time_set,uint16_t value);


#endif // __INF_LED__
