#include "TwkLPC1768.h"

//global_value
static const byte capitalPin0Port[4]={1,1,0,0};//4个CAPn.0端口号
static const byte capitalPin0Pin[4]={26,18,4,23};//4个CAPn.0引脚号
static const byte capitalPin1Port[4]={1,1,0,0};//4个CAPn.1端口号
static const byte capitalPin1Pin[4]={27,19,5,24};//4个CAPn.1引脚号

static const byte timer0ExtMatchPinPort[4]={1,1,3,3};
static const byte timer0ExtMatchPinPin[4]={28,29,25,26};
static const byte timer1ExtMatchPinPort[2]={1,1};
static const byte timer1ExtMatchPinPin[2]={22,25};
static const byte timer2ExtMatchPinPort[6]={0,0,0,0,4,4};
static const byte timer2ExtMatchPinPin[6]={6,7,8,9,28,29};
static const byte timer3ExtMatchPinPort[2]={0,0};
static const byte timer3ExtMatchPinPin[2]={10,11};

static const byte uartTXDport[6]={0,0,0,0,2,2};
static const byte uartTXDpin[6]={2,15,10,0,0,8};
static const byte uartRXDport[6]={0,0,0,0,2,2};
static const byte uartRXDpin[6]={3,16,11,1,1,9};
static const byte uartSel[6]={SEL_TRXD0,SEL_TRXD1_L,SEL_TRXD2_L,SEL_TRXD3,SEL_TRXD1_H,SEL_TRXD2_H};

static const byte timerPower[4] = {TIMER0_POWER_T,TIMER2_POWER_T,TIMER2_POWER_T,TIMER3_POWER_T};
static const reg32 timerMatchReg[4]={TIMER_MATCH0_HEAD,TIMER_MATCH1_HEAD,TIMER_MATCH2_HEAD,TIMER_MATCH3_HEAD};
static const reg32 timerCapitalReg[2]={TIMER_CAPITAL0_HEAD,TIMER_CAPITAL1_HEAD};
static const reg32 fullWord[4]={0x00000000,0x55555555,0xAAAAAAAA,0xFFFFFFFF};
const reg32 timerRegOffset[4] = {0,TIMER1_OFFSET,TIMER2_OFFSET,TIMER3_OFFSET};
const reg32 UARTregOffset[4] = {0,UART1_OFFSET,UART2_OFFSET,UART3_OFFSET};
const reg32 GP_REG[5] = {0x40024044UL,0x40024048UL,0x4002404CUL,0x40024050UL,0x40024054UL};

reg32 oneSecClk=24000000UL;
reg32 oneMsClk=24000UL;
reg32 oneUsClk=24UL;
//global_value_end


//定时器默认为定时器模式，PR为0，所有寄存器资源可用，无中断服务函数
Timer timer0Obj={0,TIMER_MODE,0,0,0x0,{0}}; Timer*timer0 = &timer0Obj;
Timer timer1Obj={1,TIMER_MODE,0,0,0x0,{0}}; Timer*timer1 = &timer1Obj;
Timer timer2Obj={2,TIMER_MODE,0,0,0x0,{0}}; Timer*timer2 = &timer2Obj;
Timer timer3Obj={3,TIMER_MODE,0,0,0x0,{0}}; Timer*timer3 = &timer3Obj;

UART uart0Obj = {0,0,FALSE,NULL,NULL,NULL,NULL,NULL}; UART *uart0 = &uart0Obj;
UART uart1Obj = {1,0,FALSE,NULL,NULL,NULL,NULL,NULL}; UART *uart1 = &uart1Obj;;
UART uart2Obj = {2,0,FALSE,NULL,NULL,NULL,NULL,NULL}; UART *uart2 = &uart2Obj;
UART uart3Obj = {3,0,FALSE,NULL,NULL,NULL,NULL,NULL}; UART *uart3 = &uart3Obj;

SPI spiObj = {FALSE,NULL,NULL};SPI *spi = &spiObj;
//global_value_end


//设置port端口的pin引脚的功能，sel等于SEL_GPIO时，引脚为GPIO功能
void setSel(byte port,byte pin,byte sel) {
	reg32 *ptr = (reg32 *)(PINSEL_HEAD+(port<<3)+((pin>>4)<<2));
	*ptr = *ptr & ~(0x3<<((pin&0xF)<<1)) | sel<<((pin&0xF)<<1);
}

//设置port端口以beginPin开始往后len个引脚的功能
void setSels(byte port,byte beginPin,byte sel,unsigned short len){
	reg32 *ptr = (reg32 *)(PINSEL_HEAD+(port<<3)+((beginPin>>4)<<2));
	*ptr = *ptr & ~(~(~0x0<<(len<<1))<<((beginPin&0xF)<<1)) 
	| fullWord[sel]&~(~0x0<<(len<<1))<<((beginPin&0xF)<<1);
}

//设置port端口的pin引脚的模式，模式有上拉PULL_UP、下拉PULL_DOWN、中继REPEATOR、无NO_PULL
void setMode(byte port,byte pin,byte mode) {
	reg32 *ptr = (reg32 *)(PINMODE_HEAD+(port<<3)+((pin>>4)<<2));
	*ptr = *ptr & ~(0x3<<((pin&0xF)<<1)) | mode<<((pin&0xF)<<1);
}

//设置port端口以beginPin开始往后len个引脚的模式
void setModes(byte port,byte beginPin,byte mode,unsigned short len){
	reg32 *ptr = (reg32 *)(PINMODE_HEAD+(port<<3)+((beginPin>>4)<<2));
	*ptr = *ptr & ~(~(~0x0<<(len<<1))<<((beginPin&0xF)<<1)) //~0x0<<(len<<1) likes 111...11110000
	| fullWord[mode]&~(~0x0<<(len<<1))<<((beginPin&0xF)<<1);//~(~0x0<<(len<<1))<<(beginPin<<1) likes 000...0011110000
}

//设置port端口的pin引脚的是否开漏
void setModeOD(byte port,byte pin,byte mode){
	reg32 *ptr = (reg32 *)(PINMODE_OD_HEAD+(port<<2));
	*ptr = *ptr & ~(0x1<<pin) | mode<<pin;
}

//设置port端口以beginPin开始往后len个引脚是否开漏
void setModeODs(byte port,byte beginPin,byte mode,unsigned short len){
	reg32 *ptr = (reg32 *)(PINMODE_OD_HEAD+(port<<2));
	*ptr = *ptr & ~(~(~0x0<<len)<<beginPin)//~0x0<<len likes 111...1111000
	|fullWord[mode?3:0]&~(~0x0<<len)<<beginPin;//~(~0x0<<len)<<beginPin likes 000...00111000
}

//设置port端口的pin引脚的方向
void setDir(byte port,byte pin,byte dir) {
	reg32 *ptr = (reg32 *)(FIODIR_HEAD+(port<<5));
	*ptr = *ptr & ~(0x1<<pin) | (dir?HIGH:LOW)<<pin;
}

//设置port端口以beginPin开始往后len个引脚的方向
void setDirs(byte port,byte beginPin,byte dir,unsigned short len) {
	reg32 *ptr = (reg32 *)(FIODIR_HEAD+(port<<5));
	*ptr = *ptr & ~(~(~0x0<<len)<<beginPin)
	|fullWord[dir?3:0]&~(~0x0<<len)<<beginPin;
}

//初始化port端口pin引脚为GPIO，模式可选输入INPUT、保持输入HOLD_INPUT、推挽输出STRONG_OUTPUT、开漏输出WEAK_OUTPUT
flag setGPIO(byte port,byte pin,byte mode){
	if(mode == INPUT){
		setMode(port,pin,PULL_UP);
	}else if(mode == HOLD_INPUT){
		setMode(port,pin,REPEATOR);
	}else if(mode == STRONG_OUTPUT){
		setMode(port,pin,NO_PULL);
		setModeOD(port,pin,PUSH_PULL);
	}else if(mode == WEAK_OUTPUT){
		setModeOD(port,pin,OPEN_DRAIN);
		setMode(port,pin,PULL_UP);
	}else return -2;
	setSel(port,pin,SEL_GPIO);
	setDir(port,pin,mode&0x2);//INPUT和HOLD_INPUT的第二位都是0，STRONG_OUTPUT和WEAK_OUTPUT的第二位都是1
	return 0;
}

//初始化port端口以beginPin开始往后len个引脚为GPIO
flag setGPIOs(byte port,byte beginPin,byte mode,unsigned short len){
	if(mode == INPUT){
		setModes(port,beginPin,PULL_UP,len);
	}else if(mode == HOLD_INPUT){
		setModes(port,beginPin,REPEATOR,len);
	}else if(mode == STRONG_OUTPUT){
		setModes(port,beginPin,NO_PULL,len);
		setModeODs(port,beginPin,PUSH_PULL,len);
	}else if(mode == WEAK_OUTPUT){
		setModeODs(port,beginPin,OPEN_DRAIN,len);
		setModes(port,beginPin,PULL_UP,len);
	}else return -2;
	setSels(port,beginPin,SEL_GPIO,len);
	setDirs(port,beginPin,mode,len);
	return 0;
}

//写port端口以beginPin开始的长度为len的引脚，word的低位写在连续引脚序列的低位
void wrBits(byte port,byte beginPin,reg32 word,unsigned short len){
	*(reg32 *)(FIOSET_HEAD+(port<<5)) = (~(~0x0<<len)&word)<<beginPin;
	*(reg32 *)(FIOCLR_HEAD+(port<<5)) = (~(~0x0<<len)&~word)<<beginPin;
}

//设置中断优先级，priority取值0~31，数值越低优先级越高
void setIntrPriority(byte intrType,byte priority){
	reg32 *ptr = (reg32*)(INTR_PRIORITY_HEAD+(intrType&~0x3));
	*(reg32 *)ptr = *(reg32 *)ptr & ~(0xF8<<((intrType&0x3)<<3))
	| (priority&0x1F)<<((intrType&0x3)<<3)+3;
}

//priority取值0~5,EXTINTR0在p2.10,EXTINTR1在p2.11,EXTINTR2在p2.12,EXTINTR3在p2.13,因此port默认为2
//mode可选LOW,HIGH,RISING_EDGE,FALLING_EDGE
void setExtIntr(byte pin,byte mode,byte priority){
	setSel(2,10+pin,SEL_EXT_INTR);
	*(reg32 *)EXT_INTR_REQ |= 0x1<<pin;
	*(reg32 *)EXT_INTR_MODE = *(reg32 *)EXT_INTR_MODE & ~(0x1<<pin) | (mode&0x6?0x1:0x0)<<pin;
	*(reg32 *)EXT_INTR_POLAR = *(reg32 *)EXT_INTR_POLAR & ~(0x1<<pin) | (mode&0x3?0x1:0x0)<<pin;
	setIntrPriority(pin+EXT0_INTR_T,priority);
	enableIntr(pin+EXT0_INTR_T);
}


//取消timerPtr指向的定时器中matchReg的匹配
flag cancelMatch(Timer *timer,byte matchReg){
	if(timer->timerType>3||matchReg>3) return -2;
	*(reg32 *)(timerMatchReg[matchReg]+timerRegOffset[timer->timerType]) &= ~(0x7<<matchReg*3);
	timer->regUsage &= ~(0x1<<matchReg);
//	if(!timer->regUsage) disableIntr(timer->timerType+1);
	return 0;
}

//取消timerPtr指向的定时器中capitalReg的捕获
flag cancelCapital(Timer *timer,byte capitalReg){
	if(timer->timerType>3||capitalReg>1) return -2;
	*(reg32 *)(timerCapitalReg[capitalReg]+timerRegOffset[timer->timerType]) &= ~(0x7<<capitalReg*3);
	timer->regUsage &= ~(0x1<<capitalReg+4);
//	if(!timer->regUsage) disableIntr(timer->timerType+1);
	return 0;
}

//取消timerPtr指向的定时器中matchReg的外部匹配引脚控制
flag cancelExtMatchPin(Timer *timer,byte matchReg){
	if(timer->timerType>3||matchReg>3) return -2;
	*(reg32 *)(TIMER_EXT_MATCH_PIN_CTRL_HEAD+timerRegOffset[timer->timerType]) &= ~(0x3<<4+(matchReg<<1));
	return 0;
}


//设置timerPtr指向的定时器为定时器模式还是计数器模式，并设置定时模式下的分频PR或计数模式下计数的引脚cntPin
//mode可选TIMER_MODE,RISING_EDGE,FALLING_EDGE,BOTH_EDGE(等效于RISING_EDGE|FALLING_EDGE)
flag setTimerMode(Timer *timer,byte mode,reg32 scale,byte cntPin){
	reg32 *ptr;
	if(timer->timerType>3 || cntPin>1 || scale<1 ) return -2;
	peripheralPowerOn(timerPower[timer->timerType]);
	timerRst(timer);
	ptr = (reg32 *)(TIMER_MODE_CTRL_HEAD+timerRegOffset[timer->timerType]);
	*ptr &= ~0xF;
	if(mode==TIMER_MODE) {
		timer->mode = TIMER_MODE;
		setTCcyc(timer,scale);
	}else{
		timer->mode = COUNTER_MODE;
		timer->cntPin = cntPin; 
		setSel(cntPin?capitalPin1Port[timer->timerType]:capitalPin0Port[timer->timerType],
		cntPin?capitalPin1Pin[timer->timerType]:capitalPin0Pin[timer->timerType],SEL_CAPITAL);
		*ptr = *ptr | mode>>1 | cntPin<<2;
		cancelCapital(timer,cntPin);
	}
	return 0;
}

//设置timerPtr指向的定时器当匹配到matchReg里的值val，执行动作action，如果func不为NULL，则执行中断，func为对应的中断服务函数
//action可选TIMER_RST_ACTION,TIMER_OFF_ACTION
flag attachMatch(Timer *timer,byte matchReg,reg32 val,byte action,void(*func)(void)){ 
	reg32 *ptr;
	byte shift;
	if(timer->timerType>3||matchReg>3) return -2;

	if(func){
		timer->intrFuncList[matchReg] = func;
		action |= INTR_ACTION;
	}else action &= ~INTR_ACTION;
	
	timerRst(timer);
	clrTimerIntrs(timer);
	wrPC(timer,0);
	
	ptr = (reg32 *)(TIMER_MATCH_CTRL_HEAD+timerRegOffset[timer->timerType]);
	shift = matchReg*3;
	*ptr = *ptr & ~(0x7<<shift) | action>>3<<shift;
	*(reg32 *)(timerMatchReg[matchReg]+timerRegOffset[timer->timerType]) = val;

	timer->regUsage |= 0x1<<matchReg;
	if(func) enableIntr(timer->timerType+1);
	return 0;
}


//设置timerPtr指向的定时器当捕获寄存器capitalReg对应的捕获引脚出现action指定的动作，捕获寄存器会捕获TC的值，如果func不为NULL，则执行中断，func为对应的中断服务函数
//action可选RISING_EDGE,FALLING_EDGE的组合
flag attachCapital(Timer *timer,byte capitalReg,byte action,void(*func)(void)){
	reg32 *ptr;
	if(timer->timerType>3||capitalReg>1
	||capitalReg==timer->cntPin&&timer->mode==COUNTER_MODE) return -2;

	if(func){
		timer->intrFuncList[4+capitalReg] = func;
		action |= INTR_ACTION;
	}else action &= ~INTR_ACTION;

	timerRst(timer);
	clrTimerIntrs(timer);
	wrPC(timer,0);

	setSel(
		capitalReg?
		capitalPin1Port[timer->timerType]:
		capitalPin0Port[timer->timerType],
		capitalReg?
		capitalPin1Pin[timer->timerType]:
		capitalPin0Pin[timer->timerType],
		SEL_CAPITAL);
	ptr = (reg32 *)(TIMER_CAPITAL_CTRL_HEAD+timerRegOffset[timer->timerType]);
	*ptr = *ptr & ~(0x7<<(capitalReg?3:0)) | (action>>1)<<(capitalReg?3:0);
	timer->regUsage |= 0x1<4+capitalReg;
	if(func) enableIntr(timer->timerType+1);
	return 0;
}

//设置timerPtr指向的定时器当匹配到matchReg的值,对对应的外部匹配引脚做出相应的动作action，有点matchReg会对应2个外部匹配引脚，若要选择高序的，highPin为1，若要选则低序的，highPin为0
//action可选NO_ACTION,SET_LOW_ACTION,SET_HIGH_ACTION,REV_ACTION
flag attachExtMatchPin(Timer *timer,byte matchReg,byte action,...){
	reg32 *ptr;
	flag highPin;
	if(timer->timerType>3||matchReg>3||timer->timerType!=2&&matchReg>1) return -2;
	ptr = (reg32 *)(TIMER_EXT_MATCH_PIN_CTRL_HEAD+timerRegOffset[timer->timerType]);
	*ptr = * ptr & ~(0x3<<4+(matchReg<<1)) | (action&0x3)<<4+(matchReg<<1);
	if(timer->timerType==0){
		highPin =*(flag *)((byte *)&action+sizeof(int));
		setSel(
			timer0ExtMatchPinPort[highPin?2+matchReg:matchReg],
			timer0ExtMatchPinPin[highPin?2+matchReg:matchReg],
			highPin?SEL_MATCH0_H:SEL_MATCH0_L);
	}else if(timer->timerType==1){
		setSel(
			timer1ExtMatchPinPort[matchReg],
			timer1ExtMatchPinPin[matchReg],
			SEL_MATCH1);
	}else if(timer->timerType==2){
		highPin = FALSE;
		if(matchReg<=1) highPin =*(flag *)((byte *)&action+sizeof(int));
		setSel(
			timer2ExtMatchPinPort[highPin?4+matchReg:matchReg],
			timer2ExtMatchPinPin[highPin?4+matchReg:matchReg],
			highPin?SEL_MATCH2_0A1_H:SEL_MATCH2_L);
	}else{
		setSel(
			timer3ExtMatchPinPort[matchReg],
			timer3ExtMatchPinPin[matchReg],
			SEL_MATCH3);
	}
	return 0;
}

//写timerPtr指向的定时器的外部匹配引脚(4bit)，写入值为val
flag wrExtMatchPins(Timer *timer,reg32 val) {
	reg32 *ptr;
	if(timer->timerType>3) return -2;
	ptr = (reg32 *)(TIMER_EXT_MATCH_PIN_CTRL_HEAD+timerRegOffset[timer->timerType]);
	*ptr = *ptr & ~0xF | val & 0xF;
	return 0;
} 

void t1Delay(reg32 time,reg32 timeCyc){
	reg32 TCval = rdTC(timer1);
	reg32 PCval = rdPC(timer1);
	reg32 PRval = rdPR(timer1);
	timerRst(timer1);
	wrPC(timer1,0);
	setTCcyc(timer1,timeCyc);
	*(reg32 *)(TIMER_MATCH_CTRL_HEAD+TIMER1_OFFSET) = 	*(reg32 *)(TIMER_MATCH_CTRL_HEAD+TIMER1_OFFSET) & ~0x7 | TIMER_OFF_ACTION>>3;
	*(reg32 *)(TIMER_MATCH0_HEAD+TIMER1_OFFSET) = time;
	timerOn(timer1);
	while(*(reg32*)(TIMER_CTRL_HEAD+TIMER1_OFFSET));
	wrTC(timer1,TCval);
	wrPC(timer1,PCval);
	wrPR(timer1,PRval);
}


void UARTbegin(UART * uart,reg32 baudRate,SqQueue *buffer,...){
	reg32 div = (oneSecClk>>4)/baudRate;
	if((uart->UARTtype == 1||uart->UARTtype == 2)
		&& *(flag *)( (byte*)&buffer+sizeof(int) ) ){//存储UART1的高位引脚信息的位置在数组的第4位置(4+1-1)而不是第5位置(4+1)
		setSel(uartTXDport[3+uart->UARTtype],uartTXDpin[3+uart->UARTtype],uartSel[3+uart->UARTtype]);
		setSel(uartRXDport[3+uart->UARTtype],uartRXDpin[3+uart->UARTtype],uartSel[3+uart->UARTtype]);
	}else{
		setSel(uartTXDport[uart->UARTtype],uartTXDpin[uart->UARTtype],uartSel[uart->UARTtype]);
		setSel(uartRXDport[uart->UARTtype],uartRXDpin[uart->UARTtype],uartSel[uart->UARTtype]);
	}
	*(reg32 *)(LINE_CTRL_HEAD+UARTregOffset[uart->UARTtype]) = 0x83;
	*(reg32 *)(DIV_LATCH_LSB_HEAD+UARTregOffset[uart->UARTtype]) = div&0xFF;
	*(reg32 *)(DIV_LATCH_MSB_HEAD+UARTregOffset[uart->UARTtype]) = div>>8&0xFF;
	*(reg32 *)(LINE_CTRL_HEAD+UARTregOffset[uart->UARTtype]) = 0x03;
	*(reg32 *)(FIFO_CTRL_HEAD+UARTregOffset[uart->UARTtype]) = 0x07;
	uart->vaild = FALSE;
	uart->buffer = buffer;
	if(buffer) setRBRintr(uart0,NULL,NULL,FOURTEEN_BYTE_TRIGGER);
}

byte UARTpeekByte(UART * uart){
	byte b;
	if(uart->buffer) {
		if(getSqQueueLength(uart->buffer)){
			sqQueuePeek(uart->buffer,&b);
			return b;
		}
	}else{
		if(uart->vaild) return uart->recTop;
		if(*(reg32 *)(LINE_STAT_HEAD+UARTregOffset[uart->UARTtype])&0x01){
			uart->vaild = TRUE;
			return uart->recTop = *(reg32 *)(RBR_HEAD+UARTregOffset[uart->UARTtype]);
		}
	}
	return 0xFF;
}

byte UARTrdByte(UART * uart){
	byte b;
	if(uart->buffer) {
		if(getSqQueueLength(uart->buffer)){
			deSqQueue(uart->buffer,&b);
			return b;
		}
	}else{
		if(uart->vaild) {
			uart->vaild = FALSE;
			return uart->recTop;
		}
		if(*(reg32 *)(LINE_STAT_HEAD+UARTregOffset[uart->UARTtype])&0x01)
			return *(reg32 *)(RBR_HEAD+UARTregOffset[uart->UARTtype]);
	}
	return 0xFF;
}

void UARTwrByte(UART *uart,byte b){
	reg32 *ptr = (reg32 *)LINE_STAT_HEAD+UARTregOffset[uart->UARTtype];
	while(!(*ptr&0x40));
	*(reg32 *)(THR_HEAD+UARTregOffset[uart->UARTtype]) = b;
}

void UARTwrBytes(UART *uart,const byte *stream,reg32 len){
	reg32 *ptr = (reg32 *)LINE_STAT_HEAD+UARTregOffset[uart->UARTtype];
	while(len--){
		while(!(*ptr&0x40));
		*(reg32 *)(THR_HEAD+UARTregOffset[uart->UARTtype]) = *stream++;
	}
}

unsigned int UARTputs(UART *uart,const char *str){
	unsigned int charNum = 0;
	reg32 *LSRptr = (reg32 *)(LINE_STAT_HEAD+UARTregOffset[uart->UARTtype]);
	reg32 *THRptr = (reg32 *)(THR_HEAD+UARTregOffset[uart->UARTtype]);
	while(*str){
		while(!(*LSRptr&0x40));
		*THRptr = *str++;
		charNum++;
	}
	return charNum;
}

void setRLSintr(UART * uart,void (*RLSintrFunc)(void)){
	if(RLSintrFunc==NULL) return;
	*(reg32 *)(UART_INTR_EN_HEAD+UARTregOffset[uart->UARTtype]) |= 0x04;
	uart->RLSintrFunc = RLSintrFunc;
	enableIntr(5+uart->UARTtype);
}

void setRBRintr(UART * uart,void(*RDAintrFunc)(void),void(*CTIintrFunc)(void), byte RBRTrigger){
	*(reg32 *)(UART_INTR_EN_HEAD+UARTregOffset[uart->UARTtype]) |= 0x01;
	uart->RDAintrFunc = RDAintrFunc;
	uart->CTIintrFunc = CTIintrFunc;
	*(reg32 *)(FIFO_CTRL_HEAD+UARTregOffset[uart->UARTtype]) = 0x01|RBRTrigger<<6;
	enableIntr(5+uart->UARTtype);
}

void setTHREintr(UART * uart,void (*THREintrFunc)(void)){
	if(THREintrFunc==NULL) return;
	*(reg32 *)(UART_INTR_EN_HEAD+UARTregOffset[uart->UARTtype]) |= 0x02;
	uart->THREintrFunc = THREintrFunc;
	enableIntr(5+uart->UARTtype);
}

void RBRintrFunc(UART *uart){
	volatile byte trash;
	SqQueue *buffer = uart->buffer;
	reg32 *LSRptr = (reg32 *)(LINE_STAT_HEAD+UARTregOffset[uart->UARTtype]);
	reg32 *RBRptr = (reg32 *)(RBR_HEAD+UARTregOffset[uart->UARTtype]);
	
	while(*LSRptr&0x01){
		if(getSqQueueLength(buffer)==getSqQueueCapacity(buffer)) {
			*(reg32 *)(FIFO_CTRL_HEAD+UARTregOffset[uart->UARTtype]) |= 0x3;
			trash = *RBRptr;
			return;
		}
		buffer->array[buffer->rear++] = *RBRptr;
		if(buffer->rear >= buffer->capacity) buffer->rear = 0;
		buffer->length++;
	}
}

//清空读缓存区
void UARTflushBuffer(UART *uart){
	if(uart->buffer){
		clrSqQueue(uart->buffer);
	}else{
		*(reg32 *)(FIFO_CTRL_HEAD+UARTregOffset[uart->UARTtype]) = 0x03;
		uart->vaild = FALSE;
	}
}

void SPIbegin(byte mode,byte frameSize,reg32 baudRate){
	reg32 div = oneSecClk/baudRate;
	if(div < 8) div= 8;
	if(frameSize<8) frameSize = 8;
	*(reg32 *)SPI_CLK = div & ~0x1;
	*(reg32 *)SPI_CTRL = mode & ~SPIINTR_EN | (frameSize & 0xF) << 8;//帧大小为9位，最高位为有效位
	spi->vaild = FALSE;
	setSel(0,15,SEL_SPI);
	setSel(0,16,SEL_SPI);
	setSel(0,17,SEL_SPI);
	setSel(0,18,SEL_SPI);
}


void setSPIintr(void (*MODF)(void),void (*SPIF)(void)){
	if(MODF==NULL||SPIF==NULL) return; 
	spi->MODFintrFunc = MODF;
	spi->SPIFintrFunc = SPIF;
	*(reg32 *)SPI_CTRL |= SPIINTR_EN;
	enableIntr(SPI_INTR_T);
}

byte SPIavailable(){
	volatile reg32 trash;
 	if(spi->vaild) return TRUE;
 	if(SPIswapEnd&&*(reg32 *)SPI_DATA&SPI_VAILD){
		if(SPImode == SPI_SLAVE) SPIdata&=~SPI_VAILD;
 		return spi->vaild = TRUE;
 	}
 	if(SPImode == SPI_MASTER){
 		*(reg32 *)SPI_DATA = 0;
		while(!SPIswapEnd);
		if(*(reg32 *)SPI_DATA&SPI_VAILD) return	spi->vaild = TRUE;	
 	}
 	return FALSE;
 }

byte SPIpeekByte(){
	if(spi->vaild) return *(reg32 *)SPI_DATA;
	if(SPIswapEnd&&*(reg32 *)SPI_DATA&SPI_VAILD){
		if(SPImode == SPI_SLAVE) *(reg32 *)SPI_DATA&=~SPI_VAILD;
		return *(reg32 *)SPI_DATA;
	}
	if(SPImode == SPI_MASTER){
		*(reg32 *)SPI_DATA = 0;
		while(!SPIswapEnd);
		if(*(reg32 *)SPI_DATA&SPI_VAILD) return	*(reg32 *)SPI_DATA;
	}
	return 0xFF;
}

byte SPIrdByte(){
	if(spi->vaild) {
		spi->vaild = FALSE;
		return *(reg32 *)SPI_DATA;
	}
	if(SPIswapEnd&&*(reg32 *)SPI_DATA&SPI_VAILD){
		if(SPImode == SPI_SLAVE) *(reg32 *)SPI_DATA&=~SPI_VAILD;
		return *(reg32 *)SPI_DATA;
	}
	if(SPImode == SPI_MASTER){
		*(reg32 *)SPI_DATA = 0;
		while(!SPIswapEnd);
		if(*(reg32 *)SPI_DATA&SPI_VAILD)	return	*(reg32 *)SPI_DATA;	
	}
	return 0xFF;
}

void SPIwrByte(byte b){
	volatile reg32 trash;
	trash = *(reg32 *)SPI_STAT;
	trash = *(reg32 *)SPI_DATA;
	spi->vaild = FALSE;
	*(reg32 *)SPI_DATA = SPI_VAILD|b;
	if(SPImode == SPI_MASTER) while(!SPIswapEnd);
}


unsigned int SPIswapByte(unsigned int w){
	volatile reg32 trash = *(reg32 *)SPI_STAT;
	*(reg32 *)SPI_DATA = w;
	while(!SPIswapEnd);
	return *(reg32 *)SPI_DATA;
}

void getTime(Time *t){
	t->sec = RTCsec;
	t->min = RTCmin;
	t->hour = RTChour;
	t->dom = RTCdom;
	t->dow = RTCdow;
	t->doy = RTCdoy;
	t->month = RTCmonth;
	t->year = RTCyear;
}		
