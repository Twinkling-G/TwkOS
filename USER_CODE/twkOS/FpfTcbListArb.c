#include "FpfTcbListArb.h"
#include "TcbList.h"

void FpfTcbListArb_init(FpfTcbListArb *arb){
	int i;
	for(i = 0;i < PRIO_SPLIT;i++){
		TCBlist_init(arb->list+i);
	}
	for(i = 0 ; i < PRIO_BIT_FLAG_LEN ;i++){
		arb->prioBitFlag[i] = 0x00000000UL;
	}
}

TCBlist *FpfTcbListArb_getNextTask(FpfTcbListArb *arb){
	int i,prio;
	PrioBitFlagWord_t w;
	prio = 0;
	for(i = 0;i < PRIO_BIT_FLAG_LEN;i++){
		if(arb->prioBitFlag[i] != 0){
			w = arb->prioBitFlag[i];

			if(w >> 16 != 0){
				w >>= 16;
			}else{
				prio += 16;
			}

			if(w >> 8 != 0){
				w >>= 8;
			}else{
				prio += 8;
			}

			return arb->list+(prio+leadZeroTab[w]);
		}
		prio+=32;
	}
	return NULL;
}

void FpfTcbListArb_headJoin(FpfTcbListArb *arb,TCB_t *tcb){
	int prio = tcb->prio;
	TCBlist *list = arb->list+prio;
	if(list->TCBqty == 0){
		arb->prioBitFlag[prio>>5] |= 0x80000000UL >> (prio & 0x0000001FUL);
	}
	TCBlist_headJoin(list,tcb);
}

void FpfTcbListArb_rearJoin(FpfTcbListArb *arb,TCB_t *tcb){
	int prio = tcb->prio;
	TCBlist *list = arb->list+prio;
	if(list->TCBqty == 0){
		arb->prioBitFlag[prio>>5] |= 0x80000000UL >> (prio & 0x0000001FUL);
	}
	TCBlist_rearJoin(list,tcb);
}


void FpfTcbListArb_quit(FpfTcbListArb *arb,TCB_t *tcb){
	int prio = tcb->prio;
	TCBlist *list = arb->list+prio;
	TCBlist_quit(list,tcb);
	if(list->TCBqty == 0){
		arb->prioBitFlag[prio>>5] &= ~(0x80000000UL >> (prio & 0x0000001FUL));
	}
}

