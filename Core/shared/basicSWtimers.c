/*
 * ============================================================================
 *  FreeRTOS / CMSIS-RTOS2 — SOFTWARE TIMERS — REFERENCE & CHEAT SHEET
 * ============================================================================
 *
 *  WHAT IS A SOFTWARE TIMER?
 *  --------------------------
 *  A software timer lets you run a function automatically after N ticks,
 *  either once (osTimerOnce) or repeatedly (osTimerPeriodic).
 *  It does NOT use a dedicated hardware timer peripheral (no TIMx). Instead,
 *  FreeRTOS runs ONE background task (the "Timer Service Task" / "RTOS
 *  daemon") which keeps track of every software timer in the project and
 *  calls their callback functions when they expire.
 *
 *  WHY USE IT INSTEAD OF A HARDWARE TIMER?
 *  -----------------------------------------
 *  - You have a limited number of real TIMx peripherals on the chip.
 *  - Software timers are "free" (RAM-only), so you can create as many as
 *    you want.
 *  - Good for "do X every second" / "do X once after a delay" type logic
 *    that doesn't need hardware-level precision.
 *
 *  REQUIREMENTS (usually already set by CubeMX when FreeRTOS is enabled):
 *  ------------------------------------------------------------------------
 *  In FreeRTOSConfig.h:
 *      #define configUSE_TIMERS                 1
 *      #define configTIMER_TASK_PRIORITY         ...
 *      #define configTIMER_QUEUE_LENGTH          ...
 *      #define configTIMER_TASK_STACK_DEPTH      ...
 *
 * ============================================================================
 *  THE 3-STEP RECIPE (this is really the whole API)
 * ============================================================================
 *
 *  STEP 1 — Write the callback:
 *      void MyCallback(void *argument) { ... }
 *
 *  STEP 2 — Create the timer (registers it, does NOT start it):
 *      osTimerId_t h = osTimerNew(MyCallback, osTimerPeriodic, NULL, NULL);
 *
 *  STEP 3 — Start it (arms it, countdown begins now):
 *      osTimerStart(h, 1000);   // 1000 ticks (≈1000ms if tick = 1ms)
 *
 * ============================================================================
 *  HARD RULES — MUST FOLLOW
 * ============================================================================
 *
 *  RULE 1 — Creation order matters.
 *      osTimerNew() MUST be called AFTER osKernelInitialize().
 *      Calling it earlier silently returns NULL (no crash, no warning) —
 *      every osTimerStart() on that NULL handle will then silently do
 *      nothing. This is a classic, hard-to-spot bug.
 *
 *      Correct order in main():
 *          osKernelInitialize();      // 1. kernel wakes up
 *          osTimerNew(...);           // 2. NOW you can create timers
 *          osTimerStart(...);         // 3. arm them
 *          osThreadNew(...);          // 4. create tasks
 *          osKernelStart();           // 5. scheduler takes over — nothing
 *                                     //    written after this in main()
 *                                     //    will ever run
 *
 *  RULE 2 — Keep callbacks SHORT.
 *      All software timer callbacks run inside the SAME shared Timer
 *      Service Task. If one callback blocks (osDelay, waiting on a
 *      semaphore, HAL_Delay, etc.) it freezes EVERY other software timer
 *      in the whole project until it returns. Callbacks should just do
 *      quick direct work: toggle a pin, set a variable, update a PWM
 *      compare register, give a semaphore.
 *
 *  RULE 3 — CMSIS timer functions (osTimerStart/osTimerStop/...) CANNOT be
 *      called from a real hardware ISR (e.g. inside a HAL_..._IRQHandler
 *      or a HAL_xxx_Callback triggered directly by hardware).
 *      From an ISR you must use the native FreeRTOS "FromISR" functions
 *      instead — see the ISR section below.
 *
 * ============================================================================
 */

//#include "cmsis_os.h"
//
///* ============================================================================
// *  EXAMPLE 1 — PERIODIC TIMER (repeats forever)
// *  Blinks an LED every 500 ticks (≈500ms @ 1ms tick)
// * ========================================================================= */
//
//osTimerId_t blinkTimerHandle;
//
//void BlinkTimer_Callback(void *argument)
//{
//    HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);   // quick, direct work only
//}
//
//void Example1_Setup(void)   // call this AFTER osKernelInitialize()
//{
//    blinkTimerHandle = osTimerNew(BlinkTimer_Callback, osTimerPeriodic, NULL, NULL);
//    osTimerStart(blinkTimerHandle, 500);
//}
//

/* ============================================================================
 *  EXAMPLE 2 — ONE-SHOT TIMER (fires exactly once, then stops)
 *  Useful for timeouts / "do X once after N ms" logic.
 *  Can be restarted any time by calling osTimerStart() again.
 * ========================================================================= */

//osTimerId_t oneShotHandle;
//
//void OneShot_Callback(void *argument)
//{
//    HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);  // e.g. turn LED off
//}
//
//void Example2_Setup(void)
//{
//    // Create it — do NOT start yet, some task/event will start it later
//    oneShotHandle = osTimerNew(OneShot_Callback, osTimerOnce, NULL, NULL);
//}
//
///* Somewhere else (e.g. inside a task), whenever you want the delayed
// * action to happen 500 ticks from now: */
//void Example2_TriggerFromTask(void)
//{
//    HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);  // turn LED on now
//    osTimerStart(oneShotHandle, 500);                          // ...off again in 500 ticks
//}
//
//
///* ============================================================================
// *  EXAMPLE 3 — STOPPING A PERIODIC TIMER AFTER N FIRINGS
// *  Demonstrates osTimerStop() called from inside its OWN callback.
// *  This is safe: callbacks run in task context (Timer Service Task),
// *  not in an ISR, so normal RTOS API calls are allowed here.
// * ========================================================================= */
//
//osTimerId_t limitedTimerHandle;
//volatile uint8_t fire_count = 0;
//
//void LimitedTimer_Callback(void *argument)
//{
//    fire_count++;
//
//    // ... do the periodic work here ...
//
//    if (fire_count >= 5)
//    {
//        osTimerStop(limitedTimerHandle);   // stop, but keep it alive for reuse
//        // osTimerDelete(limitedTimerHandle);  // <- use instead if you want to
//                                              //    free it permanently
//    }
//}
//
//void Example3_Setup(void)
//{
//    limitedTimerHandle = osTimerNew(LimitedTimer_Callback, osTimerPeriodic, NULL, NULL);
//    osTimerStart(limitedTimerHandle, 1000);
//}
//
//
///* ============================================================================
// *  EXAMPLE 4 — CHECKING IF A TIMER IS CURRENTLY RUNNING
// * ========================================================================= */
//
//void Example4_CheckStatus(void)
//{
//    if (osTimerIsRunning(blinkTimerHandle))
//    {
//        // it's active — e.g. avoid double-starting it
//    }
//}


/* ============================================================================
 *  API QUICK REFERENCE (CMSIS-RTOS2)
 * ============================================================================
 *
 *  osTimerId_t osTimerNew(
 *      osTimerFunc_t func,       // pointer to callback function
 *      osTimerType_t type,       // osTimerOnce | osTimerPeriodic
 *      void *argument,           // passed into the callback each time it fires
 *      const osTimerAttr_t *attr // usually NULL (dynamic allocation, default name)
 *  );
 *  -> returns the timer handle, or NULL on failure
 *      (NULL almost always means: called before osKernelInitialize())
 *
 *  osStatus_t osTimerStart(osTimerId_t timer_id, uint32_t ticks);
 *  -> arms/re-arms the timer; ticks = period before it fires
 *
 *  osStatus_t osTimerStop(osTimerId_t timer_id);
 *  -> pauses the timer (moved to "dormant" list internally, NOT deleted —
 *     can be restarted later with osTimerStart again)
 *
 *  uint32_t osTimerIsRunning(osTimerId_t timer_id);
 *  -> returns non-zero if currently active
 *
 *  osStatus_t osTimerDelete(osTimerId_t timer_id);
 *  -> permanently destroys the timer and frees its resources
 *
 * ============================================================================
 *  USING TIMER FUNCTIONS FROM A REAL HARDWARE ISR
 * ============================================================================
 *
 *  CMSIS functions above (osTimerStart etc.) are NOT safe to call directly
 *  from a hardware interrupt handler. Use the native FreeRTOS "FromISR"
 *  equivalents instead:
 *
 *      xTimerStartFromISR       (TimerHandle_t xTimer, BaseType_t *pxHigherPriorityTaskWoken)
 *      xTimerStopFromISR        (TimerHandle_t xTimer, BaseType_t *pxHigherPriorityTaskWoken)
 *      xTimerResetFromISR       (TimerHandle_t xTimer, BaseType_t *pxHigherPriorityTaskWoken)
 *      xTimerChangePeriodFromISR(TimerHandle_t xTimer, TickType_t xNewPeriod,
 *                                 BaseType_t *pxHigherPriorityTaskWoken)
 *
 *  Every ISR-safe FreeRTOS function follows this same pattern: it takes a
 *  pointer to a BaseType_t xHigherPriorityTaskWoken, which the function may
 *  set to pdTRUE if a higher-priority task should now run. At the END of
 *  your ISR, pass that flag to portYIELD_FROM_ISR() to let the scheduler
 *  actually switch to it (this switch does NOT happen automatically inside
 *  an interrupt — you must trigger it explicitly).
 *
 *  Example — giving a semaphore from an ISR:
 *
 *      void SomeHandler(void)
 *      {
 *          BaseType_t xHigherPriorityTaskWoken = pdFALSE;
 *          xSemaphoreGiveFromISR(xBinarySemaphore, &xHigherPriorityTaskWoken);
 *          portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
 *      }
 *
 * ============================================================================
 *  DEFERRING HEAVY WORK FROM AN ISR WITHOUT A DEDICATED TASK
 * ============================================================================
 *
 *  ISRs must stay short. If real interrupt-triggered work is heavier than
 *  "toggle a pin", you have two options:
 *
 *   (a) Dedicated task: task blocks on a semaphore; ISR just gives the
 *       semaphore and returns; task wakes up and does the real work.
 *
 *   (b) Reuse the existing Timer Service Task via:
 *
 *       xTimerPendFunctionCallFromISR(
 *           PendedFunction_t xFunctionToPend,
 *           void *pvParameter1,
 *           uint32_t ulParameter2,
 *           BaseType_t *pxHigherPriorityTaskWoken);
 *
 *       This queues your function to run inside the daemon task shortly
 *       after — no extra task needed. Example:
 *
 *       void MyDeferredWork(void *pvParameter1, uint32_t ulParameter2)
 *       {
 *           // the real, possibly-slower work goes here
 *       }
 *
 *       void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
 *       {
 *           BaseType_t xHigherPriorityTaskWoken = pdFALSE;
 *           xTimerPendFunctionCallFromISR(
 *               MyDeferredWork, NULL, 0, &xHigherPriorityTaskWoken);
 *           portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
 *       }
 *
 * ============================================================================
 *  DEBUGGING CHECKLIST — "my timer never fires" / "callback never runs"
 * ============================================================================
 *
 *  [ ] Was osTimerNew() called AFTER osKernelInitialize()? (Rule 1)
 *  [ ] Did you check the return value isn't NULL?
 *  [ ] Was osTimerStart() actually called (not just osTimerNew)?
 *  [ ] Is anything else in the code overwriting the same hardware register
 *      / variable your callback touches, right after the callback runs?
 *  [ ] Is your tick rate actually 1ms? (check configTICK_RATE_HZ in
 *      FreeRTOSConfig.h) — otherwise "1000 ticks" != 1 second
 *  [ ] Does the callback block (osDelay, HAL_Delay, waiting on something)?
 *      If so, it's stalling every other software timer too — remove the
 *      blocking call.
 *  [ ] Remember: code written after osKernelStart() in main() NEVER runs.
 *      Everything must live in tasks, timer callbacks, or before
 *      osKernelStart().
 *
 * ============================================================================
 */
