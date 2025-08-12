#include "esp8266.h"
#include <string.h>   // 字符串操作函数
#include <stdio.h>    // 标准输入输出
#include <stdbool.h>  // 布尔类型支持
#include <stdlib.h>   // 标准库函数(如atoi)
#include "cmsis_os.h" // CMSIS-RTOS接口
#include "usart.h"    // 串口通信

// WiFi连接信息
#define ESP8266_WIFI_INFO "AT+CWJAP=\"zhe\",\"88888888\"\r\n"

// OneNet平台MQTT连接参数
const char* ClintID = "shangweiji"; // 客户端ID
const char* username = "teNI4Rcv8E"; // 用户名
const char* passwd = "version=2018-10-31&res=products%2FteNI4Rcv8E%2Fdevices%2Fshangweiji&et=2542669535&method=md5&sign=G4JGUr8bNXkVSnJcLcm3Cw%3D%3D"; // 密码
const char* Url = "mqtts.heclouds.com"; // MQTT服务器地址

// MQTT主题定义
const char* pubtopic = "$sys/teNI4Rcv8E/shangweiji/thing/property/post"; // 属性上报主题
const char* subtopic = "$sys/teNI4Rcv8E/shangweiji/thing/property/post/reply"; // 响应主题
const char* set_topic = "$sys/teNI4Rcv8E/shangweiji/thing/property/set"; // 控制指令主题


// ESP8266通信缓冲区
unsigned char ESP8266_buf[ESP8266_BUF_SIZE];
unsigned short ESP8266_cnt = 0, ESP8266_cntPre = 0;

uint8_t aRxBuffer; // 串口接收中断缓冲区

// 清空ESP8266接收缓冲区
void ESP8266_Clear(void)
{
    memset(ESP8266_buf, 0, sizeof(ESP8266_buf)); // 清零缓冲区
    ESP8266_cnt = 0; // 重置接收计数器
}

// 检查数据接收状态
_Bool ESP8266_WaitRecive(void)
{
    if(ESP8266_cnt == 0) return REV_WAIT; // 无数据
    
    // 检查是否接收完成（连续两次计数相同）
    if(ESP8266_cnt == ESP8266_cntPre) {
        ESP8266_cnt = 0; // 重置计数器
        return REV_OK;    // 接收完成
    }
    
    ESP8266_cntPre = ESP8266_cnt; // 更新上次计数
    return REV_WAIT;              // 等待更多数据
}

// 发送AT指令并检查响应
_Bool ESP8266_SendCmd(char *cmd, char *res)
{
    unsigned char timeOut = 200; // 超时计数器
    
    // 通过串口2发送AT指令
    Usart_SendString(huart2, (unsigned char *)cmd, strlen((const char *)cmd));
    
    // 等待响应
    while(timeOut--) {
        if(ESP8266_WaitRecive() == REV_OK) {
            // 检查是否包含预期响应
            if(strstr((const char *)ESP8266_buf, res) != NULL) {
                ESP8266_Clear(); // 清空缓冲区
                return 0;        // 成功
            }
        }
        HAL_Delay(10); // 延迟10ms
    }
    
    return 1; // 超时或响应不符
}

// ESP8266初始化流程
void ESP8266_Init(void)
{
    char cmdBuf[512];  // 命令缓冲区
	
    ESP8266_Clear();  // 清空接收缓冲区
	
    // 1. 测试AT指令
    printf("0. AT\r\n");
    while(ESP8266_SendCmd("AT\r\n", "OK"))
        HAL_Delay(500);
	
    // 2. 重启模块
    printf("1. RST\r\n");
    ESP8266_SendCmd("AT+RST\r\n", "");
    HAL_Delay(500);	
    
    // 3. 设置WiFi模式为Station
    printf("2. CWMODE\r\n");
    while(ESP8266_SendCmd("AT+CWMODE=1\r\n", "OK"))  // 模式1(Station)
        HAL_Delay(500);
    
    // 4. 开启DHCP
		printf("3. AT+CWDHCP\r\n");
    while(ESP8266_SendCmd("AT+CWDHCP=1,1\r\n", "OK"))  // 开启DHCP
        HAL_Delay(500);
    
    // 5. 连接WiFi
    printf("4. CWJAP\r\n");
    while(ESP8266_SendCmd(ESP8266_WIFI_INFO, "OK"))
        HAL_Delay(500);
    
    // 6. 配置MQTT用户信息
    printf("5.MQTTUSERCFG\r\n");
    sprintf(cmdBuf, "AT+MQTTUSERCFG=0,1,\"%s\",\"%s\",\"%s\",0,0,\"\"\r\n", 
            ClintID, username, passwd);
    while(ESP8266_SendCmd(cmdBuf, "OK"))
        HAL_Delay(500);
    memset(cmdBuf, 0, sizeof(cmdBuf));
    
    // 7. 连接MQTT服务器
    printf("6.MQTTCONN\r\n");
    sprintf(cmdBuf, "AT+MQTTCONN=0,\"%s\",1883,1\r\n", Url);
    while(ESP8266_SendCmd(cmdBuf, "OK"))
        HAL_Delay(500);
    memset(cmdBuf, 0, sizeof(cmdBuf));
    
    // 8. 订阅上报回复主题
    printf("7.MQTTSUB\r\n");
    sprintf(cmdBuf, "AT+MQTTSUB=0,\"%s\",1\r\n", subtopic);
    while(ESP8266_SendCmd(cmdBuf, "OK"))
        HAL_Delay(500);
    memset(cmdBuf, 0, sizeof(cmdBuf));
    
    // 9. 订阅控制指令主题
    printf("8. Subscribe to SET topic\r\n");
    sprintf(cmdBuf, "AT+MQTTSUB=0,\"%s\",1\r\n", set_topic);
    while(ESP8266_SendCmd(cmdBuf, "OK"))
        HAL_Delay(500);
    memset(cmdBuf, 0, sizeof(cmdBuf));
}

// 单属性上报函数
void Esp_PUB(const char* identifier, int value) {
    char cmdBuf[256];
    
    // 构造属性上报消息
    sprintf(cmdBuf, 
        "AT+MQTTPUB=0,\"%s\","
        "\"{\\\"id\\\":\\\"123\\\"\\,\\\"params\\\":{\\\"%s\\\":{\\\"value\\\":%d\\}}}\",0,0\r\n",
        pubtopic, identifier, value);
    
    // 发送并等待确认
    while(ESP8266_SendCmd(cmdBuf, "OK"))
        HAL_Delay(500);
    
    memset(cmdBuf, 0, sizeof(cmdBuf));
    HAL_Delay(100); // 短延时防止连续发送冲突
}


// 串口接收中断回调函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance != USART2) return; // 仅处理USART2
    
    // 检查缓冲区空间
    if (ESP8266_cnt < ESP8266_BUF_SIZE - 1) {
        ESP8266_buf[ESP8266_cnt++] = aRxBuffer; // 存入数据
    } else {
        // 缓冲区溢出处理
        ESP8266_cnt = 0;
        memset(ESP8266_buf, 0, ESP8266_BUF_SIZE);
    }
    
    // 重启接收中断
    HAL_UART_Receive_IT(&huart2, (uint8_t *)&aRxBuffer, 1);
}
