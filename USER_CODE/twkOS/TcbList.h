#include "TwkOSutil.h"

void TCBlist_init(TCBlist *list);

void TCBlist_rearJoin(TCBlist *list,TCB_t *tcb);

void TCBlist_headJoin(TCBlist *list,TCB_t *tcb);

void TCBlist_quit(TCBlist *list,TCB_t *tcb);
