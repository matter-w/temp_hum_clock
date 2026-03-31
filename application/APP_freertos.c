#include "APP_freertos.h"
#include "task.h"
//任务task_collect相关配置
#define task_collect_PRIORITY 1         //优先级
#define task_collect_STACK_SIZE 128     //堆栈大小
void  task_collect(void* pvParameters); //任务函数声明
TaskHandle_t task_collect_Handle = NULL;//任务句柄

//任务task_show相关配置
#define task_show_PRIORITY 2         //优先级
#define task_show_STACK_SIZE 128     //堆栈大小
void  task_show(void* pvParameters); //任务函数声明
TaskHandle_t task_show_Handle = NULL;//任务句柄

//任务task_switch相关配置
#define task_switch_PRIORITY 3        //优先级
#define task_switch_STACK_SIZE 128     //堆栈大小
void task_switch(void* pvParameters);   //任务函数声明
TaskHandle_t task_switch_Handle=NULL;   //任务句柄

//任务task_alarm相关配置
#define task_alarm_PRIORITY 4
#define task_alarm_STACK_SIZE 128
void task_alarm(void* pvParmeters);
TaskHandle_t task_alarm_Handle=NULL;   //任务句柄

//任务task_volume相关配置
#define task_volume_PRIORITY 4
#define task_volume_STACK_SIZE 128
void task_volume(void* pvParmeters);
TaskHandle_t task_volume_Handle=NULL;   //任务句柄


//任务task_debug相关配置
#define task_debug_PRIORITY 4
#define task_debug_STACK_SIZE 128
void task_debug(void* pvParmeters);
TaskHandle_t task_debug_Handle=NULL;   //任务句柄



//闹钟信息存储变量
alarm_time_type alarm_time1={0};//存储闹钟1信息
alarm_time_type alarm_time2={0};//存储闹钟2信息

//触摸开关计数值
uint8_t touch_count=0;
uint8_t alarm_touch_count=0;

//全局变量保存温湿度的值；
int8_t temperature = 0;
int8_t humidity = 0;

//全局变量保存时间的值
Clock_Date_time_type datetime;

//音量大小
int8_t nv020d_volume=5;

//状态
show_mode_type show_mode = show_mode_normal; //显示模式

show_time_set time_set=time_set_year;//时间设置模式

show_alarm_set alarm_set=alarm_set_hour;
//12/24小时制、上下午、华氏度和摄氏度、闹钟1、2、闹钟五天、充电标识
show_type_struct show_type={.is_12_hour=0,.is_pm=0,.is_celsius=1,.is_alarm_1=0,
.is_alarm_2=0,.is_alarm_five_day=0,.is_charging=0};

//LED总开关
uint8_t led_en=1;

void APP_FREERTOS_Start(void)
{
    
    //获取时间和温湿度任务
    xTaskCreate(task_collect, "task_collect", task_collect_STACK_SIZE, 
    NULL, task_collect_PRIORITY, &task_collect_Handle);

    //展示任务
    xTaskCreate(task_show, "task_show", task_show_STACK_SIZE,
    NULL, task_show_PRIORITY, &task_show_Handle);

    //开关监控任务
    xTaskCreate(task_switch,"task_switch",task_switch_STACK_SIZE,
    NULL,task_switch_PRIORITY,&task_switch_Handle);

    //闹钟任务
    xTaskCreate(task_alarm,"task_alarm",task_alarm_STACK_SIZE,
    NULL,task_alarm_PRIORITY,&task_alarm_Handle);

    //调节音量任务
    xTaskCreate(task_volume,"task_volume",task_volume_STACK_SIZE,
    NULL,task_volume_PRIORITY,&task_volume_Handle);

//测试任务
    xTaskCreate(task_debug,"task_debug",task_debug_STACK_SIZE,
    NULL,task_debug_PRIORITY,&task_debug_Handle);
    //vTaskStartScheduler()函数会启动调度器，开始执行任务。它会一直运行，直到系统关闭或发生错误。
    vTaskStartScheduler();
}

//task_collect任务函数实现
void  task_collect(void* pvParameters)
{
    DHT11_Init();//温湿度传感器初始化
    uint8_t task_collect_count = 9;
    while(1)
    {
        task_collect_count++;
        if(task_collect_count >= 10) //每10秒读取一次温湿度数据
        {
            task_collect_count = 0;
            //周期性读取温湿度传感器的值，并保存到全局变量中
            DHT11_Get_Temp_Humi(&temperature, &humidity);
        }
        if(show_mode!=show_mode_time_set)
        {
        //周期性读取时间，并保存到全局变量中
        app_datetime_get(&datetime);
        }
        vTaskDelay(1000); //每隔1秒读取一次数据

    }

}

//task_show任务函数实现
void  task_show(void* pvParameters)
{
  
    APP_show_start();
    while(1)
    {
        if(led_en==1)
        {
            if(show_mode == show_mode_normal||show_mode==show_mode_volume_set)
        {
            APP_show_normal(&datetime,temperature,humidity,&show_type);
        }
        else if(show_mode == show_mode_time_set){
           APP_show_time_set(&datetime,temperature,humidity,&show_type);
        }
        else if(show_mode==show_mode_alarm_set)
        {
            APP_show_alarm_set(&datetime,temperature,humidity,&show_type);
        }
        }
        else{
            Inf_led_stop();
        }
         
    }

}

//task_switch任务函数实现
void task_switch(void* pvParameters)
{
    while(1)
    {
        //根据不同的模式处理不同的按键逻辑

        //正常显示模式的逻辑
        if(show_mode==show_mode_normal){
        APP_switch_key_process();
        }
        //时间设置模式
        else if(show_mode==show_mode_time_set){
            APP_switch_time_set_process();

        }
        //闹钟设置模式
        else if(show_mode==show_mode_alarm_set)
        {
            APP_switch_alarm_set_process();
        }
        //拨动开关处理
        APP_switch_rotary_process();

        //触摸开关
       Touch_value touch_value= get_touch_value();
        if(touch_value==TOUCH)
        {
            touch_count++;
            if(touch_count>5)
            {
                touch_count=0;
            }
        }
        
        vTaskDelay(50);
    }

}

//task_alarm任务函数实现
void task_alarm(void* pvParmeters)
{
    NV020D_Init();
    uint8_t count_alarm=0;
    while(1)
    {
        if(show_type.is_alarm_five_day==1&&datetime.week>=6)
        {
            
        }else{
             if(show_mode==show_mode_normal)
       {
         if(show_type.is_alarm_1==1)
        {
            //判断闹钟1
            if(alarm_time1.hour==datetime.hour&&alarm_time1.min==datetime.minute)
            {
                //触发闹钟
                if(alarm_time1.is_on==0)
                {

                    alarm_time1.is_on=1;
                    NV020D_send_cmd(0x00);
                    alarm_touch_count=touch_count;
                }
                else if(alarm_time1.is_on==1&&alarm_time1.is_done==0)
                {
                    count_alarm++;
                    if(count_alarm>=5)
                    {
                        NV020D_send_cmd(0x00);
                        count_alarm=0;
                    }

                    if(alarm_touch_count!=touch_count)
                    {
                        NV020D_send_cmd(0xfe);
                        alarm_time1.is_done=1;
                    }
                }
                else{

                }

            }else {
                if(alarm_time1.is_on==1)
                {
                    alarm_time1.is_on=0;
                    alarm_time1.is_done=0;
                    NV020D_send_cmd(0XFE);
                }
            }

        }
        else if(show_type.is_alarm_2==1)
        {
            //判断闹钟2
            if(alarm_time2.hour==datetime.hour&&alarm_time2.min==datetime.minute)
            {
                //触发闹钟
                if(alarm_time2.is_on==0)
                {

                    alarm_time2.is_on=1;
                    NV020D_send_cmd(0x00);
                    alarm_touch_count=touch_count;
                }
                else if(alarm_time2.is_on==1&&alarm_time2.is_done==0)
                {
                    count_alarm++;
                    if(count_alarm>=5)
                    {
                        NV020D_send_cmd(0x00);
                        count_alarm=0;
                    }

                    if(alarm_touch_count!=touch_count)
                    {
                        NV020D_send_cmd(0xfe);
                        alarm_time2.is_done=1;
                    }
                }
                else{

                }

            }else {
                if(alarm_time2.is_on==1)
                {
                    alarm_time2.is_on=0;
                    alarm_time2.is_done=0;
                    NV020D_send_cmd(0XFE);
                }
            }

        }
      
       }
         vTaskDelay(1000);
        }
      
    }
}

//task_alarm任务函数实现
void task_volume(void* pvParmeters)
{
    //初始nv020d
    NV020D_Init();
    NV020D_send_cmd(0xE0+nv020d_volume);
    uint8_t count_volume=99;
    //初始设置音量
    while(1)
    {
        if(show_mode==show_mode_volume_set)
        {
            //播放音乐
            count_volume++;
            if(count_volume>=100)
            {
                NV020D_send_cmd(0x00);
                count_volume=0;
            }
            //接收按键值
            Key_type_value key_value=init_get_key_value();
            switch(key_value)
            {
                case KEY_UP:
                //增加音量
                nv020d_volume++;
                if(nv020d_volume>7)
                {
                    nv020d_volume=7;
                }
                NV020D_send_cmd(0xE0+nv020d_volume);
                break;
                
                case KEY_DOWN:
                //减小音量
                nv020d_volume--;
                if(nv020d_volume<0)
                {
                    nv020d_volume=0;
                }
                NV020D_send_cmd(0xE0+nv020d_volume);
                break;
                
                case KEY_ALARM_SET:
                //停止音乐播放
                show_mode=show_mode_normal;
                NV020D_send_cmd(0xFE);
                count_volume=99;
                break;
                default:
                break;
            }
        }
        vTaskDelay(50);
    }

}

void task_debug(void* pvParmeters)
{
    Key_type_value test=0;
    while(1)
    {
        test= init_get_key_value();
        if(test==KEY_NONE)
        debug_printf("1");
        if(test==KEY_TIME_SET)
        debug_printf("2");
        if(test==KEY_TIME_SET_LONG)
        debug_printf("3");
        if(test==KEY_ALARM_SET)
        debug_printf("4");
        if(test==KEY_ALARM_SET_LONG)
        debug_printf("5");
        if(test==KEY_UP)
        debug_printf("6");
        if(test==KEY_DOWN)
        debug_printf("7");
        if(test==KEY_ALARM_EN)
        debug_printf("8");
        if(test==KEY_ALARM_5)
        debug_printf("9");
        vTaskDelay(1000);
    }
}

