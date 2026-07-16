/*
 * TOPIC: FreeRTOS Mutex — Protecting a Shared Resource (UART)
 *
 * Three tasks all print to UART. Without a mutex, their printf calls
 * interleave and produce garbled output. A single mutex serializes access:
 * only the task that holds the mutex may print; the others block until it
 * releases it.
 *
 * Each task picks a random delay (10–99 ms) before each print to simulate
 * realistic concurrent behavior.
 *
 * In main(), after osKernelInitialize():
 *   UartMutexHandle = osMutexNew(&UartMutex_attributes);
 *
 * KEY APIS:
 *   osMutexNew(attr)                     — create mutex
 *   osMutexAcquire(handle, timeout)      — lock; blocks if already held by another task
 *   osMutexRelease(handle)               — unlock; next waiting task gets it
 *
 * The two HAL timer callbacks below implement an interrupt-driven "breathing"
 * LED on TIM3/TIM6 — they run in ISR context and are independent of the tasks.
 * TIM3 PeriodElapsed turns the pin ON; TIM3 PWM PulseFinished turns it OFF.
 * TIM6 PeriodElapsed steps the duty cycle to create the fade effect.
 */

//#include "cmsis_os.h"
//#include "main.h"
//#include <stdio.h>
//#include <stdlib.h>
//
//extern TIM_HandleTypeDef htim3;
//
///* Definitions for UartMutex */
//osMutexId_t UartMutexHandle;
//const osMutexAttr_t UartMutex_attributes = {
//  .name = "UartMutex"
//};
//
//int duty = 9;
//int direction = 1;
//int step = 50;
//
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//    if (htim->Instance == TIM3)
//    {
//        HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_SET);  // pin ON
//    }
//    else if (htim->Instance == TIM6)
//    {
//        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, duty);
//        duty += direction * step;
//        if (duty >= 989) { duty = 989; direction = -1; }
//        if (duty <= 9)   { duty = 9;   direction =  1; }
//    }
//}
//
//void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
//{
//    if (htim->Instance == TIM3)
//    {
//        HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_RESET);  // pin OFF
//    }
//}
//
//void StartStar(void *argument)
//{
//  for(;;)
//  {
//	  int delay =  10 + rand() % (99 - 10 + 1);
//	  	  osDelay(delay);
//	  	  osMutexAcquire(UartMutexHandle,osWaitForever);
//	  	  printf("Task 1 ************************************************************\r\n");
//	  	  osMutexRelease(UartMutexHandle);
//  }
//}
//
//void StartDash(void *argument)
//{
//  for(;;)
//  {
//	  int delay =  10 + rand() % (99 - 10 + 1);
//	  osDelay(delay);
//	  osMutexAcquire(UartMutexHandle,osWaitForever);
//	  printf("Task 2 ------------------------------------------------------------\r\n");
//	  osMutexRelease(UartMutexHandle);
//  }
//}
//
//void StartSlash(void *argument)
//{
//  for(;;)
//  {
//	  int delay =  10 + rand() % (99 - 10 + 1);
//	  osDelay(delay);
//	  osMutexAcquire(UartMutexHandle,osWaitForever);
//	  printf("Task 3 ////////////////////////////////////////////////////////////\r\n");
//	  osMutexRelease(UartMutexHandle);
//  }
//}
