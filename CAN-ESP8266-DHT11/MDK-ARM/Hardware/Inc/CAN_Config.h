#ifndef __CAN_CONFIG_H
#define __CAN_CONFIG_H

#include "stm32f1xx_hal.h"
#include "can.h"
#include "can_task.h"
#include "usart.h"
#include "main.h"
// CAN配置
#define CAN_ID_DHT11_DATA 0x123  // DHT11数据的CAN ID

// CAN初始化函数
void CAN_Init(void);

// CAN发送函数
HAL_StatusTypeDef CAN_SendData(uint32_t id, uint8_t* data, uint8_t len);

// CAN接收处理函数
void CAN_ReceiveHandler(CAN_HandleTypeDef *hcan);

#endif
