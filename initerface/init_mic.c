#include "init_mic.h"


mic_value get_mic_value(void)
{
    //读取值为0时说明有人说话，读取值为1时说明没有人说话
    if(HAL_GPIO_ReadPin(MIC_VIN_GPIO_Port, MIC_VIN_Pin) == GPIO_PIN_RESET)
    {
        return MIC_ON;
    }
    else
    {
        return MIC_OFF;
    }
}


