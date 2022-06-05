#ifndef CPU_H
#define CPU_H 1

#include "LPC17xx.h"
#include "cpuType.h"

#define clkPerSec (5*5000000UL)

#ifndef NVIC_ICSR 
#define NVIC_ICSR (*(CPUword_t *)0xE000ED04)
#endif

#define REGISTER_INIT 1

//PendSV
#define NVIC_PENDSV_SET() (NVIC_ICSR |= 0x00000001UL<<28)

//SVC
#define CPU_ENTER_PRIV() svcHandler(0)
#define CPU_EXIT_PRIV() cpuExitPriv()
void cpuExitPriv(void);//asm
void svcHandler(CPUbyte_t code);//asm

#define SYS_TICK_ON() (SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk)
#define SYS_TICK_OFF() (SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk)
void sysTickInit(int ms);

void asm_cpsie(void);//asm
void asm_cpsid(void);//asm

void SysTick_Handler(void);
void PendSV_Handler(void);//asm
void SVC_Handler(void);//asm
#endif
