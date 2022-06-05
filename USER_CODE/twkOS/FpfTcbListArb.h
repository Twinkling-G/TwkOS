#ifndef FPF_TCB_LIST_ARB_H
#define FPF_TCB_LIST_ARB_H 1

#include "TwkOStype.h"
#include "TwkOSconfig.h"

#define PRIO_BIT_FLAG_LEN (((PRIO_SPLIT-1)>>5)+1)

typedef struct{
	TCBlist list[PRIO_SPLIT];//put on head of struction
	PrioBitFlagWord_t prioBitFlag[PRIO_BIT_FLAG_LEN];//32bit per word
}FpfTcbListArb;

void FpfTcbListArb_init(FpfTcbListArb *arb);

TCBlist *FpfTcbListArb_getNextTask(FpfTcbListArb *arb);

void FpfTcbListArb_headJoin(FpfTcbListArb *arb,TCB_t *tcb);

void FpfTcbListArb_rearJoin(FpfTcbListArb *arb,TCB_t *tcb);

void FpfTcbListArb_quit(FpfTcbListArb *arb,TCB_t *tcb);

#endif
