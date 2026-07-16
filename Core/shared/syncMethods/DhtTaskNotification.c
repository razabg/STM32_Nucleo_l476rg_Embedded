/*
 * TOPIC: FreeRTOS Direct Task Notification
 *
 * A lightweight signaling mechanism built into every task — no semaphore
 * or queue object needs to be created. Each task has its own 32-bit
 * notification value that other tasks (or ISRs) can write to directly.
 *
 * Flow:
 *   StartDelay  ──every 2s──► xTaskNotifyGive(dhtTaskHandle)
 *   StartDht    ──ulTaskNotifyTake()──► read DHT sensor
 *               ──► pack temp+humidity into uint32_t
 *               ──► xTaskNotify(printTaskHandle, value, eSetValueWithOverwrite)
 *   StartPrint  ──xTaskNotifyWait()──► unpack the 4 bytes ──► printf
 *
 * KEY APIS:
 *   xTaskNotifyGive(handle)                              — simple signal (like a semaphore give)
 *   ulTaskNotifyTake(pdTRUE, timeout)                    — block until notified, then clear
 *   xTaskNotify(handle, value, eSetValueWithOverwrite)   — send a 32-bit value
 *   xTaskNotifyWait(clearOnEntry, clearOnExit, &val, timeout)
 *
 * NOTE: dhtTaskHandle and printTaskHandle must be declared in main.c so
 * both sender and receiver can reference them by name.
 */

//#include "cmsis_os.h"
//#include "FreeRTOS.h"
//#include "task.h"
//#include "main.h"
//#include <stdio.h>
//#include "dht2.h"
//
//extern TIM_HandleTypeDef htim2;
//extern osThreadId_t dhtTaskHandle;
//extern osThreadId_t printTaskHandle;
//
//DHT_Data dht_data = {0};
//DHT_Handle *dht   = NULL;
//
//void StartDelay(void *argument)
//{
//    for(;;)
//    {
//        osDelay(2000);
//        xTaskNotifyGive(dhtTaskHandle);
//    }
//}
//
//void StartDht(void *argument)
//{
//    dht = DHT_Create(DHT_GPIO_Port, DHT_Pin, &htim2);
//    if (!dht) {
//        printf("dht == null");
//    }
//    DHT_Result result;
//
//    for(;;)
//    {
//        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
//        DHT_Result result = DHT_Read(dht, &dht_data);
//        if (result == DHT_NO_RESPONSE){
//                printf("DHT: No response\r\n");}
//            else if (result == DHT_CHECKSUM_ERROR){
//                printf("DHT: Checksum error\r\n");}
//            else
//                {printf("DHT: Read OK\r\n");}
//        uint32_t value = ((uint32_t)dht_data.temperature_int << 24) |
//                         ((uint32_t)dht_data.temperature_dec << 16) |
//                         ((uint32_t)dht_data.humidity_int    <<  8) |
//                         ((uint32_t)dht_data.humidity_dec);
//        xTaskNotify(printTaskHandle, value, eSetValueWithOverwrite);
//    }
//}
//
//void StartPrint(void *argument)
//{
//    for(;;)
//    {
//        uint32_t value;
//        xTaskNotifyWait(0, 0xFFFFFFFF, &value, portMAX_DELAY);
//
//        uint8_t temp_int  = (value >> 24) & 0xFF;
//        uint8_t temp_dec  = (value >> 16) & 0xFF;
//        uint8_t humid_int = (value >>  8) & 0xFF;
//        uint8_t humid_dec = (value)       & 0xFF;
//
//        printf("T: %d.%d  H: %d.%d\r\n", temp_int, temp_dec, humid_int, humid_dec);
//    }
//}
