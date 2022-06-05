#ifndef TWK_OS_CONFIG 
#define TWK_OS_CONFIG 1

#ifndef MAX_TASK_QTY
#define MAX_TASK_QTY 32
#endif 

#ifndef STACK_SIZE
#define STACK_SIZE 128
#endif

#ifndef PRIO_SPLIT
#define PRIO_SPLIT 32
#endif

#ifndef TIME_SLICE
#define TIME_SLICE 1
#endif

#ifndef USE_FPF_TCB_LIST_ARB
#define USE_FPF_TCB_LIST_ARB 1
#endif

#ifndef USE_FPF_TCB_LIST_ARB_REAR_JOIN
#define USE_FPF_TCB_LIST_ARB_REAR_JOIN 1
#endif

#ifndef USE_RR_TCB_ARB
#define USE_RR_TCB_ARB 1
#endif

/**TCB init**/
#ifndef TCB_INIT_DFT_TIME_SLICE
#define TCB_INIT_DFT_TIME_SLICE 1
#endif

#ifndef TCB_INIT_DFT_STACK_NAME
#define TCB_INIT_DFT_STACK_NAME(TCBname) TCBname##_stack
#endif

#ifndef TCB_INIT_DFT_STACK_SIZE
#define TCB_INIT_DFT_STACK_SIZE(stack) sizeof(stack)
#endif

#endif
