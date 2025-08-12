#include "can_config.h"

extern CAN_HandleTypeDef hcan;

// CAN总线初始化函数
void CAN_Init(void) {
   // 定义CAN过滤器结构体
   CAN_FilterTypeDef filter;
   
   // 配置过滤器参数
   filter.FilterBank = 0;              // 选择过滤器组0（单CAN模式下有效范围0-13）
   filter.FilterMode = CAN_FILTERMODE_IDMASK;  // 标识符屏蔽模式（需匹配ID & Mask）
   filter.FilterScale = CAN_FILTERSCALE_32BIT; // 32位过滤器宽度
   filter.FilterIdHigh = 0x123 << 5;   // 标准ID高位（0x123左移5位对应STDID[10:0]）
   filter.FilterIdLow = 0x0000;        // 标准ID低位（标准帧ID仅11位）
   filter.FilterMaskIdHigh = 0x7ff << 5; // 掩码高位（精确匹配ID 0x123）
   filter.FilterMaskIdLow = 0x0000;    // 掩码低位（标准帧掩码为0）
   filter.FilterFIFOAssignment = CAN_RX_FIFO0; // 接收数据存入FIFO0
   filter.FilterActivation = ENABLE;   // 启用该过滤器
   filter.SlaveStartFilterBank = 14;   // 从设备过滤器起始组（双CAN时使用）

   // 配置CAN过滤器
   if (HAL_CAN_ConfigFilter(&hcan, &filter) != HAL_OK) {
       Error_Handler();  // 过滤器配置失败处理
   }
   
   // 启动CAN总线
   if (HAL_CAN_Start(&hcan) != HAL_OK) {
       Error_Handler();  // CAN启动失败处理
   }
   
   // 激活接收中断和错误中断
   HAL_CAN_ActivateNotification(&hcan, 
       CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_ERROR);  // 接收中断+总线错误中断
}

// 接收FIFO0消息挂起回调函数
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
   CAN_ReceiveHandler(hcan);  // 自定义接收数据处理函数
}
