#ifndef __APP_SWITCH_H
#define __APP_SWITCH_H



/**
 * 处理正常显示模式下按键开关事件
 */
void APP_switch_key_process(void);

/**
 * 处理时间设置模式下按键开关事件
 */
void APP_switch_time_set_process(void);

/**
 * 处理拨动开关事件
 */
void APP_switch_rotary_process(void);

/**
 * @brief 闹钟设置按键开关事件
 */
void APP_switch_alarm_set_process(void);


#endif      

