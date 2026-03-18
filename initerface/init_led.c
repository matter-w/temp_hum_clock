#include "init_led.h"

// 设计数组 保存时钟对应的数字
uint16_t led_clk_h_s[3] = {0x0000, 0x0006, 0x005b};
uint16_t led_clk_h_g[10] = {0x1f80, 0x0300, 0x2d80, 0x2780, 0x3300, 0x3680, 0x3e80, 0x0380, 0x3f80, 0x3780};

uint16_t led_clk_m_s[10] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
uint16_t led_clk_m_g[10] = {0x1f80, 0x0300, 0x2d80, 0x2780, 0x3300, 0x3680, 0x3e80, 0x0380, 0x3f80, 0x3780};

//温湿度数组
uint16_t    led_temp_s[10] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
uint16_t    led_temp_g[10] = {0x1f80, 0x0300, 0x2d80, 0x2780, 0x3300, 0x3680, 0x3e80,
     0x0380, 0x3f80, 0x3780};
uint16_t    led_hu_s[10] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
uint16_t    led_hu_g[10] = {0x1f80, 0x0300, 0x2d80, 0x2780, 0x3300, 0x3680, 0x3e80,
     0x0380, 0x3f80, 0x3780};

//触摸开关的值，用于控制led不亮的时间，实现亮度控制
 extern uint8_t touch_count;    
/**
 * @brief 控制总开关
 *
 */
void Inf_led_start(void)
{
    HAL_GPIO_WritePin(P04_GPIO_Port, P04_Pin, GPIO_PIN_RESET);
}

/**
 * @brief 关闭总开关
 *
 */
void Inf_led_stop(void)
{
        LED_OEA_H;
        LED_OEB_H;
}

/**
 * @brief 写入数据
 *
 * @param p00 VCCP00 电源控制
 * @param p01 VCCP01 电源控制
 * @param p02 VCCP02 电源控制
 * @param data 两个驱动芯片的数据
 */
void Inf_led_write(uint8_t p00, uint8_t p01, uint8_t p02, uint32_t data)
{
 
    // 传输数据高位在前  先发的是后一个芯片 right
    for (uint8_t i = 0; i < 32; i++)
    {
        if (data & (0x80000000 >> i))
        {
            LED_SDI_H;
        }
        else
        {
            LED_SDI_L;
        }

        LED_CLK_H;
        LED_CLK_L;
    }

    // 锁存数据
    LED_LE_H; 
    LED_LE_L;
    HAL_GPIO_WritePin(VCC_P00_GPIO_Port, VCC_P00_Pin, (GPIO_PinState)p00);
    HAL_GPIO_WritePin(VCC_P01_GPIO_Port, VCC_P01_Pin, (GPIO_PinState)p01);
    HAL_GPIO_WritePin(VCC_P02_GPIO_Port, VCC_P02_Pin, (GPIO_PinState)p02);
    

    // OE 使能
    LED_OEA_L;
    LED_OEB_L;
}

/**
 * @brief 设置时钟
 *
 * @param h 小时
 * @param m 分钟
 */
void Inf_led_set_clock(uint8_t h, uint8_t m,uint8_t clock_flag)
{
    // 先让h亮5ms
    uint8_t h_shi = h / 10;
    uint8_t h_ge = h % 10;
    uint16_t data_h=0;
    //让两个点进行闪烁
    if(clock_flag)
    {
        data_h = led_clk_h_s[h_shi] + led_clk_h_g[h_ge]+0x20;
    }
    else
    {
        data_h = led_clk_h_s[h_shi]+ led_clk_h_g[h_ge];
    }
    Inf_led_write(0, 1, 1, data_h);//显示小时
    vTaskDelay(5);

    uint8_t m_shi = m / 10;
    uint8_t m_ge = m % 10;

    Inf_led_write(1, 0, 1, led_clk_m_s[m_shi] + led_clk_m_g[m_ge]);
    vTaskDelay(5);
}

void Inf_led_set_all(uint8_t h,int8_t m,int8_t temperatu,int8_t humidity,
    uint8_t clock_flag,show_type_struct* show_type)
{
    // 一次性写入32位数据 => 先写高16位 => 先写right的数据
    // 拼接VCCP00导电时候的  32data值
    // 湿度值=> 添加对应的单位符号0x4000
    uint32_t vccp02=0x38;
    //拼接vccp00的数据
    //湿度值
    uint16_t data_right=led_hu_s[humidity/10]+led_hu_g[humidity%10]+0x4000;
    //添加am/pm的灯
    uint16_t am_pm_data=0;
    if(show_type->is_12_hour)
    {
        if(show_type->is_pm)
        {
            am_pm_data=0x8000;
        }
        else{
            am_pm_data=0x4000;
        }
    }
    uint16_t data_left=0;
    if(clock_flag)
    {
        data_left = led_clk_h_s[h/10] + led_clk_h_g[h%10]+am_pm_data+0x20;
    }
    else
    {
        data_left = led_clk_h_s[h/10]+led_clk_h_g[h%10];
    }
    //五天闹钟标识
    if(show_type->is_alarm_five_day)
    {
        data_right+=0x8000;
    }

    //一次写入32位数据
    Inf_led_write(0,1,1,(data_right<<16)|data_left);
    vTaskDelay(5-touch_count);


      //修正温度的值
   int8_t temp=temperatu;
  
    uint16_t temp_data=0;
    if(show_type->is_celsius)
    {
        temp_data=0x4000;
    }else{
        temp_data=0x8000;
        temp=(int8_t)((temperatu*9/5)+32);
    }

    //拼接vccp01的数据
    if(temp<0)
    {
        temp=-temp;
        vccp02+=0x02;

    }
    if(temp>99)
    {
        temp-=100;
        vccp02+=0x05;
    }
    data_right=led_temp_s[temp/10]+led_temp_g[temp%10]+temp_data;
    data_left=led_clk_m_s[m/10]+led_clk_m_g[m%10];

    // 添加闹钟标识
    uint16_t alarm_data=0;
    if(show_type->is_alarm_1)
    {
        alarm_data+=0x4000;
    }
    if(show_type->is_alarm_2)
    {
        alarm_data+=0x8000;
    }

    data_left+=alarm_data;
    HAL_GPIO_WritePin(VCC_P00_GPIO_Port,VCC_P00_Pin,GPIO_PIN_SET);
    vTaskDelay(touch_count);
    //一次写入32位数据
    Inf_led_write(1, 0, 1, (data_right<<16)|data_left);
    vTaskDelay(5-touch_count);
    HAL_GPIO_WritePin(VCC_P01_GPIO_Port,VCC_P01_Pin,GPIO_PIN_SET);
    vTaskDelay(touch_count);
    //vccp02
    Inf_led_write(1,1,0,(vccp02<<16));
    vTaskDelay(5-touch_count);
    HAL_GPIO_WritePin(VCC_P02_GPIO_Port,VCC_P02_Pin,GPIO_PIN_SET);
    vTaskDelay(touch_count);

    //特殊灯-》充电标识
    if(show_type->is_charging)
    {
        HAL_GPIO_WritePin(P03_GPIO_Port,P03_Pin,GPIO_PIN_RESET);
    }
}

/**
 * @brief 用于时间设置时展示时钟的数字
 */
void Inf_led_number(uint16_t num1,uint16_t num2,uint16_t point)
{
    uint16_t vccp00_data=0;
    uint16_t vccp01_data=0;
    uint8_t point_data=0;
    //标记点是否亮
    if(point)
    {
        point_data=0x20;
    }
    //根据第一个数值计算vccp00的值
    if(num1>99)
    {
        vccp00_data=point_data+0;
    }else{
        vccp00_data=led_clk_h_s[num1/10]+led_clk_h_g[num1%10]+point_data;
    }
    if(num2>99)
    {
        vccp01_data=0;
    }else{
        vccp01_data=led_clk_m_s[num2/10]+led_clk_m_g[num2%10];
    }

    //根据数值让led亮起来
    Inf_led_write(0,1,1,vccp00_data);
    vTaskDelay(5);
    Inf_led_write(1,0,1,vccp01_data);
}

uint32_t time_set_last_time=0;
uint8_t led_flag=0;

void Inf_led_time_set(show_time_set time_set,uint16_t value)
{
     if(xTaskGetTickCount() - time_set_last_time >= 500) //每隔500ms切换一次时钟闪烁状态
    {
        led_flag = !led_flag; //切换时钟闪烁状态
        time_set_last_time = xTaskGetTickCount(); //更新上次切换时间
    }
    switch (time_set)
    {
        case time_set_year:
        if(led_flag)
        {
            Inf_led_number(value/100,value%100,0);
        }else{
            Inf_led_number(value/100,0xff,0);
        }
        break;

        case time_set_month:
        if(led_flag)
        {
            Inf_led_number(value,0xff,0);
        }else{
            Inf_led_number(0xff,0xff,0);
        }
        break;
        
        case time_set_day:
          if(led_flag)
        {
            Inf_led_number(0xff,value,0);
        }else{
            Inf_led_number(0xff,0xff,0);
        }
        break;
        
        case time_set_hour:
        if(led_flag)
        {
            Inf_led_number(value,0xff,1);
        }else{
            Inf_led_number(0xff,0xff,1);
        }
        break;
        
        case time_set_minute:
        if(led_flag)
        {
            Inf_led_number(0xff,value,1);
        }else{
            Inf_led_number(0xff,0xff,1);
        }
        break;
        
        default:
        break;
    }
}
