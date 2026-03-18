#include "APP_show.h"
#include "init_led.h"

uint8_t clock_flag=0;
uint32_t clock_last_time=0;     //用于记录系统时间

extern alarm_time_type alarm_time1;//存储闹钟1信息
extern alarm_time_type alarm_time2;//存储闹钟2信息
extern show_alarm_set alarm_set;
extern show_time_set time_set;

/**
 * @brief 初始化/开启led显示
 */
void APP_show_start(void)
{
    Inf_led_start(); //开启总开关
    clock_last_time = xTaskGetTickCount(); //记录当前时间
} 

/**
 * @brief 正常显示模式
 */
void APP_show_normal(Clock_Date_time_type *datetime,int8_t temperature,
    int8_t humidity,show_type_struct* show_type)
{
    if(xTaskGetTickCount() - clock_last_time >= 500) //每隔500ms切换一次时钟闪烁状态
    {
        clock_flag = !clock_flag; //切换时钟闪烁状态
        clock_last_time = xTaskGetTickCount(); //更新上次切换时间
    }
    //修正小时的值
    uint8_t hour=0;
   if(datetime->is_12h_format&&datetime->is_pm)
   {
    hour = datetime->hour + 12;
   }
   else
   {
    hour = datetime->hour;
   }
   //判断要显示am/pm
   if(show_type->is_12_hour)
   {
    if(hour>=12)
    {
        hour=hour-12;
        show_type->is_pm=1;
    }
    else    
    {
        show_type->is_pm=0;
    }
   }
 
    Inf_led_set_all(hour,datetime->minute,temperature,humidity,clock_flag,show_type);

}

/**
 * @brief 时间设置模式
 */    
void APP_show_time_set(Clock_Date_time_type *datetime,int8_t temperature,
    int8_t humidity,show_type_struct* show_type)
{
  //判断当前处于的页面
  switch (time_set)
  {
    case time_set_year:
    Inf_led_time_set(time_set_year,datetime->year);
    break;

    case time_set_month:
    Inf_led_time_set(time_set_month,datetime->month);
    break;

    case time_set_day:
    Inf_led_time_set(time_set_day,datetime->date);
    break;

    case time_set_hour:
    Inf_led_time_set(time_set_hour,datetime->hour);
    break;

    case time_set_minute:
    Inf_led_time_set(time_set_minute,datetime->minute);
    break;
  
  default:
    break;
  }
}

/**
 * @brief 闹钟设置模式
 */    
void APP_show_alarm_set(Clock_Date_time_type *datetime,int8_t temperature,
    int8_t humidity,show_type_struct* show_type)
{
       if(xTaskGetTickCount() - clock_last_time >= 500) //每隔500ms切换一次时钟闪烁状态
    {
        clock_flag = !clock_flag; //切换时钟闪烁状态
        clock_last_time = xTaskGetTickCount(); //更新上次切换时间
    }
    switch(alarm_set)
    {
        case alarm_set_hour:
        if(clock_flag)
        {
            if(show_type->is_alarm_1)
            {
                Inf_led_set_all(alarm_time1.hour,0xff,1,temperature,humidity,show_type);
            }
            else if(show_type->is_alarm_2)
            {
                Inf_led_set_all(alarm_time2.hour,0xff,1,temperature,humidity,show_type);
            }
        }else
        {
            Inf_led_set_all(0xff,0xff,1,temperature,humidity,show_type);
        } 
        break;

        case alarm_set_min:
        if(clock_flag)
        {
            if(show_type->is_alarm_1)
            {
                Inf_led_set_all(0xff,alarm_time1.min,1,temperature,humidity,show_type);
            }
            else if(show_type->is_alarm_2)
            {
                Inf_led_set_all(0xff,alarm_time2.min,1,temperature,humidity,show_type);
            }
        }else{
             Inf_led_set_all(0xff,0xff,1,temperature,humidity,show_type);
        }
        break;

        case alarm_set_end:

        break;
        default:
        break;
    }
}
