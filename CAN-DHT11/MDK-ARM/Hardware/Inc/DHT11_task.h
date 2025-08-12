#ifndef _DHT11_TASK_H_
#define _DHT11_TASK_H_

#include "main.h"
#include "cmsis_os.h"
#include "usart.h"
#include "DHT11.h"

typedef struct {
    float temperature;
    float humidity;
	uint8_t valid;
} DHT11_Data;

void DHT11StartTask(void *argument);
uint8_t GetDHT11Data(float *temp, float *humi);
#endif
