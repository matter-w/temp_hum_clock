#include "APP_switch.h"
#include "init_key.h"
#include "init_led.h"
#include "init_mic.h"
#include "APP_show.h"
extern show_time_set time_set;//设置页面
extern show_mode_type show_mode;//展示模式
extern show_type_struct show_type;  //展示类型
extern uint8_t led_en;//led总开关
extern show_alarm_set alarm_set;
extern Clock_Date_time_type datetime;
extern alarm_time_type alarm_time1;//存储闹钟1信息
extern alarm_time_type alarm_time2;//存储闹钟2信息
uint8_t switch_alarm_en=0;//切换闹钟类型
uint8_t sound_control_flag=0;//声控标志
uint32_t sound_control_last_time=0;//检测到声音后计时（灯亮五秒）

void APP_switch_key_process(void)
{
    //获取按键开关的值
    Key_type_value key_value=init_get_key_value();
    //根据不同的按键值处理不同时间
    switch(key_value)
    {
        //短按时间设置->切换12/24小时制
        case KEY_TIME_SET:
        show_type.is_12_hour=!show_type.is_12_hour;
        break;

        //短按温度显示->切换摄氏度/华氏度
        case KEY_UP:
        show_type.is_celsius=!show_type.is_celsius;
        break;

        case KEY_TIME_SET_LONG:
        //进入时间设置模块 
        show_mode=show_mode_time_set;
        break;

        case KEY_DOWN:
        //播放音频，调节音量
        show_mode=show_mode_volume_set;
        break;

        case KEY_ALARM_SET:
        //播放音频，调节音量        
        show_mode=show_mode_volume_set;
        break;
 
        case KEY_ALARM_SET_LONG:
        //进入闹钟设置模块
        show_mode=show_mode_alarm_set;
        show_type.is_12_hour=0;
        show_type.is_alarm_1=1;
        show_type.is_alarm_2=0; 
        break;

        case KEY_ALARM_EN:
         //闹钟使能
         switch_alarm_en++;
         if(switch_alarm_en%2==0)
         {
            show_type.is_alarm_1=0;
            show_type.is_alarm_2=0;
         }
         else if(switch_alarm_en==1)
         {
             show_type.is_alarm_1=1;
         }
         else if(switch_alarm_en==3)
         {
             show_type.is_alarm_2=1;
         }
         else if(switch_alarm_en==5)
         {
            show_type.is_alarm_1=1;
            show_type.is_alarm_2=1;

         }
         else if(switch_alarm_en==6)
         {
            show_type.is_alarm_1=0;
            show_type.is_alarm_2=0;
            switch_alarm_en=0;
         }
        break;

        case KEY_ALARM_5:  //五天闹钟使能
         show_type.is_alarm_five_day=!show_type.is_alarm_five_day;
        break;
        default:
        break;

    }
}

void APP_switch_rotary_process(void)
{ 
    //读取拨动开关当前值 
   LED_ON_Type_value led_on_value= get_led_value();//led_on
   //当为light_off时为声控模式
   Light_type_value light_type_value = get_light_value();//light

   //读取声控值
   mic_value mic_v= get_mic_value();
   if(mic_v==MIC_ON)//检测到声音
   {
    sound_control_flag=1;//标志位置为1
    sound_control_last_time=xTaskGetTickCount();//记录当前系统时间

   }
   if(led_on_value==LED_ON)
   {
    //判断常亮或者声控模式

    if(light_type_value==LIGHT_ON)//常亮模式
    {
        led_en=1;
    }else//声控模式
    {
        if(sound_control_flag==1)//标志位为1则开始亮灯五秒
        {
            if(xTaskGetTickCount()-sound_control_last_time<=5000)
            {
                led_en=1;
            }else{
                led_en=0;
                sound_control_flag=0;
            }
        }
    }
    
   }
   else
   {
    led_en=0;
   }

}

void APP_switch_time_set_process(void)
{
    Key_type_value key_v= init_get_key_value();//获取按键值
    switch (key_v)
    {
        case KEY_UP:
        //当前页面值加1
        if(time_set==time_set_year)
        {
            datetime.year++;
            if(datetime.year>2099)
            {
                datetime.year=2000;
            }
        }
        else if(time_set==time_set_month)
        {
            datetime.month++;
            if(datetime.month>12)
            {
                datetime.month=1;
            }
        }
        else if(time_set==time_set_day)
        {
            datetime.date++;
            if(datetime.date>31)
            {
                datetime.date=1;
            }
        }
        else if(time_set==time_set_hour)
        {
            datetime.hour++;
           
            if(datetime.hour>23)
            {
                datetime.hour=0;
            }
            
        }
        else if(time_set==time_set_minute)
        {
         datetime.minute++;
         if(datetime.minute>59)
         {
            datetime.minute=0;
         }   
        }
        break;


        case KEY_DOWN:
        //当前页面值减1
         if(time_set==time_set_year)
        {
            datetime.year--;
            if(datetime.year<2000)
            {
                datetime.year=2099;
            }
        }
        else if(time_set==time_set_month)
        {
            datetime.month--;
            if(datetime.month<1)
            {
                datetime.month=12;
            }
        }
        else if(time_set==time_set_day)
        {
            datetime.date--;
            if(datetime.date<1)
            {
                datetime.date=31;
            }
        }
        else if(time_set==time_set_hour)
        {
            datetime.hour--;
            if(datetime.hour<0)
            {
                datetime.hour=23;
            }
        }
        else if(time_set==time_set_minute)
        {
         datetime.minute--;
         if(datetime.minute<0)
         {
            datetime.minute=59;
         }   
        }
        break;
        case KEY_TIME_SET:
        //进入到下一个页面的设置
        time_set++;
        if(time_set==time_set_end)
        {
            //退出时间设置页面
            show_mode=show_mode_normal;
            //设置时间
            datetime.is_12h_format=show_type.is_12_hour;
            datetime.is_pm=show_type.is_pm;
            app_datetime_set(&datetime);
            time_set=time_set_year;
        }
        break;
        default:
        break;        
    }
}

void APP_switch_alarm_set_process(void)
{
    Key_type_value key_v=init_get_key_value();
    switch(key_v)
    {
        //设置闹钟时间+1
        case KEY_UP:
        if(alarm_set==alarm_set_hour)
        {
            if(show_type.is_alarm_1==1)
            {
                alarm_time1.hour++;
                if(alarm_time1.hour>23)
                {
                    alarm_time1.hour=0;
                }
            }
            else if(show_type.is_alarm_2==1)
            {
                alarm_time2.hour++;
                if(alarm_time2.hour>23)
                {
                    alarm_time2.hour=0;
                }
            }
        }
        else if(alarm_set==alarm_set_min)
        {
            if(show_type.is_alarm_1==1)
            {
                alarm_time1.min++;
                if(alarm_time1.min>59)
                {
                    alarm_time1.min=0;
                }
            }
            else if(show_type.is_alarm_2==1)
            {   
            if(show_type.is_alarm_2==1)
            {
                alarm_time2.min++;
                if(alarm_time2.min>59)
                {
                    alarm_time2.min=0;
                }
            }
            }
        }
        break;
        
        case KEY_DOWN:
        //设置闹钟时间-1
        if(alarm_set==alarm_set_hour)
        {
            if(show_type.is_alarm_1==1)
            {
                alarm_time1.hour--;
                if(alarm_time1.hour<0)
                {
                    alarm_time1.hour=23;
                }
            }
            else if(show_type.is_alarm_2==1)
            {
                alarm_time2.hour--;
                if(alarm_time2.hour<0)
                {
                    alarm_time2.hour=23;
                }
            }
        }
        else if(alarm_set==alarm_set_min)
        {
            if(show_type.is_alarm_1==1)
            {
                alarm_time1.min--;
                if(alarm_time1.min<0)
                {
                    alarm_time1.min=59;
                }
            }
            else if(show_type.is_alarm_2==1)
            {   
            if(show_type.is_alarm_2==1)
            {
                alarm_time2.min--;
                if(alarm_time2.min<0)
                {
                    alarm_time2.min=59;
                }
            }
            }
        }
   
        break;
        //确认时间进去下一级配置
        case KEY_ALARM_SET:
        alarm_set++;
        if(alarm_set==alarm_set_end)
        {
            //退出设置页面，保存好设置的时间
            if(switch_alarm_en%2==0)
            {
                if(show_type.is_alarm_1==1)
                {
                    switch_alarm_en=1;
                }
                else if(show_type.is_alarm_2==1)
                {
                    switch_alarm_en=3;
                }
            }
            else if(switch_alarm_en==1)
            {
                if(show_type.is_alarm_2==1)
                {
                    switch_alarm_en=5;
                    show_type.is_alarm_1=1;
                }
            }
            else if(switch_alarm_en==3)
            {
                if(show_type.is_alarm_1==1)
                {
                    switch_alarm_en=5;
                    show_type.is_alarm_2=1;
                }
            }
            show_mode=show_mode_normal;
            alarm_set=alarm_set_hour;
        }
        
        break;
        //切换设置闹钟1/2
        case KEY_ALARM_EN:
        if(show_type.is_alarm_1==1)
        {
            show_type.is_alarm_1=0; 
            show_type.is_alarm_2=1; 
        }
        if(show_type.is_alarm_2==1)
        {
            show_type.is_alarm_1=1; 
            show_type.is_alarm_2=0; 
        }
        break;

        default:
        break;
    }
}
