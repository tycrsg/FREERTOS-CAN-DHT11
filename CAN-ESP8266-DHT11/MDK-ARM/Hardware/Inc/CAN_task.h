#ifndef __CAN_TASK_H
#define __CAN_TASK_H

#include "cmsis_os2.h"

// 温湿度数据结构
typedef struct {
    float temperature;
    float humidity;
    uint8_t valid;
} DHT11_Data_t;

// CAN接收队列
extern osMessageQueueId_t canRxQueueHandle;

void CANStartTask(void *argument);

#endif
