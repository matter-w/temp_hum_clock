#ifndef INIT_MIC_H
#define INIT_MIC_H

#include "gpio.h"


typedef enum{
    MIC_ON,
    MIC_OFF,
}mic_value;

/**
 * @brief 获取当前麦克风开关的值
 * @return mic_value 当前麦克风开关的值
 */
mic_value get_mic_value(void);


#endif /* INIT_MIC_H */

