#include "wifi_task.h"    // WiFi任务头文件

extern osMessageQueueId_t WiFiQueueHandle;

/**
  * @brief WiFi任务主函数
  * @param argument: FreeRTOS任务参数
  * @retval None
  */
void WiFiStartTask(void *argument) {
    // 1. 初始化ESP8266模块（连接WiFi和MQTT）
    ESP8266_Init();
    printf("[WiFi] Started\n"); // 初始化完成提示
    DHT11_Data_t displayData;
    uint32_t lastReportTime = HAL_GetTick(); // 记录上次上报时间
    
    // 2. 主任务循环
    for(;;) {
        uint32_t now = HAL_GetTick(); // 获取当前时间
        if(osMessageQueueGet(WiFiQueueHandle, &displayData, NULL, 100) == osOK) {
//           printf("wifi get ok \r\n");
					if(displayData.valid) {
							uint16_t temp_int = (uint16_t)displayData.temperature;
							uint16_t humi_int = (uint16_t)displayData.humidity;
             
        //  定期上报数据（每5秒）
        if (now - lastReportTime > 5000) {
						Esp_PUB("temp",temp_int);
						Esp_PUB("humi",humi_int);
            
            //  输出上报日志（调试信息）
            printf("Reported WiFi Data: temp=%d, humi=%d\r\n", temp_int, humi_int);
            
            //  更新最后上报时间
            lastReportTime = now;
        }
			}
		}
    }
}
