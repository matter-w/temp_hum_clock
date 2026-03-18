#include "APP_dateime.h"


/**
 * @brief 获取当前时间:年月日时分星期
 */
void app_datetime_get(Clock_Date_time_type *datetime)
{
    //获取年
    uint8_t year_data = ds1302z_read(DS1302Z_YEAR_REX);
    datetime->year = (year_data >> 4) * 10 + (year_data & 0x0F) + 2000;
    //获取月
    uint8_t month_data = ds1302z_read(DS1302Z_MONTH_REX);
    datetime->month = (month_data >> 4) * 10 + (month_data & 0x0F);
    //获取日 
    uint8_t date_data = ds1302z_read(DS1302Z_DATE_REX);
    datetime->date = (date_data >> 4) * 10 + (date_data & 0x0F);
    //获取星期
    uint8_t week_data = ds1302z_read(DS1302Z_WEEK_REX);
    datetime->week = (week_data & 0x0f);
    //获取时
    uint8_t hour_data = ds1302z_read(DS1302Z_HOUR_REX);

    if (hour_data & 0x80)        // 判断使用的是否为12小时制
    {
        datetime->is_12h_format = 1;
        //判断是否为下午
        if (hour_data & 0x20)
        {
            datetime->is_pm = 1;
        }
        else
        {
            datetime->is_pm = 0;
        }
        if (hour_data & 0x10)//判断十位是否有数据
        {
            datetime->hour = 10 + (hour_data & 0x0f);
        }
        else
        {
            datetime->hour = (hour_data & 0x0f);
        }
    }
    else
    {
        // 24小时
        datetime->is_12h_format = 0;
        if (hour_data & 0x20)//第二个十位
        {
            // 20小时位
            datetime->hour = 20 + (hour_data & 0x0f);
        }
        else if (hour_data & 0x10)//第一个十位
        {
            // 10小时位
            datetime->hour = 10 + (hour_data & 0x0f);
        }
        else
        {
            datetime->hour = (hour_data & 0x0f);
        }
    }
    //统一使用24小时制保存日期
    if(datetime->is_12h_format==1)
    {
        if(datetime->is_pm)
        {
            datetime->hour=datetime->hour+12;
        }
        datetime->is_12h_format=0;
    }

    //获取分
    uint8_t minute_data = ds1302z_read(DS1302Z_MINUTE_REX);
    datetime->minute = (minute_data >> 4) * 10 + (minute_data & 0x0F);
}

/**
 * @brief 设置所有的时间
 */
void app_datetime_set(Clock_Date_time_type *datetime)
{
    //关闭ds1302z芯片写保护
    ds1302z_write(DS1302Z_CONTRAL_REX,0x00);
    //1.将设置的年份写入进芯片
    ds1302z_write(DS1302Z_YEAR_REX,((datetime->year-2000)/10)<<4|(datetime->year-2000)%10);
    //2.将设置的月份写入芯片
    ds1302z_write(DS1302Z_MONTH_REX,((((datetime->month)/10)&0X01)<<4)|(datetime->month)%10);
    //3.将设置的日期写入芯片
    ds1302z_write(DS1302Z_DATE_REX,((((datetime->date)/10)&0X03)<<4)|(datetime->date)%10);
    //4.将设置的小时写入芯片
    if(datetime->hour>12)
    {
        uint8_t hour_shi=(datetime->hour-12)/10;
        uint8_t hour_ge=(datetime->hour-12)%10;
        ds1302z_write(DS1302Z_HOUR_REX,(0X80|(0X20)|(hour_shi<<4)|(hour_ge)));
    }else{
        uint8_t hour_shi=(datetime->hour)/10;
        uint8_t hour_ge=(datetime->hour)%10;
        ds1302z_write(DS1302Z_HOUR_REX,(0X80|(hour_shi<<4)|(hour_ge%10)));
    }
    //5.将设置的分钟写入芯片
    ds1302z_write(DS1302Z_MINUTE_REX,(((datetime->minute/10)&0x07)<<4)|(datetime->minute%10));
    //6.给秒写入一个随机值
    ds1302z_write(DS1302Z_SECOND_REX,0X00);
    //7.计算星期->使用蔡勒公式计算，0->星期天，1-6 ->星期一至六
    uint16_t y=datetime->year;
    uint8_t m=datetime->month;
    uint8_t c= y / 100;
    if (m < 3) { m += 12; y--; }
    int16_t w= y + y / 4 + c / 4 - 2 * c + 26 * (m+1) / 10 + datetime->date-1;
    while(w<0)
    {
        w+=7;
    }
    w=w%7;
    if(w==0)
    {
        w=7;
    }
    ds1302z_write(DS1302Z_WEEK_REX,w);
}

