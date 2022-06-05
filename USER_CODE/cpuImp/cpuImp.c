#include "TwkOS.h"

void sysSrcInit(void){
	SystemInit();
	//将PSP初始化为指向空闲任务栈
	svcHandler(REGISTER_INIT);
	//设置PendSV异常
	NVIC_SetPriority(PendSV_IRQn, (1<<__NVIC_PRIO_BITS) - 1);
	//设置系统时钟异常
	if(TIME_SLICE){
		sysTickInit(TIME_SLICE);
		SYS_TICK_ON();
	}
}

void gotoNextTask(void){
	NVIC_PENDSV_SET();
}

void cpuEnableIntr(void){
	asm_cpsie();
}	

void cpuDisableIntr(void){
	asm_cpsid();
}
