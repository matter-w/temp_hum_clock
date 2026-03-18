#include "init_key.h"

/**
 * @brief 获取按键值
 * @return 1--按键被按下，0--按键未被按下
 */
uint8_t get_key_value(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{ 
    if(HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_RESET)
    {
        vTaskDelay(10); // 消抖延时
        if(HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_RESET)
        {
            //消抖完成，等待用户松开按键
            while(HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_RESET)
            {
                vTaskDelay(1);
            } 
            //用户松开按键
            return 1; // 按键被按下
        }
    }
    return 0; // 按键未被按下
}



/**
 * @brief 获取按键值
 * @return Key_type_value 按键值
 */
Key_type_value init_get_key_value(void)
{
 
    //获取四个只有短按的按键值：上调、下调、闹钟使能、五天闹钟使能
   if(get_key_value(UP_GPIO_Port, UP_Pin))
    {
        return KEY_UP;
    }
    else if(get_key_value(DOWN_GPIO_Port, DOWN_Pin))
    {
        return KEY_DOWN;
    }
    else if(get_key_value(ALARM_EN_GPIO_Port, ALARM_EN_Pin))
    {
        return KEY_ALARM_EN;
    }
    else if(get_key_value(ALARM_5_GPIO_Port, ALARM_5_Pin))
    {
        return KEY_ALARM_5;
    }

    //添加长按逻辑,时间设置按键和闹钟设置按键需要区分短按和长按

    //检测时间设置按键
    if(HAL_GPIO_ReadPin(TIME_SET_GPIO_Port,TIME_SET_Pin)==GPIO_PIN_RESET)
    {
        vTaskDelay(10); // 消抖延时
        if(HAL_GPIO_ReadPin(TIME_SET_GPIO_Port,TIME_SET_Pin)==GPIO_PIN_RESET)
        {
            //消抖完成，等待用户松开按键
         TickType_t start_tick = xTaskGetTickCount();
            while(HAL_GPIO_ReadPin(TIME_SET_GPIO_Port,TIME_SET_Pin)==GPIO_PIN_RESET)
            {
                vTaskDelay(1);
            } 
            //用户松开按键
            TickType_t end_tick = xTaskGetTickCount();
           if(end_tick - start_tick >= 3000) // 长按超过3秒
            {
                return KEY_TIME_SET_LONG; // 时间设置长按
            }
            else
            {
                return KEY_TIME_SET; // 时间设置短按
            }
        }
    }

    //检测闹钟设置按键
    if(HAL_GPIO_ReadPin(ALARM_SET_GPIO_Port,ALARM_SET_Pin)==GPIO_PIN_RESET)
    {
        vTaskDelay(10); // 消抖延时
        if(HAL_GPIO_ReadPin(ALARM_SET_GPIO_Port,ALARM_SET_Pin)==GPIO_PIN_RESET)
        {
            //消抖完成，等待用户松开按键
         TickType_t start_tick = xTaskGetTickCount();
            while(HAL_GPIO_ReadPin(ALARM_SET_GPIO_Port,ALARM_SET_Pin)==GPIO_PIN_RESET)
            {
                vTaskDelay(1);
            } 
            //用户松开按键
            TickType_t end_tick = xTaskGetTickCount();
           if(end_tick - start_tick >= 3000) // 长按超过3秒
            {
                return KEY_ALARM_SET_LONG; // 闹钟设置长按
            }
            else
            {
                return KEY_ALARM_SET; // 闹钟设置短按
            }
        }
    }

    return KEY_NONE;
}

/**
 * @brief 获取LED状态
 * @return LED_ON_Type_value LED状态
 */
LED_ON_Type_value get_led_value(void)
{
    if(HAL_GPIO_ReadPin(LED_ON_GPIO_Port,LED_ON_Pin)==GPIO_PIN_RESET)
    {
        return LED_OFF; // LED 亮
    }
    else
    {
        return LED_ON; // LED 灭
    }
}

/**
 * @brief 获取背光灯状态
 * @return Light_type_value 背光灯状态
 */
Light_type_value get_light_value(void)
{
    if(HAL_GPIO_ReadPin(LIGHT_GPIO_Port,LIGHT_Pin)==GPIO_PIN_RESET)
    {
        return LIGHT_OFF; // 背光灯亮
    }
    else
    {
        return LIGHT_ON; // 背光灯灭
    }
}

