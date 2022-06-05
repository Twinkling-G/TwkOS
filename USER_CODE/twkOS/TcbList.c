#include "TcbList.h"

void TCBlist_init(TCBlist *list){
	list->head = NULL;
	list->rear = NULL;
	list->TCBqty = 0;
}

void TCBlist_rearJoin(TCBlist *list,TCB_t *tcb){
	if(list->TCBqty == 0){
		tcb->next = NULL;
		tcb->last = NULL;

		list->head = tcb;
		list->rear = tcb;
	}else{
		tcb->next = NULL;
		tcb->last = list->rear;

		list->rear->next = tcb;
		list->rear = tcb;
	}

	list->TCBqty++;
}

void TCBlist_headJoin(TCBlist *list,TCB_t *tcb){
	if(list->TCBqty == 0){
		tcb->next = NULL;
		tcb->last = NULL;

		list->head = tcb;
		list->rear = tcb;
	}else{
		tcb->next = list->head;
		tcb->last = NULL;

		list->head->last = tcb;
		list->head = tcb;
	}

	list->TCBqty++;
}

void TCBlist_quit(TCBlist *list,TCB_t *tcb){
	if(tcb->next){
		tcb->next->last = tcb->last;	
	}else{
		list->rear = tcb->last;
	}	

	if(tcb->last){
		tcb->last->next = tcb->next;
	}else{
		list->head = tcb->next;
	}

	tcb->next = NULL;
	tcb->last = NULL;	
	list->TCBqty--;
}
