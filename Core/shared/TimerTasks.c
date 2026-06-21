#include "TimerTasks.h"
#define MAX_TASKS 10

struct TimerTasks_
{
    TimerTask tasks[MAX_TASKS];
    size_t numTasks;
};


typedef struct
{
    TimerCallbackHandler handler; // pointer to function
    void *param;
    uint32_t periodic;
    uint32_t elapsed;  /* counts up every 10ms tick */
} TimerTask;



