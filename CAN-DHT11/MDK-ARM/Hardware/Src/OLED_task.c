#include "OLED_task.h"

extern osMessageQueueId_t OLEDQueueHandle;

void OLEDStartTask(void *argument)
{
    OLED_Init();
    DHT11_Msg msg;
    for(;;)
    {
        // 接收数据（超时2001ms）
        if(osMessageQueueGet(OLEDQueueHandle, &msg, NULL, 2001) == osOK)
        {
					printf("oled get ok  %d\r\n",msg.valid);
            if(msg.valid)
            {
							uint16_t temp_int = (uint16_t)msg.temperature;                   // 整数部分
							uint16_t temp_dec = (uint16_t)((msg.temperature - temp_int) * 10); // 小数部分
							uint16_t humi_int = (uint16_t)msg.humidity;
							uint16_t humi_dec = (uint16_t)((msg.humidity - humi_int) * 10);

							OLED_ShowString(4, 1, "TP:");
							OLED_ShowNum(4, 4, temp_int, 2);  // 显示整数部分，2位宽
							OLED_ShowString(4, 6, ".");       // 小数点
							OLED_ShowNum(4, 7, temp_dec, 1);  // 显示小数部分，1位宽
							OLED_ShowString(4, 8, "C");

							OLED_ShowString(4, 9, "HI:");
							OLED_ShowNum(4, 12, humi_int, 2);
							OLED_ShowString(4, 14, ".");
							OLED_ShowNum(4, 15, humi_dec, 1);
							OLED_ShowString(4, 16, "%");
						}
						else
            {
                OLED_ShowString(4, 1, "No Data");
            }
        }
        else
        {
					printf("oled get error  %d\r\n",msg.valid);
            // 超时处理（可选）
//            OLED_ShowString(4, 1, "No Data");
        }

  }
}

