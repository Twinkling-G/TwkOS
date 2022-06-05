#include "TwkOSutil.h"

State_t osRunning;

int tidCounter;
TCB_t *curTask;
TCB_t *nextTask;
TCB_t *taskQueue[MAX_TASK_QTY];

TCB_t idleTCB;
StackWord_t idleStack[STACK_SIZE];
FpfTcbListArb osFpfTcbListArb;

CPUbyte_t leadZeroTab[256] ={
	8u,//00000000
	7u,//00000001
	6u,6u,//00000010,00000011
	5u,5u,5u,5u,//00000100-00000111
	4u,4u,4u,4u,4u,4u,4u,4u,//00001000-00001111
	3u,3u,3u,3u,3u,3u,3u,3u,3u,3u,3u,3u,3u,3u,3u,3u,
	//00010000-00011111
	2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u, 
	2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,2u,
	//00100000-00111111
	1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u, 
	1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u, 
	1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,
	1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,1u,
	//01000000-01111111
	0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u, 
	0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,
	0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u, 
	0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u, 
	0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u, 
	0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u, 
	0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u, 
	0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u
	//10000000-11111111
};
