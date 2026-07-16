/*
 * TOPIC: FreeRTOS Message Queue — Passing Data Between Tasks
 *
 * A message queue lets tasks send typed values (not just signals) to each
 * other in a thread-safe, buffered way. The queue holds up to N items;
 * the sender blocks if full, the receiver blocks if empty.
 *
 * Flow:
 *   UARTThread  ──reads number from terminal──► osMessageQueuePut(msgQueueID, &delay, ...)
 *   blinkThread ──osMessageQueueGet(msgQueueID, &msg, ...)──► uses value as LED delay
 *
 * In main(), after osKernelInitialize():
 *   msgQueueID = osMessageQueueNew(5, sizeof(int16_t), NULL);
 *
 * KEY APIS:
 *   osMessageQueueNew(count, msgSize, attr)             — create queue (count = max items)
 *   osMessageQueuePut(handle, &msg, priority, timeout)  — enqueue; blocks if full
 *   osMessageQueueGet(handle, &msg, &priority, timeout) — dequeue; blocks if empty
 *
 * NOTE: Both tasks must agree on the message type (uint16_t here for ms delay).
 */

//#include "cmsis_os.h"
//#include <stdio.h>
//
//#include <stdlib.h> // Needed for atoi()
//#include <ctype.h>  // Needed for isdigit()
//
//#include "main.h"
//
//extern UART_HandleTypeDef huart2; //ALREADY IN MAIN C LINE 45
//extern osMessageQueueId_t msgQueueID;
//
//
//void blinkThread(void *argument) {
//
//	uint16_t delay = 500; /* Default delay */
//
//	uint16_t msg = 0;
//
//	osStatus_t status;
//
//	while (1)
//	{
//		status = osMessageQueueGet(msgQueueID, &msg, 0, 10);
//
//		if (status == osOK)
//		{
//
//				delay = msg;
//
//		}
//
//		HAL_GPIO_TogglePin(BLUE_LED_GPIO_Port,BLUE_LED_Pin);
//
//		osDelay(delay);
//	}
//
//}
//
//void UARTThread(void *argument) {
//    char inputBuffer[16];
//    uint8_t idx = 0;
//    uint16_t delay = 0;
//
//    while(1)
//    {
//        printf("\r\nSpecify the LED blink period: ");
//        fflush(stdout);
//        idx = 0;
//
//        while(1)
//        {
//            char ch;
//            // Read 1 byte from UART
//            if(HAL_UART_Receive(&huart2, (uint8_t *)&ch, 1, HAL_MAX_DELAY) == HAL_OK)
//            {
//                // --- HANDLE ENTER KEY ---
//                if(ch == '\r' || ch == '\n')
//                {
//                    if (idx > 0)
//                    {
//                        inputBuffer[idx] = '\0'; // Null-terminate the string
//                        printf("\r\n");
//                        break; // Exit character accumulation loop
//                    }
//                }
//                // --- HANDLE BACKSPACE (ASCII 8 or 127) ---
//                else if(ch == '\b' || ch == 127)
//                {
//                    if(idx > 0)
//                    {
//                        idx--; // Remove last character from our buffer
//                        // Send "Backspace, Space, Backspace" to the terminal to erase it from screen
//                        printf("\b \b");
//                        fflush(stdout);
//                    }
//                }
//                // --- HANDLE NORMAL DIGITS ---
//                else if(isdigit((unsigned char)ch))
//                {
//                    if(idx < sizeof(inputBuffer) - 1)
//                    {
//                        inputBuffer[idx++] = ch;
//                        printf("%c", ch); // Echo character back to terminal
//                        fflush(stdout);
//                    }
//                }
//            }
//        }
//
//        // Convert the clean string to an integer safely
//        delay = (uint16_t)atoi(inputBuffer);
//        printf("Specified period: %hu\r\n", delay);
//
//        // Put the data into your message queue
//        osMessageQueuePut(msgQueueID, &delay, 0, osWaitForever);
//    }
//}
//
//
///*
// * put this main c to run the exersice
// *
// * osMessageQueueId_t msgQueueID;     // ID for your message queue
//
//osThreadId_t blinkThreadID;        // ID for the blink thread
//const osThreadAttr_t blinkThread_attr = {
//  .name = "blinkThread",
//  .stack_size = 128 * 4,
//  .priority = (osPriority_t) osPriorityNormal,
//};
//
//osThreadId_t uartThreadID;         // ID for the UART thread
//const osThreadAttr_t UARTThread_attr = {
//  .name = "UARTThread",
//  .stack_size = 256 * 4,           // Increased slightly because printf/scanf use more stack
//  .priority = (osPriority_t) osPriorityNormal,
//};
//
// *
// *
// *
// *
// *
// *
// * msgQueueID = osMessageQueueNew(5, sizeof( int16_t ), NULL);
//
//    blinkThreadID = osThreadNew(blinkThread, NULL, &blinkThread_attr);
//
//    uartThreadID = osThreadNew(UARTThread, NULL, &UARTThread_attr);
// *
// *
// */
