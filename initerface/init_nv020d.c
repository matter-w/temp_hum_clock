#include "init_nv020d.h"


/**
 * @brief  初始化延时
 */
void NV020D_Init(void)
{
    vTaskDelay(500);  // 延时500毫秒，等待NV020D模块上电稳定

}

void NV020D_send_byte(uint8_t byte)
{
    NV020D_CLK_H;// 先拉高时钟线
  if(byte&(1))
        {
            NV020D_DATA_H;
        }
        else
        {
            NV020D_DATA_L;
        }
    vTaskDelay(4);
    for(uint8_t i=0;i<8;i++)
    {
        if(byte&(1<<i))
        {
            NV020D_DATA_H;
        }
        else
        {
            NV020D_DATA_L;
        }
        NV020D_CLK_L;
        init_delay(400);
        NV020D_CLK_H;
        init_delay(400);
    }
        NV020D_DATA_H;
}

/**
 * @brief  发送命令给NV020D模块
 * @param  cmd: 要发送的命令
 */
/*
void NV020D_send_cmd(uint8_t cmd)
{
    while(NV020D_DATA_Read==GPIO_PIN_SET) // 等待数据线变为低电平，表示模块准备好接收数据
    {}
    vTaskDelay(100);
    NV020D_send_byte(0xf1);
    NV020D_send_byte(cmd);
    NV020D_send_byte(0xf3);
    uint8_t sum=(uint8_t)(0xf1+cmd+0xf3);
    NV020D_send_byte(sum);
}
*/
//使用单线协议发送命令
void NV020D_send_cmd(uint8_t cmd)
{
    NV020D_CTR_H;
    NV020D_CTR_L;
    vTaskDelay(4);
    for(uint8_t i=0;i<8;i++)
    {
        if(cmd&(1<<i))
        {
            NV020D_DATA_H;
            vTaskDelay(3);
            NV020D_CTR_L;
            vTaskDelay(1);
        }
        else
        {
            
            NV020D_DATA_H;
            vTaskDelay(1);
            NV020D_CTR_L;
            vTaskDelay(3);
        } 
    }
    NV020D_CTR_H;  
    vTaskDelay(100);      

}

void nv020d_set_volume(uint8_t volume)
{
    if(volume>7) volume=7; // 限制音量范围在0-7
    NV020D_send_cmd(0xe0+volume); // 发送设置音量的命令，命令格式为0xe0 + 音量值
}


void nv020d_stop(void)
{
    NV020D_send_cmd(0xfe);
}
