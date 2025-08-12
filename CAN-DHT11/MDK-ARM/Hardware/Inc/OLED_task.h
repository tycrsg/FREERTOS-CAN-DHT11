#ifndef __OLED_TASK_H__
#define __OLED_TASK_H__

#ifdef __cplusplus
extern "C" {
#endif


#include "main.h"
#include "OLED.h"
#include "main.h"
#include "gpio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "DHT11_task.h"

// 定义相同的数据结构
typedef struct {
    float temperature;
    float humidity;
    uint8_t valid;
} DHT11_Msg;

void OLEDStartTask(void *argument);

#ifdef __cplusplus
}
#endif

#endif /* __OLED_TASK_H__ */
