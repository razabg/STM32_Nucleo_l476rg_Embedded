#ifndef THREADS_H_
#define THREADS_H_

#include "cmsis_os2.h" // Includes CMSIS-RTOS v2 functions and types
#include "main.h"      // Typically includes HAL and Pin definitions (LD2_Pin, etc.)
#include <stdio.h>     // For printf and scanf

/* ------------------------------------------------------------------------- */
/* Shared OS Identifiers                           */
/* ------------------------------------------------------------------------- */

/**
 * @brief Message Queue ID used to communicate the blink period
 * from UARTThread to blinkThread.
 * @note  This must be defined in your main.c file and created
 * using osMessageQueueNew() before starting the kernel.
 */


/* ------------------------------------------------------------------------- */
/* Thread Prototypes                             */
/* ------------------------------------------------------------------------- */

/**
 * @brief Thread responsible for toggling the LED at a variable rate.
 * @param argument Unused thread argument.
 */
void blinkThread(void *argument);

/**
 * @brief Thread responsible for capturing user input from UART
 * and sending it to the blinkThread via the message queue.
 * @param argument Unused thread argument.
 */
void UARTThread(void *argument);

#endif /* THREADS_H_ */
