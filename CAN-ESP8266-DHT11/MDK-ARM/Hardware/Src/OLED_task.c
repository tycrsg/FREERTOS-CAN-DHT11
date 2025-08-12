#include "oled_task.h"
#include "can_task.h"
#include "OLED.h"
#include "main.h"
#include <stdio.h>

extern osMessageQueueId_t OLEDQueueHandle;

void OLEDStartTask(void *argument)
{
    OLED_Init();
    DHT11_Data_t displayData;
    
//    printf("OLED Task Started\r\n");
    
    OLED_ShowString(3,1,"Wait...");
    for(;;) {
        // 从CAN接收队列获取数据（等待最多100ms）
        if(osMessageQueueGet(OLEDQueueHandle, &displayData, NULL, 100) == osOK) {
//					printf("oled get ok \r\n");
            if(displayData.valid) {
                // 显示温度
                uint16_t temp_int = (uint16_t)displayData.temperature;
                uint16_t temp_dec = (uint16_t)((displayData.temperature - temp_int) * 10);
                
                OLED_ShowString(3, 1, "Temp:");
                OLED_ShowNum(3, 6, temp_int, 2);
                OLED_ShowString(3, 8, ".");
                OLED_ShowNum(3, 9, temp_dec, 1);
                OLED_ShowString(3, 10, "C");
                
                // 显示湿度
                uint16_t humi_int = (uint16_t)displayData.humidity;
                uint16_t humi_dec = (uint16_t)((displayData.humidity - humi_int) * 10);
                
                OLED_ShowString(4, 1, "Humi:  ");
                OLED_ShowNum(4, 6, humi_int, 2);
                OLED_ShowString(4, 8, ".");
                OLED_ShowNum(4, 9, humi_dec, 1);
                OLED_ShowString(4, 10, "%");
                
                printf("OLED Updated: %.1fC, %.1f%%\r\n", 
                      displayData.temperature, displayData.humidity);
            } else {
                OLED_ShowString(3, 1, "Invalid Data Received!");
                osDelay(2000);
                OLED_ShowString(3, 1, "                   "); // 清空行
            }
        }
        osDelay(10);
    }
}
