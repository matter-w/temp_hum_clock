#ifndef _COMMON_H
#define _COMMON_H

#include "usart.h"
#include "stdio.h"
#include "stdarg.h"

#define DEBUG_ENABLE 1
#ifdef DEBUG_ENABLE
//统一使用通用的宏做日志输出
#define debug_printf(format,...) printf("[%s:%d]" format "\r\n",__FILE__,__LINE__,##__VA_ARGS__)

#else  
#define debug_printf(...)   

#endif //DEBUG_ENABLE



#endif  //_COMMON_H
