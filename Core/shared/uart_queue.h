#ifndef __UARTQUEUE_H__
#define __UARTQUEUE_H__
#include "main.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef struct Queue Queue;

Queue * Queue_create(void);
int Queue_enque(Queue * queue,char * data,uint16_t size);
char Queue_deque(Queue * queue);
uint16_t Queue_size(Queue * queue);
void uartSend(char * data,uint16_t size);



#endif
