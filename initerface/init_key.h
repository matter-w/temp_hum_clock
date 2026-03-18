#ifndef INIT_KEY_H
#define INIT_KEY_H

#include "gpio.h"
#include "common.h"
#include "FreeRTOS.h"
#include "task.h"
typedef enum 
{
    KEY_NONE = 0,       // 按键未按下
    KEY_TIME_SET,       // 表示时间设置按键开关短按->进行24小时/12小时切换
    KEY_TIME_SET_LONG,  // 表示时间设置开关长按->进入时间设置模式
    KEY_ALARM_SET,      // 闹铃设置短按->播放音乐
    KEY_ALARM_SET_LONG, // 闹钟设置长按->闹铃设置
    KEY_UP,             // 上调->切换华氏度和摄氏度，在时间设置模式下调节时间
    KEY_DOWN,           // 下调
    KEY_ALARM_EN,       // 闹钟使能
    KEY_ALARM_5,        // 五天闹钟使能
}Key_type_value;

typedef enum 
{
    LED_OFF = 0,        // LED 灭
    LED_ON,             // LED 亮

}LED_ON_Type_value;

typedef enum 
{
    LIGHT_OFF = 0,      // 背光灯灭
    LIGHT_ON,           // 背光灯亮
} Light_type_value;
/**
 * @brief 获取按键值
 * @return Key_type_value 按键值
 */
Key_type_value init_get_key_value(void);

/**
 * @brief 获取LED状态
 * @return LED_ON_Type_value LED状态
 */
LED_ON_Type_value get_led_value(void);

/**
 * @brief 获取背光灯状态
 * @return Light_type_value 背光灯状态
 */
Light_type_value get_light_value(void);

#endif /* INIT_KEY_H */

