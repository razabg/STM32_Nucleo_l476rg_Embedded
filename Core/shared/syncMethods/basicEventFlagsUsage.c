/*
 * TOPIC: FreeRTOS Event Flags — Fan-Out / Fan-In Synchronization
 *
 * defaultTask
 *     ↓ creates all tasks + sets FLAG1|FLAG2|FLAG3
 *
 * task1 ──wait FLAG1──► delay 500ms  ──► set DONE1
 * task2 ──wait FLAG2──► delay 700ms  ──► set DONE2
 * task3 ──wait FLAG3──► delay 1100ms ──► set DONE3
 *
 * printTask ──wait ALL DONE (AND)──► print SysTick ──► set FLAG1|FLAG2|FLAG3 again
 *
 * Each cycle takes ~1100ms (the slowest task). printTask fires only after
 * ALL three finish — a classic fan-in barrier using osFlagsWaitAll.
 *
 * KEY APIS:
 *   osEventFlagsNew(attr)
 *   osEventFlagsSet(handle, flags)
 *   osEventFlagsWait(handle, flags, options, timeout)
 *     osFlagsWaitAny — unblocks when ANY listed flag is set
 *     osFlagsWaitAll — unblocks only when ALL listed flags are set simultaneously
 */

//#include "cmsis_os.h"
//#include "main.h"
//#include <stdio.h>
//
//osEventFlagsId_t delayFlags;
//
//#define FLAG_TASK1  0x01
//#define FLAG_TASK2  0x02
//#define FLAG_TASK3  0x04
//
//#define FLAG_DONE1  0x08
//#define FLAG_DONE2  0x10
//#define FLAG_DONE3  0x20
//
//void StartDefaultTask(void *argument)
//{
//    delayFlags = osEventFlagsNew(NULL);
//    waitForBitOneHandle   = osThreadNew(StartBitOne,   NULL, &waitForBitOne_attributes);
//    waitForBitTwoHandle   = osThreadNew(StartBitTwo,   NULL, &waitForBitTwo_attributes);
//    waitForBitThreeHandle = osThreadNew(StartBitThree, NULL, &waitForBitThree_attributes);
//    printTaskHandle       = osThreadNew(StartPrint,    NULL, &printTask_attributes);
//    osEventFlagsSet(delayFlags, FLAG_TASK1 | FLAG_TASK2 | FLAG_TASK3);
//    for(;;)
//    {
//        osDelay(1);
//    }
//}
//
//void StartBitOne(void *argument)
//{
//    for(;;)
//    {
//        osEventFlagsWait(delayFlags, FLAG_TASK1, osFlagsWaitAny, osWaitForever);
//        osDelay(500);
//        osEventFlagsSet(delayFlags, FLAG_DONE1);
//    }
//}
//
//void StartBitTwo(void *argument)
//{
//    for(;;)
//    {
//        osEventFlagsWait(delayFlags, FLAG_TASK2, osFlagsWaitAny, osWaitForever);
//        osDelay(700);
//        osEventFlagsSet(delayFlags, FLAG_DONE2);
//    }
//}
//
//void StartBitThree(void *argument)
//{
//    for(;;)
//    {
//        osEventFlagsWait(delayFlags, FLAG_TASK3, osFlagsWaitAny, osWaitForever);
//        osDelay(1100);
//        osEventFlagsSet(delayFlags, FLAG_DONE3);
//    }
//}
//
//void StartPrint(void *argument)
//{
//    for(;;)
//    {
//        osEventFlagsWait(delayFlags, FLAG_DONE1 | FLAG_DONE2 | FLAG_DONE3,
//                         osFlagsWaitAll, osWaitForever);
//        printf("SysTick: %lu ms\r\n", HAL_GetTick());
//        osEventFlagsSet(delayFlags, FLAG_TASK1 | FLAG_TASK2 | FLAG_TASK3);
//    }
//}
