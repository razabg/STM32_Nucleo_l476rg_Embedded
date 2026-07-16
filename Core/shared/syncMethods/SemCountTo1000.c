/*
 * TOPIC: FreeRTOS Semaphores — Ping-Pong Signaling Between Two Tasks
 *
 * Two tasks alternate printing numbers 1–1000 in order using two binary
 * semaphores as a handoff signal:
 *
 *   OddSem  starts at count=1  → OddTask runs first
 *   EvenSem starts at count=0  → EvenTask blocks until OddTask releases it
 *
 * OddTask prints its number, then releases EvenSem to unblock EvenTask.
 * EvenTask prints its number, then releases OddSem to unblock OddTask.
 * Each task terminates itself when its counter exceeds 1000.
 *
 * KEY APIS:
 *   osSemaphoreNew(maxCount, initialCount, attr)
 *   osSemaphoreAcquire(handle, timeout)   — blocks if count == 0
 *   osSemaphoreRelease(handle)            — increments count, unblocks waiter
 *
 * In main(), after osKernelInitialize():
 *   OddSemHandle  = osSemaphoreNew(1, 1, &OddSem_attributes);   // starts OPEN
 *   EvenSemHandle = osSemaphoreNew(1, 0, &EvenSem_attributes);  // starts CLOSED
 */

//#include "cmsis_os.h"
//#include <stdio.h>
//
//osSemaphoreId_t OddSemHandle;
//const osSemaphoreAttr_t OddSem_attributes  = { .name = "OddSem"  };
//
//osSemaphoreId_t EvenSemHandle;
//const osSemaphoreAttr_t EvenSem_attributes = { .name = "EvenSem" };
//
//void OddNum1(void *argument)
//{
//    int odd = 1;
//    for(;;)
//    {
//        osSemaphoreAcquire(OddSemHandle, osWaitForever);
//        if (odd > 1000) {
//             osThreadTerminate(osThreadGetId());
//             return;
//        }
//        printf("%d\r\n", odd);
//        odd += 2;
//        osSemaphoreRelease(EvenSemHandle);
//    }
//}
//
//void EvenNum2(void *argument)
//{
//    int even = 2;
//    for(;;)
//    {
//        osSemaphoreAcquire(EvenSemHandle, osWaitForever);
//        if (even > 1000) {
//             osThreadTerminate(osThreadGetId());
//             return;
//        }
//        printf("%d\r\n", even);
//        even += 2;
//        osSemaphoreRelease(OddSemHandle);
//    }
//}
