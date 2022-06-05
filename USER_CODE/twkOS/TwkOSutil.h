#ifndef TWK_OS_UTIL_H
#define TWK_OS_UTIL_H 1

#include "TwkOStype.h"
#include "TwkOSconfig.h"
#include "FpfTcbListArb.h"
#include "RrTcbArb.h"

#define TASK_QTY tidCounter

#ifndef NULL
#define NULL ((void *)0)
#endif

extern State_t osRunning;

extern int tidCounter;
extern TCB_t *curTask;
extern TCB_t *nextTask;
extern TCB_t *taskQueue[MAX_TASK_QTY];

extern TCB_t idleTCB;
extern StackWord_t idleStack[STACK_SIZE];

extern CPUbyte_t leadZeroTab[256];

extern FpfTcbListArb osFpfTcbListArb;

#endif
