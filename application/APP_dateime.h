#ifndef APP_DATEIME_H
#define APP_DATEIME_H

#include "init_ds1302z.h"

//保存时间的结构体
typedef struct
{
    uint16_t year;  //年
    uint8_t month; //月
    uint8_t date;  //日
    int8_t hour;  //时
    int8_t minute;//分     
    uint8_t week;  //星期
    uint8_t is_pm;  //下午
    uint8_t is_12h_format; //是否是12小时制
} Clock_Date_time_type;

//保存设置闹钟的时间的结构体
typedef struct 
{
    int8_t hour;
    int8_t min;
    uint8_t is_on;
    uint8_t is_done;
}alarm_time_type;

/**
 * @brief 获取当前时间:年月日时分星期
 */
void app_datetime_get(Clock_Date_time_type *datetime);

/**
 * @brief 设置所有的时间
 */
void app_datetime_set(Clock_Date_time_type *datetime);
#endif /* APP_DATEIME_H */

