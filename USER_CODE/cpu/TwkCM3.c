#include "TwkCM3.h"

void NVICenableExtIntr(unsigned int intr){
	NVIC->SETENA[intr>>5] |= 0x00000001UL<<(intr&0x0000001F);
}


void NVICdisableExtIntr(unsigned int intr){
	NVIC->CLRENA[intr>>5] |= 0x00000001UL<<(intr&0x0000001F);
	
}

void NVICsetPrioGroup(unsigned int prioGroup){
	NVIC->AIRCR = NVIC->AIRC & ~(0x7<<8) | VECTKEY | prioGroup<<8;
}
