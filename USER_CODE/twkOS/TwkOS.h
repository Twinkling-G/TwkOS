#ifndef TWK_OS_H
#define TWK_OS_H 1

#include "TwkOSutil.h"//系统资源

void TwkOS_init(void);

void TwkOS_start(void);

void TwkOS_createTask(TCB_t *tcb);

void TwkOS_deleteTask(TCB_t *tcb);

void TwkOS_tickProc(void);

void idleTask(void *arg);

void TCB_init(TCB_t *tcb,
	Stack_t stk,
	int stkSize,
	Task_t task,
	void *arg,
	Prio_t prio,
	int timeSlice
);	

#define TCB_dftInit(tcb,task,arg,prio) \
	TCB_init(&tcb,\
		TCB_INIT_DFT_STACK_NAME(tcb),\
		TCB_INIT_DFT_STACK_SIZE(TCB_INIT_DFT_STACK_NAME(tcb)),\
		task,\
		arg,\
		prio,\
		TCB_INIT_DFT_TIME_SLICE\
)

#endif 
