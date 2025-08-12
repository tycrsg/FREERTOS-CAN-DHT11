// can_task.c
#include "can_task.h"
#include "can_config.h"
#include "DHT11_task.h"


extern osMessageQueueId_t CANQueueHandle;
uint8_t loopback_test = 0; // 环回测试标志，1表示启用

void CANStartTask(void *argument)
{
    // 初始化CAN
    CAN_Init();
    
    CAN_Message_t canMsg;
    DHT11_Data dhtData;
    
//    printf("CAN Task Started - Loopback Mode: %s\r\n", loopback_test ? "ON" : "OFF");
    
    for(;;)
    {
        // 从CAN队列获取数据（等待100ms）
        if(osMessageQueueGet(CANQueueHandle, &dhtData, NULL, 100) == osOK)
        {
					printf("can get ok\r\n");
            if(dhtData.valid)
            {
                // 准备CAN消息
                canMsg.id = 0x123; // CAN ID
                canMsg.len = 4;
                
                // 打包数据：温度(16bit) + 湿度(16bit)
                uint16_t temp = (uint16_t)(dhtData.temperature * 10); // 保留1位小数
                uint16_t humi = (uint16_t)(dhtData.humidity * 10);
                
                canMsg.data[0] = temp >> 8;    // 温度高字节
                canMsg.data[1] = temp & 0xFF;  // 温度低字节
                canMsg.data[2] = humi >> 8;    // 湿度高字节
                canMsg.data[3] = humi & 0xFF;  // 湿度低字节
                
                // 打印发送数据
                printf("[TX] Temp: %.1fC, Humi: %.1f%% -> ", 
                       dhtData.temperature, dhtData.humidity);
                printf("CAN Data: %02X %02X %02X %02X\r\n", 
                       canMsg.data[0], canMsg.data[1], canMsg.data[2], canMsg.data[3]);
                
                // 发送CAN消息
                if(CAN_SendData(canMsg.id, canMsg.data, canMsg.len) != HAL_OK)
                {
                    printf("CAN Send Failed\r\n");
                }
            }
        }
        else
        {
            osDelay(10);
        }
    }
}
