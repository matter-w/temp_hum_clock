#ifndef APP_FREERTOS_H
#define APP_FREERTOS_H

#include "FreeRTOS.h"
#include "task.h"
#include "common.h"
#include "init_key.h"
#include "init_led.h"
#include "APP_dateime.h"
#include "init_nv020d.h"
#include "init_dht11.h"
#include "init_touch.h"
#include "init_touch.h"
#include "init_mic.h"
#include "APP_show.h"
#include "APP_switch.h"



//启动FreeRTOS
void APP_FREERTOS_Start(void);


#endif  //APP_FREERTOS_H
