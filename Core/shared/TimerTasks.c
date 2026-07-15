#include "TimerTasks.h"
#include <stdlib.h>
#define MAX_TASKS 10



typedef struct
{
    TimerCallbackHandler handler; // pointer to function
    void *param;
    uint32_t periodic;
    uint32_t elapsed;  /* counts up every 10ms tick */
} TimerTask;


struct TimerTasks_
{
    TimerTask tasks[MAX_TASKS];
    uint16_t numTasks;
};





TimerTasks *TimerTasks_create(){

	TimerTasks  *tts_ = (TimerTasks*)malloc(sizeof(TimerTasks));
	if (!tts_)
	{
		return NULL;
	}

	tts_->numTasks = 0;

	return tts_;


}


TimerTasksResult TimerTasks_registerCallback(TimerTasks *_t, TimerCallbackHandler _func, void *_obj, uint32_t _periodic)
{
	if (!_t)
	{
		return TIMER_TASKS_UNINITIALIZED_ERROR;
	}

	if (!_func) {
		return TIMER_TASKS_NULL_HANDLER_ERROR;
	}

	if (_periodic == 0 || (_periodic % 10) != 0)
	{
	    return TIMER_TASKS_INVALID_PERIOD_ERROR;
	}

	if (_t->numTasks == 10) {
		return TIMER_TASKS_FULL_ERROR;
	}


	_t->tasks[_t->numTasks].handler = _func;
	_t->tasks[_t->numTasks].param = _obj;
	_t->tasks[_t->numTasks].periodic = _periodic;
	_t->tasks[_t->numTasks].elapsed = 0;

	_t->numTasks++;


	return TIMER_TASKS_SUCCESS;
}


TimerTasksResult TimerTasks_interrupt(TimerTasks *_t)
{
	int i = 0;

	if (!_t)
	{
		return TIMER_TASKS_UNINITIALIZED_ERROR;
	}

	for (i = 0; i< _t->numTasks; ++i) {
		_t->tasks[i].elapsed += 10;
		if (_t->tasks[i].elapsed >= _t->tasks[i].periodic ) {
			_t->tasks[i].handler(_t->tasks[i].param); // call the function
			_t->tasks[i].elapsed = 0;
		}
	}

   return TIMER_TASKS_SUCCESS;
}




//* USER CODE BEGIN 4 */
////*************************************************** this is for the timer tasks exercise put in main.c
//void Periodic_LedToggle_Callback(void *argument)
//{
//    Led_Task *task = (Led_Task *)argument;
//    if (task != NULL)
//    {
//        if (task->color == 0) {
//            counterRed++;
//        } else {
//            counterBlue++;
//        }
//        HAL_GPIO_TogglePin(task->port, task->pin);
//    }
//}
//
//
//void Periodic_PrintCounters_Callback(void *argument)
//{
//    // No loops, no osDelay!
//    // This function is invoked by the scheduler exactly every 3000ms.
//    printf("red :%d  blue :%d\r\n", counterRed, counterBlue);
//}
//
///* Corrected HAL ISR override */
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//    if (htim->Instance == TIM7)
//    {
//        // Call your ADT interrupt handler here
//        TimerTasks_interrupt(tts);
//    }
//}

