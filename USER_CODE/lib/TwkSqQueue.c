#include "TwkSqQueue.h"


flag sqQueuePeek(SqQueue *queue,void *value){
	byte *ptr = (byte *)value;
	unsigned int i,j;
	if(getSqQueueLength(queue)==0) return FALSE;
	for(i=0,j=queue->front;i<queue->typeSize;i++)
		ptr[i] = queue->array[j++];
	if(j >= queue->capacity*queue->typeSize) j = 0;
	return TRUE;
}

flag deSqQueue(SqQueue *queue,void *value){
	byte *ptr = (byte *)value;
	unsigned int i;
	if(getSqQueueLength(queue)==0) return FALSE;
	for(i=0;i<queue->typeSize;i++)
		ptr[i] = queue->array[queue->front++];
	if(queue->front >= queue->capacity*queue->typeSize) queue->front = 0;
	queue->length--;
	return TRUE;
}

flag enSqQueue(SqQueue *queue,const void *value){
	const byte *ptr = (const byte *)value;
	unsigned int i;
	if(getSqQueueLength(queue)==getSqQueueCapacity(queue)) return FALSE;
	for(i=0;i<queue->typeSize;i++)
		queue->array[queue->rear++] = ptr[i];
	if(queue->rear >= queue->capacity*queue->typeSize) queue->rear = 0;
	queue->length++;
	return TRUE;
}
