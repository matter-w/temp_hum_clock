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
 * @brief 鎺у埗�?诲紑鍏�
 *
 */
void Inf_led_start(void);

/**
 * @brief 鍏抽棴鎬诲紑鍏�
 *
 */
void Inf_led_stop(void);

/**
 * @brief 鍐欏叆鏁版嵁
 * 
 * @param p00 VCCP00 鐢垫�?鎺у埗
 * @param p01 VCCP01 鐢垫�?鎺у埗
 * @param p02 VCCP02 鐢垫�?鎺у埗
 * @param data 涓や釜椹卞姩鑺�鐗囩殑鏁版嵁
 */
void Inf_led_write(uint8_t p00,uint8_t p01,uint8_t p02,uint32_t data);


/**
 * @brief 璁剧疆鏃堕挓
 * 
 * @param h 灏忔�?
 * @param m 鍒嗛�?
 * @param clock_flag 鏃堕挓闂�鐑佹爣蹇�
 */
void Inf_led_set_clock(uint8_t h,uint8_t m,uint8_t clock_flag);

/**
 * @brief 鎶婂悇椤规暟鎹�鏄剧ず鍦╨ed涓�
 */
void Inf_led_set_all(uint8_t h, uint8_t m, int8_t temperature,
     int8_t humidity,uint8_t clock_flag,  show_type_struct *show_type);

/**
 * @brief 杩涘叆璁剧疆鏃堕棿椤甸潰led鏄剧ず鐨�?唴瀹�
 * @param time_set 鏄剧ず涓嶅悓椤甸�?
 * @param value 鏄剧ず鐨�?叿浣撳€�?
 */
void Inf_led_time_set(show_time_set time_set,uint16_t value);


#endif // __INF_LED__
