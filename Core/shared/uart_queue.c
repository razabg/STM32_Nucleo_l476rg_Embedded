#include "uart_queue.h"
#define QUEUE_SIZE 256

extern UART_HandleTypeDef huart2;
Queue *queue = NULL;

struct Queue {
	char * m_qu;
	uint16_t size;
	uint16_t head;
	uint16_t tail;
	uint16_t n_items;
};


Queue * Queue_create(void){

	Queue *queue = (Queue *)malloc(sizeof(Queue));
	if(!queue){
		return NULL;
	}
	queue->size = QUEUE_SIZE; //in
	queue->m_qu = (char*)malloc(sizeof(char) * queue->size);
	if(!queue->m_qu){
		free(queue);
		return NULL;
	}
	queue->tail = 0;
	queue->head = 0;
	queue->n_items = 0;

	return queue;

}


int Queue_enque(Queue * queue,char * data,uint16_t size){
	if(!queue) {return 0;}
	if(size == queue->size) {return 0;}
	if(queue->n_items + size > queue->size){ return 0; }

	for (int var = 0; var < size; ++var) {
		queue->m_qu[queue->tail] = data[var];
		queue->tail = (queue->tail + 1) % queue->size;
		queue->n_items++;
	}

	return 1;

}

char Queue_deque(Queue * queue){
	char byte;
	if(!queue) {return 0;}
	if(queue->n_items == 0) {return 0;}
	byte = queue->m_qu[queue->head];
	queue->head = (queue->head + 1) % queue->size;
	queue->n_items--;

	return byte;
}

uint16_t Queue_size(Queue * queue){
	return queue->n_items;
}


void uartSend(char * data,uint16_t size){

	if(HAL_BUSY == HAL_UART_Transmit_IT(&huart2, (uint8_t *)data, size)){
		Queue_enque(queue, data, size);
	}

}



void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
	uint8_t data;
	if(Queue_size(queue) > 0)
	{
		data =	Queue_deque(queue);
		if(data)
		{
			HAL_UART_Transmit_IT(&huart2,&data,1);
		}
	}

}







