/*
 * TOPIC: FreeRTOS Binary Semaphores — 3-Stage Producer-Consumer Pipeline
 *
 * Three tasks form a sequential pipeline. Two binary semaphores act as
 * handoff signals between stages:
 *
 *   StartDelay  ──every 5s──► osSemaphoreRelease(readDhtSemaphoreHandle)
 *   StartDht    ──acquire readDht──► read DHT ──► release printData
 *   StartPrint  ──acquire printData──► printf
 *
 * Unlike task notifications (which are private to one task), semaphores are
 * named shared objects — any task can give or take them.
 *
 * In main(), after osKernelInitialize():
 *   readDhtSemaphoreHandle  = osSemaphoreNew(1, 0, &readDhtSemaphore_attributes);  // starts CLOSED
 *   printDataSemaphoreHandle = osSemaphoreNew(1, 0, &printDataSemaphore_attributes); // starts CLOSED
 *
 * KEY APIS:
 *   osSemaphoreNew(maxCount, initialCount, attr)   — 0 initial count = starts closed
 *   osSemaphoreAcquire(handle, timeout)            — block until count > 0, then decrement
 *   osSemaphoreRelease(handle)                     — increment count, unblock waiter
 */

//#include "cmsis_os.h"
//#include "main.h"
//#include <stdio.h>
//#include "dht2.h"
//
//extern TIM_HandleTypeDef htim2;
//
///* Definitions for readDhtSemaphore */
//osSemaphoreId_t readDhtSemaphoreHandle;
//const osSemaphoreAttr_t readDhtSemaphore_attributes = {
//  .name = "readDhtSemaphore"
//};
///* Definitions for printDataSemaphore */
//osSemaphoreId_t printDataSemaphoreHandle;
//const osSemaphoreAttr_t printDataSemaphore_attributes = {
//  .name = "printDataSemaphore"
//};
//
//DHT_Data dht_data = {0};
//DHT_Handle *dht = NULL;
//
//void StartDelay(void *argument)
//{
//  for(;;)
//  {
//
//    osDelay(5000);
//    osSemaphoreRelease(readDhtSemaphoreHandle);
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
//	osSemaphoreAcquire(readDhtSemaphoreHandle,osWaitForever);
//	 DHT_Result result = DHT_Read(dht, &dht_data);
//	if (result == DHT_NO_RESPONSE){
//	        printf("DHT: No response\r\n");}
//	    else if (result == DHT_CHECKSUM_ERROR){
//	        printf("DHT: Checksum error\r\n");}
//	    else
//	        {printf("DHT: Read OK\r\n");}
//
//   osSemaphoreRelease(printDataSemaphoreHandle);
////   osDelay(10);
//  }
//}
//
//void StartPrint(void *argument)
//{
//  for(;;)
//  {
//	          osSemaphoreAcquire(printDataSemaphoreHandle, osWaitForever); // ✅ blocking
//	          printf("T:%d H:%d\r\n",
//	                 dht_data.temperature_int,
//	                 dht_data.humidity_int);
//  }
//}
