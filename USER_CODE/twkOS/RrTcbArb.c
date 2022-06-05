#include "RrTcbArb.h"
#include "TcbList.h"

TCB_t *RrTcbArb_getNextTask(TCBlist *list){
	TCB_t *tcb = list->head;
	if(list->TCBqty == 0){
		return NULL;
	}else if(list->TCBqty == 1){
		return tcb;
	}else if(tcb->unusedTimeSlice != 0){
		tcb->unusedTimeSlice--;
		return tcb;
	}else{
		tcb->unusedTimeSlice = tcb->timeSlice;
		TCBlist_quit(list,tcb);
		TCBlist_rearJoin(list,tcb);
		return list->head;
	}
}
