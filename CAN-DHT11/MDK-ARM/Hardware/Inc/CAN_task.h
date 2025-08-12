// can_task.h
#ifndef __CAN_TASK_H
#define __CAN_TASK_H

#include "cmsis_os2.h"

// CAN消息结构
typedef struct {
    uint32_t id;
    uint8_t data[8];
    uint8_t len;
} CAN_Message_t;

// CAN队列句柄
extern osMessageQueueId_t CANQueueHandle;

// 环回测试标志
extern uint8_t loopback_test;

// CAN任务函数
void CANStartTask(void *argument);

#endif
