#include  "init_ds1302z.h"

void init_delay(uint32_t us)
{
    uint32_t count = 0;
    count = us * 8;
    while(count--)
    {
        __NOP();
        
    }
}

/**
 * @brief 读取DS1302Z的寄存器值
 * @param rex_addr 寄存器地址
 * @return uint8_t
 */
uint8_t ds1302z_read(uint8_t reg_addr)
{
    reg_addr|=0x01;//最低位为1表示读取
    //初始化引脚:拉低RST和CLK
    DS_RST_L;
    DS_CLK_L;

    //拉高RST引脚，准备通信
    DS_RST_H;
    init_delay(5);

    //发送寄存器地址，最低位为1表示读取
    for(int i = 0; i < 8; i++)
    {
      if(reg_addr&(1<<i))
      {
        DS_IO_H;
      }
      else
      {
        DS_IO_L;
      }
    DS_CLK_H;
    init_delay(1);
    DS_CLK_L;
    }

    //读取数据
    uint8_t data = 0;
    DS_IO_H; //释放IO引脚，准备读取数据
    for(int i = 0; i < 8; i++)
    {
        data |= (DS_IO_READ<<i);
        if(i<7){
        DS_CLK_H;  
        DS_CLK_L;
        }
    }
   DS_RST_L; //通信结束，拉低RST引脚
   return data;
}


/**
 * @brief 向DS1302Z的寄存器写入数据
 * @param rex_addr 寄存器地址
 * @param data 要写入的数据
 */
void ds1302z_write(uint8_t reg_addr, uint8_t data)
{
    reg_addr&=(0xfe); //确保最低位为0，表示写入
    
    //初始化引脚:拉低RST和CLK
    DS_RST_L;
    DS_CLK_L;
    //拉高RST引脚，准备通信
    DS_RST_H;
    init_delay(5);
    //发送寄存器地址，最低位为0表示写入
    for(int i = 0; i < 8; i++)
    {
      if(reg_addr&(1<<i))
      {
        DS_IO_H;
      }
      else
      {
        DS_IO_L;
      }
      DS_CLK_H;
      init_delay(1);
      DS_CLK_L;
    }
    //发送数据
    for(int i = 0; i < 8; i++)
    {
      if(data&(1<<i))
      {
        DS_IO_H;
      }
      else
      {
        DS_IO_L;
      }
      DS_CLK_H;
      init_delay(1);
      DS_CLK_L;
    }


}

