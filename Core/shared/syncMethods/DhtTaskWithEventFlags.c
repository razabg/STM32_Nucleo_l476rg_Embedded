/*
 * TOPIC: FreeRTOS Event Flags — Sequential Signal Chain
 *
 * Event flags replace semaphores when you want named, bit-level signals that
 * any task can set or wait on. Here two flags form a 3-stage pipeline:
 *
 *   StartDelay  ──every 1s──► osEventFlagsSet(dht_flags, DHT_TASK)
 *   StartDht    ──wait DHT_TASK──► read DHT ──► set PRINT_TASK
 *   StartPrint  ──wait PRINT_TASK──► printf
 *
 * dht_flags is initialized in main() BEFORE osKernelInitialize():
 *   dht_flags = osEventFlagsNew(NULL);
 *
 * KEY APIS:
 *   osEventFlagsNew(attr)
 *   osEventFlagsSet(handle, flags)                           — set one or more bits
 *   osEventFlagsWait(handle, flags, osFlagsWaitAny, timeout) — block until any listed bit is set
 *
 * NOTE: osEventFlagsWait() clears the waited flag automatically after it fires,
 * so no manual clear is needed.
 */

//#include "cmsis_os.h"
//#include "main.h"
//#include <stdio.h>
//#include "dht2.h"
//
//extern TIM_HandleTypeDef htim2;
//
//DHT_Data dht_data = {0};
//DHT_Handle *dht = NULL;
//
//osEventFlagsId_t dht_flags; //******************************** this the events flag ***********************************
//
//#define DHT_TASK  0x01   // bit 0
//#define PRINT_TASK  0x02   // bit 1
//
//void StartDelay(void *argument)
//{
//  for(;;)
//  {
//
//    osDelay(1000);
//    osEventFlagsSet(dht_flags, DHT_TASK);
//
//  }
//}
//
//void StartDht(void *argument)
//{
//	dht = DHT_Create(DHT_GPIO_Port,DHT_Pin,&htim2);
//	if (!dht) {
//		printf("dht == null");
//	}
//	DHT_Result result;
//
//  for(;;)
//  {
//	osEventFlagsWait(dht_flags, DHT_TASK,osFlagsWaitAny, osWaitForever);
//	 DHT_Result result = DHT_Read(dht, &dht_data);
//	if (result == DHT_NO_RESPONSE){
//	        printf("DHT: No response\r\n");}
//	    else if (result == DHT_CHECKSUM_ERROR){
//	        printf("DHT: Checksum error\r\n");}
//	    else
//	        {printf("DHT: Read OK\r\n");}
//
//   osEventFlagsSet(dht_flags, PRINT_TASK);
//
//  }
//}
//
//void StartPrint(void *argument)
//{
//  for(;;)
//  {
//	  	  	  osEventFlagsWait(dht_flags, PRINT_TASK,osFlagsWaitAny, osWaitForever);
//	          printf("T:%d H:%d\r\n",
//	                 dht_data.temperature_int,
//	                 dht_data.humidity_int);
//  }
//}
