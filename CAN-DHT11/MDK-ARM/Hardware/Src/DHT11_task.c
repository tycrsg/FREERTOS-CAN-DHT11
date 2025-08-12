#include "DHT11_task.h"

/* 外部声明 - 引用其他文件中定义的消息队列句柄 */
// OLED显示消息队列句柄，用于传递温湿度数据到OLED显示任务
extern osMessageQueueId_t OLEDQueueHandle;
// CAN通信消息队列句柄，用于传递温湿度数据到CAN发送任务
extern osMessageQueueId_t CANQueueHandle; 

/**
  * @brief  DHT11任务主函数
  *         负责初始化DHT11传感器，并周期性读取温湿度数据，
  *         将数据通过消息队列发送给OLED显示任务和CAN通信任务
  * @param  argument: FreeRTOS任务参数（未使用）
  * @retval None
  */
void DHT11StartTask(void *argument)
{
    // 初始化DHT11传感器
    DHT11_Init();
    
    // 创建DHT11数据结构体实例，用于存储温湿度数据
    DHT11_Data msg = {0};
    
    // 定义消息队列发送状态变量
    osStatus_t sta;    // OLED队列发送状态
    osStatus_t res;    // CAN队列发送状态
    
    for(;;)
    {
        // 临时变量，用于存储读取到的温度和湿度值
        uint8_t temp = 0, humi = 0;
        
        // 设置读取超时时间为当前时间+500ms，防止读取操作无限阻塞
        uint32_t timeout = HAL_GetTick() + 500; 
        
        // 读取DHT11数据，若读取失败则重试，直到成功或超时
        while(DHT11_Read_Data(&temp, &humi) != 0)
        {
            // 检查是否超时，若超时则退出循环
            if(HAL_GetTick() > timeout) break;
            
            // 每10ms重试一次
            osDelay(10);
        }
        
        // 检查是否成功读取到有效数据（温度或湿度不为0）
        if(temp || humi)
        {
            msg.temperature = (float)temp;
            msg.humidity = (float)humi;
            // 标记数据有效
            msg.valid = 1;
        }
        else
        {
            // 标记数据无效
            msg.valid = 0;
        }
        
        // 将温湿度数据放入OLED消息队列，供OLED任务显示
        // 优先级为0，超时时间为0（立即返回）
        sta = osMessageQueuePut(OLEDQueueHandle, &msg, 0, 0);
        if (sta == osOK) {
            // 调试信息：OLED队列发送成功（默认注释掉）
//            printf("oled put ok\r\n");
        } 
        
        // 将温湿度数据放入CAN消息队列，供CAN任务发送
        // 优先级为0，超时时间为0（立即返回）
        res = osMessageQueuePut(CANQueueHandle, &msg, 0, 0);
        if (res == osOK) {
            // 调试信息：CAN队列发送成功（默认注释掉）
//            printf("can put ok\r\n");
        } 
        
        // 任务延时2000ms，即每2秒读取一次温湿度数据
        osDelay(2000);
    }
}
