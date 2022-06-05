	AREA code,CODE,readonly

	IMPORT curTask
	IMPORT nextTask
	IMPORT idleStack
	
	EXPORT svcHandler
	EXPORT cpuExitPriv
	EXPORT asm_cpsie
	EXPORT asm_cpsid
		
	EXPORT PendSV_Handler
	EXPORT SVC_Handler

svcHandler PROC
	LDR R1,=svcList
	CMP R0,#2
	ITE LT
	ADDLT R1,R1,R0,LSL#1
	LDRGE R1,=svcEnd
	BX R1

svcList 
	B svc0
	B svc1
svc0
	SVC 0
	B svcEnd
svc1
	SVC 1
	B svcEnd
svcEnd
	BX LR		
	ENDP

cpuExitPriv PROC
	MRS R0,CONTROL
	ORR R0,R0,#0x1 ;set CONTROL[0]
	MSR CONTROL,R0
	ENDP

asm_cpsie
	CPSIE I
	BX LR

asm_cpsid
	CPSID I
	BX LR

PendSV_Handler PROC
	CPSID I
	
	LDR R2,=curTask     ;R2 <- address of address of current task TCB 
	LDR R0,[R2]         ;R0 <- address of current task TCB
	
	;store task
	MRS R1,PSP
	STMDB R1!,{R4-R11}
	STR R1,[R0]         ;stack top of current task TCB<- R1,R1 is stack top of current task
	
	;load task
	LDR R0,=nextTask			;R0 <- address of address of next task TCB 
	LDR R0,[R0]					;R0 <- address of next task TCB   
	STR R0,[R2]					;update current Task pointer
	LDR R0,[R0]					;R0 <- stack top
	LDMIA R0!,{R4-R11}
	MSR PSP,R0

	;return
	ORR LR,LR,#0x04 ;use psp
	CPSIE I
	BX LR 				;在异常过程中，LR是一个特殊的值。在CM3中都是Thumb状态所以最低位固定为1。handler模式不能使用PSP,因此0xFFFFFFF5是非法值。
						;如果在一个异常过程中嵌套一个异常过程，返回的时候，不可以通过ORR LR,LR,#0x04强制使返回使用psp，
						;因为返回是handler模式，LR也就是0xFFFFFFF5，这样做会导致一个UsageFault。					
	ENDP

SVC_Handler PROC
	TST LR,#0x4
	ITE EQ
	MRSEQ R0,MSP 	;update flag zero by testing LR&#0x4,flag zero is high(true) 
	MRSNE R0,PSP	;update flag zero by testing LR&#0x4,flag zero is low(false)
	
	LDR R0, [R0,#24] ;6*4 = 24 偏移6个寄存器的长度
	LDRB R0, [R0,#-2] ;R0指向SVC指令的下一条指令，所以要R0减去一个值来指向SVC指令。CM3使用小端存储，故SVC指令的立即数在低地址，故减2而不是减1。读取的是一个字节立即数。
	
	CBNZ R0, svcHandler1
	;svcHandler0:CPU ENTER PRIV
	MRS R1,CONTROL
	BIC R1,R1,#0x1 ;clear CONTROL[0]
	MSR CONTROL,R1
	B svcHandlerEnd

svcHandler1
	CMP R0,#1
	BNE svcHandlerEnd
	PUSH {LR}
	BL registerInit
	B svcHandlerEnd

svcHandlerEnd
	POP {PC}
	ENDP


registerInit PROC
	LDR R0,=idleStack
	MSR PSP,R0 			;初始化PSP为空闲任务栈
	BX LR
	ENDP
		
	nop
	END


;--------------------------------
;exception stack frame format
;--------------------------------
;high address|xPSR
;            |PC
;            |LR
;            |R12
;            |R3                    A
;            |R2                    |  pop
;            |R1                    |
;low address |R0  <--stack top
;            |---R11                |
;            |...                   |  push
;            |---R4                 V 
;--------------------------------
