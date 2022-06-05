#include "cpu.h"
#include "TwkOS.h"

void SysTick_Handler(void){
	TwkOS_tickProc();
}

void sysTickInit(int ms){
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
	SysTick->LOAD  = ms * clkPerSec / 1000 ;
	NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);
	SysTick->VAL   = 0;
	SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
				     SysTick_CTRL_TICKINT_Msk ;
}
