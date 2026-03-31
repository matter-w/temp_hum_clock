#ifndef APP_SHOW_H
#define APP_SHOW_H


#include "APP_Dateime.h"
#include "FreeRTOS.h"
#include "task.h"
#include "APP_switch.h"

//时钟的模式
typedef enum
{   
    show_mode_normal, //正常显示模式
    show_mode_time_set,//进入时间设置显示
    show_mode_alarm_set,//闹钟设置模式
    show_mode_volume_set,//音量调节模式

}show_mode_type;

//设置时间时的几种页面，依次设置年月日小时分钟
typedef enum 
{
    time_set_year,
    time_set_month,
    time_set_day,
    time_set_hour,
    time_set_minute,
    time_set_end,
}show_time_set;

//设置闹钟时的页面
typedef enum{

    alarm_set_hour,
    alarm_set_min,
    alarm_set_end,
}show_alarm_set;

//页面显示的几个图标
typedef struct 
{
    uint8_t is_12_hour;
    uint8_t is_pm;
    uint8_t is_celsius;
    uint8_t is_alarm_1;
    uint8_t is_alarm_2;
    uint8_t is_alarm_five_day;
    uint8_t is_charging;

}show_type_struct;

/**
 * @brief 初始化/开启led显示
 */
void APP_show_start(void);

/**
 * @brief 正常显示模式
 */
void APP_show_normal(Clock_Date_time_type *datetime,int8_t temperature,
    int8_t humidity,show_type_struct* show_type);

/**
 * @brief 时间设置模式
 */    
void APP_show_time_set(Clock_Date_time_type *clock_date_time,int8_t temperature,int8_t humidity,
    show_type_struct* show_type);

/**
 * @brief 闹钟设置模式
 */    
void APP_show_alarm_set(Clock_Date_time_type *datetime,int8_t temperature,int8_t humidity,
show_type_struct* show_type);
#endif /* APP_SHOW_H */


