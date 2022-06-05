#include "TwkOS.h"

void TwkOS_init(void){
	//硬件初始化
	sysSrcInit();
	//全局资源初始化
	osRunning = 0;
	tidCounter = 0;
	curTask = &idleTCB;
	FpfTcbListArb_init(&osFpfTcbListArb);
	//创建空闲任务
	TCB_init(&idleTCB,idleStack,STACK_SIZE,idleTask,NULL,PRIO_SPLIT-1,1);
	TwkOS_createTask(&idleTCB);
}

void TwkOS_start(){
	if(!osRunning){
		osRunning = 1;
		nextTask = RrTcbArb_getNextTask(FpfTcbListArb_getNextTask(&osFpfTcbListArb));
		if(nextTask != curTask)
			gotoNextTask();
	}
}

void TwkOS_createTask(TCB_t *tcb){
#if USE_FPF_TCB_LIST_ARB
#if USE_FPF_TCB_LIST_ARB_REAR_JOIN
	FpfTcbListArb_rearJoin(&osFpfTcbListArb,tcb);
#else
	FpfTcbListArb_headJoin(&osFpfTcbListArb,tcb);
#endif
#endif
}

void TwkOS_deleteTask(TCB_t *tcb){
#if USE_FPF_TCB_LIST_ARB
	FpfTcbListArb_quit(&osFpfTcbListArb,tcb);
#endif
}

void TwkOS_tickProc(void){
	nextTask = RrTcbArb_getNextTask(FpfTcbListArb_getNextTask(&osFpfTcbListArb));
	if(nextTask != curTask)
		gotoNextTask();
}

void idleTask(void *arg){
	while(1){

	}
}

void TCB_init(TCB_t *tcb,Stack_t stk,
	int stkSize,
	Task_t task,
	void *arg,
	Prio_t prio,
	int timeSlice
	){
	StackWord_t *stkTop;
	tcb->stkTop = stk+stkSize;
	tcb->stkSize = stkSize;
	tcb->prio = prio;
	tcb->timeSlice = timeSlice;
	tcb->unusedTimeSlice = timeSlice;
	tcb->next = NULL;
	tcb->last = NULL;
	
	stkTop = tcb->stkTop;
	*(--stkTop) =  (StackWord_t)0x01000000UL; //bit24 means Thumb instruction set
	*(--stkTop) =  (StackWord_t)task;
	*(--stkTop) =  (StackWord_t)0xEEEEEEEEUL;
	*(--stkTop) =  (StackWord_t)0xCCCCCCCCUL;
	*(--stkTop) =  (StackWord_t)0x33333333UL;
	*(--stkTop) =  (StackWord_t)0x22222222UL;
	*(--stkTop) =  (StackWord_t)0x11111111UL;
	*(--stkTop) =  (StackWord_t)arg;
	*(--stkTop) =  (StackWord_t)0xBBBBBBBBUL;
	*(--stkTop) =  (StackWord_t)0xAAAAAAAAUL;
	*(--stkTop) =  (StackWord_t)0x99999999UL;
	*(--stkTop) =  (StackWord_t)0x88888888UL;
	*(--stkTop) =  (StackWord_t)0x77777777UL;
	*(--stkTop) =  (StackWord_t)0x66666666UL;
	*(--stkTop) =  (StackWord_t)0x55555555UL;
	*(--stkTop) =  (StackWord_t)0x44444444UL;
	tcb->stkTop = stkTop;
}
