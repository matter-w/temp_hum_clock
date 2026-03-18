#ifndef INIT_TOUCH_H
#define INIT_TOUCH_H

#include "gpio.h"

typedef enum{
    NO_TOUCH,
    TOUCH,
}Touch_value;

/**
 * @brief 获取当前触摸开关的值
 * @return touch_value 当前触摸开关的值
 */
Touch_value get_touch_value(void);

#endif /* INIT_TOUCH_H */

