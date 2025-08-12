#ifndef __WIFI_TASK_H__
#define __WIFI_TASK_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "main.h"
#include "esp8266.h"
#include "cmsis_os.h"
#include "usart.h"         // 串口通信
#include "can_task.h"
void StartWiFiTask(void *argument);



#ifdef __cplusplus
}
#endif

#endif /* __WIFI_TASK_H__ */
