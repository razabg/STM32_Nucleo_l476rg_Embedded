//#ifndef __TIMER_TASKS_H__
//#define __TIMER_TASKS_H__
//
//#include <stdint.h>
//
//typedef struct TimerTasks_ TimerTasks;
//typedef void (*TimerCallbackHandler)(void *);
//
//typedef enum TimerTasks_Result
//{
//    TIMER_TASKS_SUCCESS,
//    TIMER_TASKS_UNINITIALIZED_ERROR,  /**< Uninitialized TimerTasks error      */
//    TIMER_TASKS_ALLOCATION_ERROR,     /**< Memory allocation failure            */
//    TIMER_TASKS_NULL_HANDLER_ERROR,   /**< Provided callback handler is NULL    */
//    TIMER_TASKS_INVALID_PERIOD_ERROR, /**< Period is 0 or not a multiple of 10  */
//    TIMER_TASKS_FULL_ERROR            /**< Max number of tasks already reached  */
//} TimerTasksResult;
//
///**
// * @brief Dynamically create a new TimerTasks object.
// * @return TimerTasks* - on success / NULL on fail
// */
//TimerTasks *TimerTasks_create();
//
///**
// * @brief Register a callback function to be called periodically.
// * @param[in] _t       - Pointer to TimerTasks object.
// * @param[in] _func    - Function pointer to the callback handler.
// * @param[in] _obj     - User provided parameter passed to the callback on invocation.
// * @param[in] _periodic - Period in ms at which the callback will be called.
// *                        Must be a multiple of 10ms.
// * @return TimerTasksResult
// * @retval TIMER_TASKS_SUCCESS on success
// * @retval TIMER_TASKS_UNINITIALIZED_ERROR if _t is NULL
// * @retval TIMER_TASKS_NULL_HANDLER_ERROR if _func is NULL
// * @retval TIMER_TASKS_INVALID_PERIOD_ERROR if _periodic is 0 or not a multiple of 10
// * @retval TIMER_TASKS_FULL_ERROR if max tasks already registered
// */
//TimerTasksResult TimerTasks_registerCallback(TimerTasks *_t, TimerCallbackHandler _func, void *_obj, uint32_t _periodic);
//
///**
// * @brief Should be called from the timer ISR every 10ms.
// * @details Iterates over all registered callbacks and invokes those
// *          whose period has elapsed.
// * @param[in] _t - Pointer to TimerTasks object.
// * @return TimerTasksResult
// * @retval TIMER_TASKS_SUCCESS on success
// * @retval TIMER_TASKS_UNINITIALIZED_ERROR if _t is NULL
// */
//TimerTasksResult TimerTasks_interrupt(TimerTasks *_t);
//
//#endif /* __TIMER_TASKS_H__ */
