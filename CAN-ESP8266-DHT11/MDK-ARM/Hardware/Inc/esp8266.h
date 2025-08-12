#ifndef _ESP8266_H_
#define _ESP8266_H_

// 必要的系统头文件
#include <string.h> // 字符串操作函数
#include <stdio.h>  // 标准输入输出
#include <stdint.h> // 精确宽度整数类型

#include <stdbool.h> // 布尔类型支持

// 接收状态宏定义
#define REV_OK   0   // 数据接收完成标志
#define REV_WAIT 1   // 数据接收未完成标志

// 接收缓冲区大小定义 (扩大至512字节)
#define ESP8266_BUF_SIZE 512 

// 外部声明的接收缓冲区
extern unsigned char ESP8266_buf[ESP8266_BUF_SIZE];

// 功能函数声明
void ESP8266_Init(void);                   // 模块初始化
void ESP8266_Clear(void);                  // 清空接收缓冲区
_Bool ESP8266_WaitRecive(void);            // 等待接收完成检查
void ESP8266_ProcessMessage(const char *message); // MQTT消息处理函数
void Esp_PUB(const char* identifier, int value);  // 单属性上报函数
void SendCommandResponse(const char *req_id);      // 发送命令响应
bool ESP8266_ConnectMQTT(void);                  // MQTT连接函数
bool ESP8266_CheckAndMaintainMQTT(void);         // MQTT连接维护检查

#endif // 头文件结束
