#ifndef __DHT11_H
#define __DHT11_H

#include "stm32f1xx_hal.h"

// 定义DHT11引脚

#define DHT11_GPIO_PORT  GPIOA
#define DHT11_GPIO_PIN   GPIO_PIN_5
/*模式定义*/
#define Int 0
#define Out 1

/*高低电平设置*/
#define DHT11_High     HAL_GPIO_WritePin(DHT11_GPIO_PORT, DHT11_GPIO_PIN,	GPIO_PIN_SET) //输出高电平
#define DHT11_Low      HAL_GPIO_WritePin(DHT11_GPIO_PORT, DHT11_GPIO_PIN, GPIO_PIN_RESET)//输出低电平
#define DHT11_IO_IN      HAL_GPIO_ReadPin(DHT11_GPIO_PORT, DHT11_GPIO_PIN)//读取IO口电平
 

/*函数定义*/
void Delay_us(uint32_t xus);
void Delay_ms(uint32_t xms);
void Delay_s(uint32_t xs);
	
uint8_t DHT11_Init(void);
                          //DHT11初始化
void DHT11_OUT(void);												
void DHT11_IN(void);                          //DHT11_Data IO设置为输入模式
void DHT11_Strat(void);				          //主机发出起始信号
uint8_t DHT11_Check(void);                    //DHT11发送响应信号
uint8_t DHT11_Read_Bit(void);                 //读取DHT11一个BIT的数据
uint8_t DHT11_Read_Byte(void);                //读取DHT11一个Byte的数据
uint8_t DHT11_Read_Data(uint8_t* temp , uint8_t* humi);  //读取DHT11湿度和温度的数据

#endif /* __DHT11_H */
