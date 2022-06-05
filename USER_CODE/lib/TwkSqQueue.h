#ifndef TWK_SQ_QUEUE_H
#define TWK_SQ_QUEUE_H 1

#include "TwkLib.h"

typedef struct{
	byte *array;
	unsigned int capacity;
	unsigned int typeSize;
	unsigned int length;
	unsigned int front;
	unsigned int rear;
}SqQueue;


#define getSqQueueLength(queue) ((queue)->length)
#define getSqQueueCapacity(queue) ((queue)->capacity)
#define clrSqQueue(queue) ((queue)->length = 0,(queue)->front = 0,(queue)->rear = 0)
#define constructSqQueue(queue,arr,cap,type) (\
(queue)->array = (byte *)(arr),\
(queue)->capacity = (cap),\
(queue)->typeSize = sizeof(type),\
(queue)->length = 0,\
(queue)->front = 0,\
(queue)->rear = 0)

flag sqQueuePeek(SqQueue *queue,void *value);
flag deSqQueue(SqQueue *queue,void *value);
flag enSqQueue(SqQueue *queue,const void *value);
#endif
