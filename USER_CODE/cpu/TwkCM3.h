#ifndef TWK_CM3
#define TWK_CM3 1

typedef unsigned int reg32;
typedef unsigned short reg16;
typedef unsigned char reg8;

#define NVIC_BASE_PTR 0xE000E100UL
#define SYS_TICK_BASE_PTR 0xE000E010UL

#define MY_NVIC ((NVIC_t *)NVIC_BASE_PTR)
#define MY_SYS_TICK ((SYS_TICK_t*)SYS_TICK_BASE_PTR) 

#define clkPerSec (5*5000000UL)
#define VECTKEY (0x05FA<<16)

typedef struct{
	reg32 SETENA[8];//0xE000E100 enable extern interruption
	reg32 RESERVED0[24];
	reg32 CLRENA[8];//0xE000E180 disenable extern interruption
	reg32 RSERVED1[24];
	reg32 SETPEND[8];//0xE000E200 pend extern interruption
	reg32 RESERVED2[24];
	reg32 CLRPEND[8];//0xE000E280 don't pend extern interruption
	reg32 RESERVED3[24];
	reg32 ACTIVE[8];//0xE000E300 extern interruption active state
	reg32 RESERVED4[56];
	reg8  PRI[240];//0xE000E400 extern interruption priority
	reg32 RESERVED5[204];
	reg32 AIRCR;//0xE000ED00 Application interruption and reset control
	reg32 ICSR;//0xE000ED04 interruption control and state
	reg32 RESERVED6[4];
	reg8  SPRI;//0xE000ED18 system excption priority
	reg32 RESERVED7[2];
	reg32 SHCSR;//0xE000ED24 system handler control and state
	reg32 RESERVED8[118];
	reg32 STIR;//0xE000EF00 software interruption trigger
}NVIC_t;

typedef struct{
	reg32 CS;//system tick control
	reg32 RELOAD;//system tick reload value
	reg32 CURRENT;//system tick current value
	reg32 CALIB;//system tick calibration
}SYS_TICK_t;

typedef enum{
	RST_TYPE = 1,
	NMI_TYPE,
	HARD_FAULT_TYPE,
	MM_FAULT_TYPE,
	BUS_FAULT_TYPE,
	USAGE_FAULT_TYPE,
	SVC_TYPE = 11,
	DEBUG_TYPE,
	PENDSV_TYPE = 14,
	SYS_TICK_TYPE
}SysExp_t;

#define SYS_TICK_ON() (MY_SYS_TICK->CS |= 0x00000001ul)
#define SYS_TICK_OFF() (MY_SYS_TICK-> &= ~0x00000001ul)
#define SYS_TICK_INTR_ON() (MY_SYS_TICK->CS |= 0x00000002ul)
#define SYS_TICK_INTR_OFF() (MY_SYS_TICK->CS &= ~0x00000002ul)
#define SYS_TICK_USE_FCLK() (MY_SYS_TICK->CS |= 0x00000004ul) //使用内核时钟源
#define SYS_TICK_USE_STCLK() (MY_SYS_TICK->CS &= ~0x00000004ul) //使用外部时钟源
#define SYS_TICK_HAS_OV() (MY_SYS_TICK->CS & 0x00010000ul)

#define SYS_TICK_RELOAD(val) (MY_SYS_TICK->RELOAD = (val))

#define SYS_TICK_RD_CUR() (MY_SYS_TICK->CURRENT)
#define SYS_TICK_RST_CUR() (MY_SYS_TICK->CURRENT = 0)


//中断与异常使能
void NVICenableExtIntr(unsigned int intr); 
void NVICdisableExtIntr(unsigned int intr);

#define NVIC_ENABLE_USAGE_FAULT() (MY_NVIC->CHCSR |= 0x00000001UL)
#define NVIC_DISENABLE_USAGE_FAULT() (MY_NVIC->CHCSR &= ~0x00000001UL)

#define NVIC_ENABLE_BUS_FAULT() (MY_NVIC->CHCSR |= 0x00000002UL)
#define NVIC_DISENABLE_BUS_FAULT() (MY_NVIC->CHCSR &= ~0x00000002UL)

#define NVIC_ENABLE_MM_FAULT() (MY_NVIC->CHCSR |= 0x00000004UL)
#define NVIC_DISENABLE_MM_FAULT() (MY_NVIC->CHCSR &= ~0x00000004UL)

//中断与异常挂起
#define NVIC_NMIPEND_SET() (MY_NVIC->ICSR |= 0x00000001UL<<31)

#define NVIC_PENDSV_SET() (MY_NVIC->ICSR |= 0x00000001UL<<28)
#define NVIC_PENDSV_CLR() (MY_NVIC->ICSR |= 0x00000001UL<<27)

#define NVIC_PENDST_SET() (MY_NVIC->ICSR |= 0x00000001UL<<26)
#define NVIC_PENDST_CLR() (MY_NVIC->ICSR |= 0x00000001UL<<25)


//设置优先级
#define NVIC_SET_SYS_EXP_PRIO(exp,prio) (MY_NVIC->SPRI[exp] = (prio))
#define NVIC_SET_INTR_PRIO(intr,prio) (MY_NVIC->PRI[exp] = (prio))
#endif
