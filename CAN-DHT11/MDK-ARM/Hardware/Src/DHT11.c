#include "DHT11.h"
#include "main.h"
#include "tim.h"  // CubeMX生成的定时器句柄头文件
void Delay_us(uint32_t us)
{
    __HAL_TIM_SET_COUNTER(&htim3, 0);  // 将计数器清零
    HAL_TIM_Base_Start(&htim3);        // 启动定时器
    while(__HAL_TIM_GET_COUNTER(&htim3) < us);  // 等待计数达到us
    HAL_TIM_Base_Stop(&htim3);         // 关闭定时器
}

/**
  * @brief  毫秒级延时
  * @param  xms 延时时长，范围：0~4294967295
  * @retval 无
  */
void Delay_ms(uint32_t xms)
{
    while(xms--)
    {
        Delay_us(1000);
    }
}

/**
  * @brief  秒级延时
  * @param  xs 延时时长，范围：0~4294967295
  * @retval 无
  */
void Delay_s(uint32_t xs)
{
    while(xs--)
    {
        Delay_ms(1000);
    }
}
void DHT11_OUT(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct = {0};
 
	GPIO_InitStruct.Pin = DHT11_GPIO_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}
/**
  * @brief  DATA引脚（PA7）设置为输入模式
  * @param  无
  * @retval	无 
  */
void DHT11_IN(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct = {0};
 
	GPIO_InitStruct.Pin  = DHT11_GPIO_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}
/**
  * @brief  DHT11检测起始信号
  * @param  无
  * @retval 无
  */
void DHT11_Strat(void)
{
	DHT11_OUT();   //PA7设置为输出模式
	DHT11_Low;     //主机拉低总线
	HAL_Delay(20); //延迟必须大于18ms ； 
	DHT11_High;    //主机拉高总线等待DHT11响应
	Delay_us(30);   

}
/**
  * @brief  DHT11发送响应信号
  * @param  无
  * @retval 返回值0/1  0：响应成功 1：响应失败
  */
uint8_t DHT11_Check(void)
{
	uint8_t retry = 0 ;
	DHT11_IN();
	//采用while循环的方式检测响应信号
	while(DHT11_IO_IN && retry <100) // DHT11会拉低 40us ~80us
	{
		retry++;
		Delay_us(1);//1us
	}
	if(retry>=100) //判断当DHT11延迟超过80us时return 1 ， 说明响应失败
	{return  1;}
	else retry =  0 ;
		
	while(!DHT11_IO_IN && retry<100)// // DHT11拉低之后会拉高 40us ~80us
	{
		retry++;
		Delay_us(1);//1us
	}
		
	if(retry>=100)
	{return 1;}
	return 0 ;
}
/*DHT11初始化*/
uint8_t DHT11_Init(void)
{	 
 	GPIO_InitTypeDef  GPIO_InitStructure;
    __HAL_RCC_GPIOA_CLK_ENABLE();  // 假设DHT11接在GPIOA，根据实际情况修改

    GPIO_InitStructure.Pin = DHT11_GPIO_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DHT11_GPIO_PORT, &GPIO_InitStructure);

    HAL_GPIO_WritePin(DHT11_GPIO_PORT, DHT11_GPIO_PIN, GPIO_PIN_SET);

    DHT11_Strat();  //复位DHT11
    return DHT11_Check();//等待DHT11的回应
} 

/**
  * @brief  DHT11读取一位数据
  * @param  无
  * @retval 返回值0/1  1：读取成功 0：读取失败
  */
uint8_t DHT11_Read_Bit(void)
{
	uint8_t retry = 0 ;
	while(DHT11_IO_IN && retry <100)//同上采用while循环的方式去采集数据
	{
		retry++;
		Delay_us(1);
	}
	retry = 0 ;
	while(!DHT11_IO_IN && retry<100)
	{
		retry++;
		Delay_us(1);
	}
 
	Delay_us(40);              //结束信号，延时40us 
	if(DHT11_IO_IN) return 1;  //结束信号后，总线会被拉高 则返回1表示读取成功
	else 
	return 0 ;
}
/**
  * @brief  DHT11读取一个字节数据
  * @param  无
  * @retval 返回值：dat 将采集到的一个字节的数据返回
  */
uint8_t DHT11_Read_Byte(void)
{
	uint8_t i , dat ;
	dat = 0 ;
	for(i=0; i<8; i++)
	{
		dat <<= 1; //通过左移存储数据
		dat |= DHT11_Read_Bit();
	}
	return dat ; 
}
/**
  * @brief  DHT11读取数据
  * @param  temp：温度值 humi ：湿度值
  * @retval 返回值0/1 0：读取数据成功 1：读取数据失败
  */
uint8_t DHT11_Read_Data(uint8_t* temp , uint8_t* humi)
{
	uint8_t buf[5];        //储存五位数据
    uint8_t i;    
	DHT11_Strat();         //起始信号
	if(DHT11_Check() == 0) //响应信号
    {
		for(i=0; i<5; i++)
		{
			buf[i] = DHT11_Read_Byte();
		}
		if(buf[0]+buf[1]+buf[2]+buf[3] == buf[4]) //校验数据
		{
		    *humi = buf[0]; // 湿度
			*temp = buf[2]; // 温度
		}
	}else return 1;
	
   return 0 ;
}
