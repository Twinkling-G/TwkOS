#ifndef TWK_OS_TYPE_H
#define TWK_OS_TYPE_H 1

#include "cpuImp.h"
typedef CPUword_t Prio_t;
typedef CPUword_t StackWord_t;
typedef CPUword_t *Stack_t;
typedef CPUword_t PrioBitFlagWord_t;
typedef CPUbyte_t State_t;
typedef void(*Task_t)(void *);

typedef struct TCB_t{
	Stack_t stkTop;
	int stkSize;
	//int tid;
	Prio_t prio;
	int timeSlice;
	int unusedTimeSlice; 
	struct TCB_t *next;
	struct TCB_t *last;
}TCB_t;

typedef struct{
	TCB_t *head;
	TCB_t *rear;
	int TCBqty;
}TCBlist;

#endif
