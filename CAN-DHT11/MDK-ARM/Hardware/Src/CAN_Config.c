#include "can_config.h"
#include "main.h"
#include "usart.h"
// can.c
#include "can.h"
#include "oled.h"
extern CAN_HandleTypeDef hcan;
extern uint8_t loopback_test; // 来自can_task.h

// CAN初始化
void CAN_Init(void) {
    // 启动CAN
    if (HAL_CAN_Start(&hcan) != HAL_OK) {
        printf("CAN Start Failed\r\n");
    }
    else {
        printf("CAN Started Successfully\r\n");
    }
    
    // 配置过滤器 - 允许所有消息通过
    CAN_FilterTypeDef filter;
    filter.FilterBank = 0;
    filter.FilterMode = CAN_FILTERMODE_IDMASK;
    filter.FilterScale = CAN_FILTERSCALE_32BIT;
    filter.FilterIdHigh = 0x0000;
    filter.FilterIdLow = 0x0000;
    filter.FilterMaskIdHigh = 0x0000;
    filter.FilterMaskIdLow = 0x0000;
    filter.FilterFIFOAssignment = CAN_RX_FIFO0;
    filter.FilterActivation = ENABLE;
    filter.SlaveStartFilterBank = 14;
    
    if (HAL_CAN_ConfigFilter(&hcan, &filter) != HAL_OK) {
        printf("CAN Filter Config Failed\r\n");
    }
    else {
        printf("CAN Filter Configured\r\n");
    }
    
    // 启用接收中断
    HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
}

// CAN发送数据
HAL_StatusTypeDef CAN_SendData(uint32_t id, uint8_t* data, uint8_t len) {
    CAN_TxHeaderTypeDef header;
    uint32_t mailbox;
    
    header.StdId = id;
    header.ExtId = 0;
    header.RTR = CAN_RTR_DATA;
    header.IDE = CAN_ID_STD;
    header.DLC = len;
    header.TransmitGlobalTime = DISABLE;
    
    return HAL_CAN_AddTxMessage(&hcan, &header, data, &mailbox);
}


// CAN接收中断处理
void CAN_ReceiveHandler(CAN_HandleTypeDef *hcan) {
    CAN_RxHeaderTypeDef header;
    uint8_t data[8];
    
    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &header, data) == HAL_OK) {
        printf("[RX] ID: 0x%03X, DLC: %d, Data: ", header.StdId, header.DLC);
        for (int i = 0; i < header.DLC; i++) {
            printf("%02X ", data[i]);
        }
        printf("\r\n");
        
        // 如果是DHT11数据，解析并打印
        if(header.StdId == 0x123 && header.DLC == 4) {
            uint16_t temp = (data[0] << 8) | data[1];
            uint16_t humi = (data[2] << 8) | data[3];
            float temperature = temp / 10.0f;
            float humidity = humi / 10.0f;
						OLED_ShowNum(1,1,temp/10,2);
						OLED_ShowNum(2,1,humi/10,2);
            printf("  Parsed Data: Temp=%.1fC, Humi=%.1f%%\r\n", temperature, humidity);
        }
    }
}
