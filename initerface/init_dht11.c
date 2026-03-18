#include "init_dht11.h"

void DHT11_Init(void)
{
    DHT11_DATA_H;  // 将数据线拉高
    vTaskDelay(1000);  // DHT11上电后需要延时1秒钟才能正常工作
}

void DHT11_Get_Temp_Humi(int8_t *temperature, int8_t * humidity)
{
    int8_t tempe=0;
    int8_t humi=0;
    // 发送开始信号
    DHT11_DATA_L;  // 将数据线拉低
    vTaskDelay(20);
    DHT11_DATA_H;  // 将数据线拉高

    //以下读取温湿度的代码不能被打断，需要完整执行
    //添加临界区
    taskENTER_CRITICAL();
    
    uint32_t count_max=0xffffff;
    //接收向应信号
    while(DHT11_DATA_Read == GPIO_PIN_SET &&count_max--)
    {
        // 等待DHT11拉低数据线，表示准备发送数据
    }
    while(DHT11_DATA_Read== GPIO_PIN_RESET &&count_max--)
    {
        // 等待DHT11拉高数据线，表示开始发送数据
    }
    while(DHT11_DATA_Read== GPIO_PIN_SET &&count_max--)
    {
        // 等待DHT11拉低数据线，表示开始发送数据
    }
    if(count_max==0)
    {
        debug_printf("DHT11没有响应");
    }

    uint8_t data[5] = {0};  // 存储接收到的40位数据
  for(uint8_t i=0;i<5;i++)
  {
    data[i]=0;
    for(uint8_t j=0;j<8;j++)
    {
        while(DHT11_DATA_Read== GPIO_PIN_RESET)
        {
            // 等待DHT11拉高数据线，表示开始发送数据
        }
        init_delay(40);  // 等待40微秒，判断是0还是1
        if(DHT11_DATA_Read== GPIO_PIN_SET)
        {
            data[i]|=(1<<(7-j));  // 接收到了1，设置对应位
            while(DHT11_DATA_Read== GPIO_PIN_SET)
            {
                // 等待DHT11拉低数据线，准备发送下一位数据
            }
        }
    }
  }

  uint32_t checksum = data[0] + data[1] + data[2] + data[3];  // 计算校验和
    if ((uint8_t)checksum != data[4]) {
        debug_printf("DHT11数据校验失败");
        return;
    }

    humi = data[0];  // 湿度整数部分
    tempe = data[2];  // 温度整数部分
    if(data[3] & 0x80)  // 判断温度是否为负数
    {
        tempe = -tempe;
    }
    *temperature = tempe;
    *humidity = humi;
    
  taskEXIT_CRITICAL();//退出临界区
}

