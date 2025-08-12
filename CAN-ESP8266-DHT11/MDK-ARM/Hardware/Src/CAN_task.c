#include "can_task.h"
#include "can_config.h"
#include "main.h"
#include <stdio.h>

extern osMessageQueueId_t OLEDQueueHandle;
extern osMessageQueueId_t WiFiQueueHandle;

void CANStartTask(void *argument)
{   

    // 初始化CAN硬件
    CAN_Init();
//    printf("CAN Receive Task Started\r\n");
    
    for(;;) {
        // 主任务只需保持运行，接收通过中断处理
        osDelay(1000);
    }
}

// CAN接收中断处理函数
void CAN_ReceiveHandler(CAN_HandleTypeDef *hcan) {
    CAN_RxHeaderTypeDef header;
    uint8_t data[8];
    DHT11_Data_t receivedData;
    osStatus_t sta;
		osStatus_t res;
	
    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &header, data) == HAL_OK) {
        if(header.StdId == 0x123 && header.DLC == 4) { // 检查ID和数据长度
            // 解析温湿度数据
            uint16_t temp = (data[0] << 8) | data[1];
            uint16_t humi = (data[2] << 8) | data[3];
            
            receivedData.temperature = temp / 10.0f;
            receivedData.humidity = humi / 10.0f;
            receivedData.valid = 1;
            
            printf("Received: Temp=%.1fC, Humi=%.1f%%\r\n", 
                  receivedData.temperature, receivedData.humidity);
            
            // 将数据放入接收队列
            
				sta = osMessageQueuePut(OLEDQueueHandle, &receivedData, 0, 0);
        if (sta == osOK) {
//            printf("oled put ok\r\n");
        } 
				res = osMessageQueuePut(WiFiQueueHandle, &receivedData, 0, 0);
        if (res == osOK) {
//            printf("can put ok\r\n");
        } 
        }
    }
}
