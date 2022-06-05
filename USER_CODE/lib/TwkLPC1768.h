#ifndef TWK_LPC1768_H
#define TWK_LPC1768_H 1
//author:黄聪
//使用本库完成作业请标注作者！

/*return byte:
0:return normally
-1:memory allocation error
-2:syntax error
-3:no useable source
*/
#include "TwkSqQueue.h"

//reg:
#define PERIPHERAL_POWER  0x400FC0C4UL

#define PINSEL_HEAD 0x4002C000UL
#define PINMODE_HEAD 0x4002C040UL
#define PINMODE_OD_HEAD 0x4002C068UL
#define FIODIR_HEAD 0x2009C000UL
#define FIOPIN_HEAD 0x2009C014UL
#define FIOSET_HEAD 0x2009C018UL
#define FIOCLR_HEAD 0x2009C01CUL
#define FIOMASK_HEAD 0x2009C010UL

#define EXT_INTR_REQ 0x400FC140UL
#define EXT_INTR_MODE 0x400FC148UL
#define EXT_INTR_POLAR 0x400FC14CUL

#define INTR_EN0 0xE000E100UL//中断使能寄存器
#define INTR_EN1 0xE000E104UL
#define INTR_DISEN0 0xE000E180UL//中断禁用寄存器
#define INTR_DISEN1 0xE000E184UL
#define INTR_PRIORITY_HEAD 0xE000E400UL//中断优先级寄存器

#define INTR_SET_PENDING0 0xE000E200UL//中断挂起寄存器
#define INTR_SET_PENDING1 0xE000E204UL
#define INTR_CLR_PENDING0 0xE000E280UL//中断取消挂起寄存器
#define INTR_CLR_PENDING1 0xE000E284UL

#define IO_INTR_REQ 0x40028080UL//read only
#define IO0_INTR_REQ_RISING 0x40028084UL//port0上升沿的GPIO中断状态//read only
#define IO2_INTR_REQ_RISING 0x400280A4UL//port2上升沿的GPIO中断状态//read only
#define IO0_INTR_REQ_FALLING 0x40028088UL//port0下降沿的GPIO中断状态//read only
#define IO2_INTR_REQ_FALLING 0x400280A8UL//port2下降沿的GPIO中断状态//read only

#define IO0_INTR_REQ_CLR 0x4002808CUL//port0的GPIO中断清零//write only
#define IO2_INTR_REQ_CLR 0x400280ACUL//port2的GPIO中断清零//write only
#define IO0_INTR_EN_RISING 0x40028090UL//port0的GPIO上升沿中断使能
#define IO2_INTR_EN_RISING 0x400280B0UL//port2的GPIO上升沿中断使能
#define IO0_INTR_EN_FALLING 0x40028094UL//port0的GPIO下降沿中断使能
#define IO2_INTR_EN_FALLING 0x400280B4UL//port2的GPIO下降沿中断使能

//UART_reg
#define THR_HEAD 0x4000C000UL
#define RBR_HEAD 0x4000C000UL
#define DIV_LATCH_LSB_HEAD 0x4000C000UL
#define DIV_LATCH_MSB_HEAD 0x4000C004UL
#define UART_INTR_EN_HEAD 0x4000C004UL
#define UART_INTR_REQ_HEAD 0x4000C008UL
#define FIFO_CTRL_HEAD 0x4000C008UL
#define LINE_CTRL_HEAD 0x4000C00CUL
#define LINE_STAT_HEAD 0x4000C014UL

#define UART1_OFFSET 0x00010000UL
#define UART2_OFFSET 0x0008C000UL
#define UART3_OFFSET 0x00090000UL
//UART_reg_end

//SPI_reg
#define SPI_CTRL 0x40020000UL
#define SPI_STAT 0x40020004UL
#define SPI_DATA 0x40020008UL
#define SPI_CLK 0x4002000CUL
#define SPI_TEST_CTRL 0x40020010UL
#define SPI_TEST_STAT 0x40020014UL
#define SPI_INTR_REQ 0x4002001CUL
//SPI_reg_end

//timer_reg
//working_reg
#define TIMER_INTR_REQ_HEAD 0x40004000UL
#define TIMER_TC_HEAD 0x40004008UL
#define TIMER_PR_HEAD 0x4000400CUL
#define TIMER_PC_HEAD 0x40004010UL
//working_reg_end
//sample_reg
#define TIMER_MATCH0_HEAD 0x40004018UL
#define TIMER_MATCH1_HEAD 0x4000401CUL
#define TIMER_MATCH2_HEAD 0x40004020UL
#define TIMER_MATCH3_HEAD 0x40004024UL
#define TIMER_CAPITAL0_HEAD 0x4000402CUL
#define TIMER_CAPITAL1_HEAD 0x40004030UL
//sample_reg_end
//ctrl_reg
#define TIMER_MODE_CTRL_HEAD 0x40004070UL
#define TIMER_CTRL_HEAD 0x40004004UL
#define TIMER_CAPITAL_CTRL_HEAD 0x40004028UL
#define TIMER_MATCH_CTRL_HEAD 0x40004014UL
#define TIMER_EXT_MATCH_PIN_CTRL_HEAD 0x4000403CUL
//ctrl_reg_end

#define TIMER1_OFFSET 0x00004000UL
#define TIMER2_OFFSET 0x0008C000UL
#define TIMER3_OFFSET 0x00090000UL
//timer_reg_end

//RTC_reg
#define RTC_INTR_REQ 0x40024000UL
#define RTC_CLK_CRTL 0x40024008UL
#define RTC_INC_INTR 0x4002400CUL
#define RTC_ALARM_INTR 0x40024010UL
#define CONSOLIDATED_TIMR0 0x40024014UL
#define CONSOLIDATED_TIMR1 0x40024018UL
#define CONSOLIDATED_TIMR2 0x4002401CUL

#define SEC_CNT 0x40024020UL
#define MIN_CNT 0x40024024UL
#define HOUR_CNT 0x40024028UL
#define DOM_CNT 0x4002402CUL
#define DOW_CNT 0x40024030UL
#define DOY_CNT 0x40024034UL
#define MONTH_CNT 0x40024038UL
#define YEAR_CNT 0x4002403CUL
#define CALIBRATION 0x40024040UL

#define ALARM_SEC 0x40024060UL
#define ALARM_MIN 0x40024064UL
#define ALARM_HOUR 0x40024068UL
#define ALARM_DOM 0x4002406CUL
#define ALARM_DOW 0x40024070UL
#define ALARM_DOY 0x40024074UL
#define ALARM_MONTH 0x40024078UL
#define ALARM_YEAR 0x4002407CUL
//RTC_reg_end
//reg_end

//circuit_connection
#define PULL_UP 0x0
#define REPEATOR 0x1
#define NO_PULL 0x2
#define PULL_DOWN 0x3
#define PUSH_PULL 0x0
#define OPEN_DRAIN 0x1
//circuit_connection_end

//pin_mode
#define INPUT 0x0//输入
#define HOLD_INPUT 0x1//保持输入
#define STRONG_OUTPUT 0x2//推挽输出
#define WEAK_OUTPUT 0x3//开漏输出
//pin_mode_end

//pin_function
#define SEL_GPIO 0x0//GPIO
#define SEL_EXT_INTR 0x1//外部中断功能
#define SEL_CAPITAL 0x3//timer捕获功能
//match
//MAT0.0,MAT0.1 only
#define SEL_MATCH0_L 0x3//timer0的低序MAT引脚功能
#define SEL_MATCH0_H 0x2//timer0的高序MAT引脚功能
//MAT1.0,MAT1.1 only
#define SEL_MATCH1 0x3//timer1的MAT引脚功能
//MAT2.0,MAT2.1,MAT2.2,MAT2.3
#define SEL_MATCH2_L 0x3//timer2的低序MAT引脚功能
#define SEL_MATCH2_0A1_H 0x2//timer2的高序MAT2.0和MAT2.1引脚功能
//MAT3.0,MAT3.1 only
#define SEL_MATCH3 0x3//timer3的MAT引脚功能
//match_end
//TRXD
#define SEL_TRXD0 0x1
#define SEL_TRXD1_L 0x1
#define SEL_TRXD1_H 0x2
#define SEL_TRXD2_L 0x1
#define SEL_TRXD2_H 0x2
#define SEL_TRXD3 0x2
//TRXD_end
#define SEL_SPI 0x3
//pin_function_end

//signal_type
#define LOW 0x00//低电平
#define HIGH 0x01//高电平
#define RISING_EDGE 0x02 //上升沿
#define FALLING_EDGE 0x04 //下升沿
#define BOTH_EDGE RISING_EDGE|FALLING_EDGE//双边沿
//signal_type_end


//timer
#define DEFAULT_TIMER_SCALE oneMsClk
//timer_mode
#define TIMER_MODE 0x0//定时模式
#define COUNTER_MODE 0x1//计数模式
//timer_mode_end

//timer_action
#define NO_ACTION 0x00
#define SET_LOW_ACTION 0x01
#define SET_HIGH_ACTION 0x02
#define REV_ACTION SET_LOW_ACTION|SET_HIGH_ACTION
#define INTR_ACTION 0x08 //中断动作
#define TIMER_RST_ACTION 0x10//计数器复位
#define TIMER_OFF_ACTION 0x20//计数器复位
//timer_action_end
//timer_end

//RTC_time_type
#define RTC_SEC 0x01
#define RTC_MIN 0x02
#define RTC_HOUR 0x04
#define RTC_DOM 0x08
#define RTC_DOW 0x10
#define RTC_DOY 0x20
#define RTC_MONTH 0x40
#define RTC_YEAR 0x80
//RTC_time_type_end

//RTC_interruption_type
#define RTC_INC_INTR_T 0x1
#define RTC_ALARM_INTR_T 0x2
//RTC_interruption_type_end

//RTC_time_val
#define RTCsec (*(reg32 *)SEC_CNT)
#define RTCmin (*(reg32 *)MIN_CNT)
#define RTChour (*(reg32 *)HOUR_CNT)
#define RTCdom (*(reg32 *)DOM_CNT)
#define RTCdow (*(reg32 *)DOW_CNT)
#define RTCdoy (*(reg32 *)DOY_CNT)
#define RTCmonth (*(reg32 *)MONTH_CNT)
#define RTCyear (*(reg32 *)YEAR_CNT)
//RTC_time_val_end

//peripheral_power_type
#define TIMER0_POWER_T 1
#define TIMER1_POWER_T 2
#define TIMER2_POWER_T 22
#define TIMER3_POWER_T 23
//peripheral_power_type_end

//interruption_type
#define TIMER0_INTR_T 1
#define TIMER1_INTR_T 2
#define TIMER2_INTR_T 3
#define TIMER3_INTR_T 4
#define UART0_INTR_T 5
#define UART1_INTR_T 6
#define UART2_INTR_T 7
#define UART3_INTR_T 8
#define SPI_INTR_T 13
#define RTC_INTR_T 17
#define EXT0_INTR_T 18
#define EXT1_INTR_T 19
#define EXT2_INTR_T 20
#define EXT3_INTR_T 21
#define GPIO_INTR_T EXT3_INTR_T
//interruption_type_end

//RBR_Trigger
#define ONE_BYTE_TRIGGER 0x0
#define FOUR_BYTE_TRIGGER 0x1
#define EIGHT_BYTE_TRIGGER 0x2
#define FOURTEEN_BYTE_TRIGGER 0x3
//RBR_trigger_end

//UART_interruption_type
#define UART_RLS_INTR_T 0x6
#define UART_RDA_INTR_T 0x4
#define UART_CTI_INTR_T 0xC
#define UART_THRE_INTR_T 0x2
//UART_interruption_type_end

//SPI_interruption
#define SPI_MODF_INTR_T 0x10
#define SPI_SPIF_INTR_T 0x80
//SPI_interruption_end

//SPI
//SPI_mode
#define BIT_EN 0x04
#define FIRST_EDGE 0x00
#define SECOND_EDGE 0x08
#define ACTIVE_HIGH 0x00
#define ACTIVE_LOW 0x10
#define SPI_SLAVE 0x00
#define SPI_MASTER 0x20
#define MSB_FO 0x00
#define LSB_FO 0x40
#define SPIINTR_EN 0x80
//SPI_mode_end
#define SPI_VAILD 0x0100
#define DEFAULT_SPI_MODE BIT_EN|FIRST_EDGE|ACTIVE_LOW|MSB_FO

//SPI_state
#define SPImode (*(reg32 *)SPI_CTRL & SPI_MASTER)
#define SPIswapEnd (*(reg32 *)SPI_STAT & 0x80)
#define SPIwcol (*(reg32 *)SPI_STAT & 0x40)
#define SPIrovr (*(reg32 *)SPI_STAT & 0x20)
#define SPImodf (*(reg32 *)SPI_STAT & 0x10)
#define SPIabrt (*(reg32 *)SPI_STAT & 0x04)
#define SPIdata (*(reg32 *)SPI_DATA)
#define SPIvaild (*(reg32 *)SPI_DATA & SPI_VAILD?HIGH:LOW)
//SPI_state_end

//interruption_rename
#define extIntr0Serve EINT0_IRQHandler
#define extIntr1Serve EINT1_IRQHandler
#define extIntr2Serve EINT2_IRQHandler
#define extIntr3Serve EINT3_IRQHandler
//interruption_rename_end

//typedef
typedef unsigned long reg32;//4byte

typedef struct{
	byte sec,min,hour,dow,dom,month;
	unsigned short year,doy;
}Time;

typedef struct Timer{
	byte timerType;//定时器编号
	byte mode;//定时器模式或计数器模式
	byte cntPin;//计数器模式下的外部时钟引脚，capitalPin0和capitalPin1
	byte regUsage;//match和capital寄存器资源占用情况
	void(*intrFuncList[6])(void);//中断函数列表
	//bit0:MATCH0,bit1:MATCH1,bit2:MATCH2,bit3:MATCH3,bit4:CAPITAL0,bit5:CAPITAL1
}Timer;

typedef struct{
	byte UARTtype;
	byte recTop;
	flag vaild;
	SqQueue *buffer;

	void (*RLSintrFunc)(void);
	void (*RDAintrFunc)(void);
	void (*CTIintrFunc)(void);
	void (*THREintrFunc)(void);
}UART;

typedef struct{
	flag vaild;
	void (*SPIFintrFunc)(void);
	void (*MODFintrFunc)(void);
}SPI;
//typedef_end

//global_value
extern const reg32 GP_REG[5];
extern reg32 oneSecClk;
extern reg32 oneMsClk;
extern reg32 oneUsClk;
//global_value_end


//定时器默认为定时器模式，PR为0，所有寄存器资源可用，无中断服务函数
extern Timer timer0Obj; extern Timer*timer0;
extern Timer timer1Obj; extern Timer*timer1;
extern Timer timer2Obj; extern Timer*timer2;
extern Timer timer3Obj; extern Timer*timer3;

extern UART uart0Obj; extern UART *uart0;
extern UART uart1Obj; extern UART *uart1;
extern UART uart1ObjH; extern UART *uart1H;
extern UART uart2Obj; extern UART *uart2;
extern UART uart2ObjH; extern UART *uart2H;
extern UART uart3Obj; extern UART *uart3;

extern SPI spiObj;extern SPI *spi;
extern const reg32 timerRegOffset[4];
extern const reg32 UARTregOffset[4];
//global_value_end



//power
#define peripheralPowerOn(peripheralPowerType) ( *(reg32 *)PERIPHERAL_POWER |= 0x1UL<<peripheralPowerType )
#define peripheralPowerOff(peripheralPowerType) ( *(reg32 *)PERIPHERAL_POWER &= ~(0x1UL<<peripheralPowerType) )
//power_end

//GPIO
//写port端口的pin引脚，val为HIGH或LOW
#define	wrBit(port,pin,val) ((val)?\
	(*(reg32 *)(FIOSET_HEAD+((port)<<5)) = 0x1UL<<(pin)):\
	(*(reg32 *)(FIOCLR_HEAD+((port)<<5)) = 0x1UL<<(pin)))
//写port端口以beginPin开始的一字节长的引脚
#define wrByte(port,beginPin,b) wrBits(port,beginPin,b,8)
//写port端口以beginPin开始的一节长的引脚
#define wrWord(port,beginPin,w) wrBits(port,beginPin,w,sizeof(reg32))
//读取port端口的pin引脚，返回值为HIGH或LOW
#define rdBit(port,pin) (*(reg32 *)(FIOPIN_HEAD+((port)<<5))&0x1UL<<(pin)?HIGH:LOW)
//读取port端口以beginPin开始的长度为len的数据，读取的数据低位在连续引脚序列的低位
#define rdBits(port,beginPin,len) (*(reg32 *)(FIOPIN_HEAD+((port)<<5))>>(beginPin)&~(~0x0<<(len)))
//读取port端口以beginPin开始的一字节长的数据
#define rdByte(port,beginPin) rdBits(port,beginPin,8)
//读取port端口以beginPin开始的一字长的数据
#define rdWord(port,beginPin) rdBits(port,beginPin,sizeof(reg32))
#define setMask(port,mask) (*(reg32 *)(FIOMASK_HEAD+(port)<<5) = (mask))
//设置port端口的pin引脚的功能，sel等于SEL_GPIO时，引脚为GPIO功能
void setSel(byte port,byte pin,byte sel);
//设置port端口以beginPin开始往后len个引脚的功能
void setSels(byte port,byte beginPin,byte sel,unsigned short len);
//设置port端口的pin引脚的模式，模式有上拉PULL_UP、下拉PULL_DOWN、中继REPEATOR、无NO_PULL
void setMode(byte port,byte pin,byte mode);
//设置port端口以beginPin开始往后len个引脚的模式
void setModes(byte port,byte beginPin,byte mode,unsigned short len);
//设置port端口的pin引脚的是否开漏
void setModeOD(byte port,byte pin,byte mode);
//设置port端口以beginPin开始往后len个引脚是否开漏
void setModeODs(byte port,byte beginPin,byte mode,unsigned short len);
//设置port端口的pin引脚的方向
void setDir(byte port,byte pin,byte dir);
//设置port端口以beginPin开始往后len个引脚的方向
void setDirs(byte port,byte beginPin,byte dir,unsigned short len);
//初始化port端口pin引脚为GPIO，模式可选输入INPUT、保持输入HOLD_INPUT、推挽输出STRONG_OUTPUT、开漏输出WEAK_OUTPUT
flag setGPIO(byte port,byte pin,byte mode);
//初始化port端口以beginPin开始往后len个引脚为GPIO
flag setGPIOs(byte port,byte beginPin,byte mode,unsigned short len);
//写port端口以beginPin开始的长度为len的引脚，word的低位写在连续引脚序列的低位
void wrBits(byte port,byte beginPin,reg32 word,unsigned short len);
//GPIO_end


//external_interruption
#define clrExtIntr(extIntrType) (*(reg32 *)EXT_INTR_REQ = 0x1UL<<(extIntrType)&0xF)
#define setExtIntr0(mode,priority) setExtIntr(0,mode,priority)
#define setExtIntr1(mode,priority) setExtIntr(1,mode,priority)
#define setExtIntr2(mode,priority) setExtIntr(2,mode,priority)
#define setExtIntr3(mode,priority) setExtIntr(3,mode,priority)
//priority取值0~31,EXTINTR0在p2.10,EXTINTR1在p2.11,EXTINTR2在p2.12,EXTINTR3在p2.13,因此port默认为2
//mode可选LOW,HIGH,RISING_EDGE,FALLING_EDGE
void setExtIntr(byte pin,byte mode,byte priority);
//external_interruption_end

//GPIO_interruption
#define checkIOintrs(port) ( *(reg32 *)IO_INTR_REQ&((port)?0x4:0x1)?HIGH:LOW)
#define checkIOintr(port,pin,mode) ( *(reg32 *)( (port)?\
((mode==RISING_EDGE)?IO2_INTR_REQ_RISING:IO2_INTR_REQ_FALLING):\
((mode==RISING_EDGE)?IO0_INTR_REQ_RISING:IO0_INTR_REQ_FALLING))\
&0x1UL<<(pin)?HIGH:LOW )//mode可选RISING_EDGE或者FALLING_EDGE
#define clrIOIntr(port,pin) ( *(reg32 *)((port)?IO2_INTR_REQ_CLR:IO0_INTR_REQ_CLR) |= 0x1UL<<(pin) )
#define setGPIOIntrPriority(priority) setIntrPriority(EXT3_INTR_T,priority)
//GPIO_interruption_end

//interruption
#define checkIntrEnable(intrType) ( *(reg32 *)((intrType)&0x20?INTR_DISEN1:INTR_DISEN0) & 0x1UL<<((intrType)&0x1F) )
//使能中断，intrType为中断源类型号
#define enableIntr(intrType) ( *(reg32 *)((intrType)&0x20?INTR_EN1:INTR_EN0) |= 0x1UL<<((intrType)&0x1F) )
//禁止使能中断，intrType为中断源类型号
#define disableIntr(intrType) ( *(reg32 *)((intrType)&0x20?INTR_DISEN1:INTR_DISEN0) |= 0x1UL<<((intrType)&0x1F) )
//设置中断优先级，priority取值0~31，数值越低优先级越高
void setIntrPriority(byte intrType,byte priority);
//interruption_end



//Timer
//延迟ms毫秒
#define delay(ms) t1Delay(ms,oneMsClk) 
//延迟us微秒
#define udelay(us) t1Delay(us,oneUsClk)
//在timerPtr指定的定时器环境下，毫秒向匹配值的转换
#define setTCcyc(timer,scale) wrPR(timer,scale-1) 
//读取timerPtr指向的定时器的外部匹配引脚的值(4bit)

#define rdExtMatchPin(timer) ( *(reg32 *)(TIMER_EXT_MATCH_PIN_CTRL_HEAD+timerRegOffset[(timer)->timerType])&0x0F )
//读取timerPtr指向的定时器的匹配寄存器matchReg的值
#define rdMatch(timer,matchReg) ( *(reg32 *)(timerMatchReg[matchReg]+timerRegOffset[(timer)->timerType]) )
//读取timerPtr指向的定时器的捕获寄存器capitalReg的值
#define rdCapital(timer,capitalReg) ( *(reg32 *)(timerMatchReg[capitalReg]+timerRegOffset[(timer)->timerType]) )
//读取timerPtr指向的定时器的TC
#define rdTC(timer) ( *(reg32 *)(TIMER_TC_HEAD+timerRegOffset[(timer)->timerType&0x3]) )
//读取timerPtr指向的定时器的PR
#define rdPR(timer)	( *(reg32 *)(TIMER_PR_HEAD+timerRegOffset[(timer)->timerType&0x3]) )
//读取timerPtr指向的定时器的PC
#define rdPC(timer)	( *(reg32 *)(TIMER_PC_HEAD+timerRegOffset[(timer)->timerType&0x3]) )

//写val到timerPtr指向的定时器的matchReg匹配寄存器
#define wrMatch(timer,matchReg,val) (*(reg32 *)(timerMatchReg[matchReg]+timerRegOffset[(timer)->timerType]) = (val))
//写val到timerPtr指向的定时器的TC
#define wrTC(timer,val) ( *(reg32 *)(TIMER_TC_HEAD+timerRegOffset[(timer)->timerType&0x3]) = (val) )
//写val到timerPtr指向的定时器的PR
#define wrPR(timer,val) ( *(reg32 *)(TIMER_PR_HEAD+timerRegOffset[(timer)->timerType&0x3]) = (val) )
//写val到timerPtr指向的定时器的PC
#define wrPC(timer,val) ( *(reg32 *)(TIMER_PC_HEAD+timerRegOffset[(timer)->timerType&0x3]) = (val) )

#define rdTimerIntr(timer) (*(reg32 *)(TIMER_INTR_REQ_HEAD+timerRegOffset[(timer)->timerType&0x3]))
//清除timerPtr指向的定时器的capitalReg的中断
#define clrTimerCapitalIntr(timer,capitalReg) clrTimerIntr(timer,capitalReg+4)
//清除timerPtr指向的定时器的matchReg的中断
#define clrTimerMatchIntr(timer,matchReg) clrTimerIntr(timer,matchReg)
#define clrTimerIntr(timer,index) (*(reg32*)(TIMER_INTR_REQ_HEAD+timerRegOffset[(timer)->timerType&0x3])|= 0x1UL<<(index)&0x3F )
//清除timerPtr指向的定时器的所有中断
#define clrTimerIntrs(timer) ( *(reg32 *)(TIMER_INTR_REQ_HEAD+timerRegOffset[(timer)->timerType&0x3]) = 0x3F )

//设置微秒定时器
#define setUsTimer(timer) setTimerMode(timer,TIMER_MODE,oneUsClk,0)
//设置毫秒定时器
#define setMsTimer(timer) setTimerMode(timer,TIMER_MODE,oneMsClk,0)
//设置定时器为定时器模式，分频为PR
#define setTimer(timer,scale) setTimerMode(timer,TIMER_MODE,scale,0)
//设置定时器为计数模式，计数引脚为cntPin，计数方式为cntMode
#define setCounter(timer,cntMode,cntPin) setTimerMode(timer,cntMode,1,cntPin)

//开启定时器
#define timerOn(timer) ( *(reg32*)(TIMER_CTRL_HEAD+timerRegOffset[(timer)->timerType&0x3])=0x1 )
//关闭定时器
#define timerOff(timer) ( *(reg32*)(TIMER_CTRL_HEAD+timerRegOffset[(timer)->timerType&0x3])=0x0 )
//复位并关闭定时器
//计数器复位置1，使得PC和PR在下一个pclk上升沿复位,并在该位为0之前保持复位
#define timerRst(timer) ( *(reg32*)(TIMER_CTRL_HEAD+timerRegOffset[(timer)->timerType&0x3])=0x2 )

//选择timerPtr指向的定时器，设置每计数达到val中断一次，中断服务函数为func
//当定时器为定时器模式时，val指定一个时间，这个时间的单位由定时器决定，定时器为毫秒（微秒）定时器，则val时间单位为毫秒（微秒）
//当定时器为计数器模式时，val指定一个计数值
//定时器1被用作delay，不建议选择定时器1作为参数
#define setCycIntr(timer,val,func) attachMatch(timer,0,val,TIMER_RST_ACTION,func)

//取消timerPtr指向的定时器中matchReg的匹配
flag cancelMatch(Timer *timer,byte matchReg);
//取消timerPtr指向的定时器中capitalReg的捕获
flag cancelCapital(Timer *timer,byte capitalReg);
//取消timerPtr指向的定时器中matchReg的外部匹配引脚控制
flag cancelExtMatchPin(Timer *timer,byte matchReg);
//设置timerPtr指向的定时器为定时器模式还是计数器模式，并设置定时模式下的分频PR或计数模式下计数的引脚cntPin
//mode可选TIMER_MODE,RISING_EDGE,FALLING_EDGE,BOTH_EDGE(等效于RISING_EDGE|FALLING_EDGE)
flag setTimerMode(Timer *timer,byte mode,reg32 scale,byte cntPin);
//设置timerPtr指向的定时器当匹配到matchReg里的值val，执行动作action，如果func不为NULL，则执行中断，func为对应的中断服务函数
//action可选TIMER_RST_ACTION,TIMER_OFF_ACTION
flag attachMatch(Timer *timer,byte matchReg,reg32 val,byte action,void(*func)(void));
//设置timerPtr指向的定时器当捕获寄存器capitalReg对应的捕获引脚出现action指定的动作，捕获寄存器会捕获TC的值，如果func不为NULL，则执行中断，func为对应的中断服务函数
//action可选RISING_EDGE,FALLING_EDGE的组合
flag attachCapital(Timer *timer,byte capitalReg,byte action,void(*func)(void));
//设置timerPtr指向的定时器当匹配到matchReg的值,对对应的外部匹配引脚做出相应的动作action，有点matchReg会对应2个外部匹配引脚，若要选择高序的，highPin为1，若要选则低序的，highPin为0
//action可选NO_ACTION,SET_LOW_ACTION,SET_HIGH_ACTION,REV_ACTION
flag attachExtMatchPin(Timer *timer,byte matchReg,byte action,...);
//写timerPtr指向的定时器的外部匹配引脚(4bit)，写入值为val
flag wrExtMatchPins(Timer *timer,reg32 val);
//定时器1的阻塞式延时，timeCyc为延时单位，time为延时时间量
void t1Delay(reg32 time,reg32 timeCyc);
//Timer_end

//UART
#define rdUARTintr(uart) (*(reg32 *)(UART_INTR_REQ_HEAD+UARTregOffset[(uart)->UARTtype&0x3])&0xF)
#define UARTtransmitterEmpty(uart) (*(reg32 *)(LINE_STAT_HEAD+UARTregOffset[(uart)->UARTtype])&0x40)
#define cancelRLSintr(uart) (*(reg32 *)(UART_INTR_EN_HEAD+UARTregOffset[(uart)->UARTtype]) &= ~0x04)
#define cancelRBRintr(uart) (*(reg32 *)(UART_INTR_EN_HEAD+UARTregOffset[(uart)->UARTtype]) &= ~0x01)
#define cancelTHREintr(uart) (*(reg32 *)(UART_INTR_EN_HEAD+UARTregOffset[(uart)->UARTtype]) &= ~0x02)
#define UARTavailable(uart) ((uart)->buffer?getSqQueueLength((uart)->buffer):*(reg32 *)(LINE_STAT_HEAD+UARTregOffset[(uart)->UARTtype])&0x01||(uart)->vaild)
void UARTbegin(UART * uart,reg32 baudRate,SqQueue *buffer,...);
byte UARTpeekByte(UART * uart);
byte UARTrdByte(UART * uart);
void UARTwrByte(UART *uart,byte b);
void UARTwrBytes(UART *uart,const byte *stream,reg32 len);
unsigned int UARTputs(UART *uart,const char *str);
void setRLSintr(UART * uart,void (*RLSintrFunc)(void));
void setRBRintr(UART * uart,void(*RDAintrFunc)(void),void(*CTIintrFunc)(void), byte RBRTrigger);
void setTHREintr(UART * uart,void (*THREintrFunc)(void));
void RBRintrFunc(UART *uart);
//清空读缓存区
void UARTflushBuffer(UART *uart);
//UART_end

//SPI
#define wrSPIctrl(mode) ( *(reg32 *)SPI_CTRL = (mode) )
#define wrSPIdata(w) (*(reg32 *)SPI_DATA = (w))
#define SPImasterBegin(baudRate) SPIbegin(SPI_MASTER|DEFAULT_SPI_MODE,9,baudRate)
#define SPIslaveBegin() SPIbegin(SPI_SLAVE|DEFAULT_SPI_MODE,9,1)
#define cancelSPIintr() (/*disableIntr(SPI_INTR_T),*/ *(reg32 *)SPI_CTRL &= ~SPIINTR_EN)
void SPIbegin(byte mode,byte frameSize,reg32 baudRate);
void setSPIintr(void (*MODF)(void),void (*SPIF)(void));
byte SPIavailable(void);
byte SPIpeekByte(void);
byte SPIrdByte(void);
void SPIwrByte(byte b);
unsigned int SPIswapByte(unsigned int w);
//SPI_end

//RTC
#define RTCon() (*(reg32 *)RTC_CLK_CRTL|=0x1)
#define RTCoff() (*(reg32 *)RTC_CLK_CRTL&=~0x1)
#define RTCrst() (*(reg32*)RTC_CLK_CRTL=*(reg32*)RTC_CLK_CRTL&~0x3|0x2)
#define clrRTCintr(type) (*(reg32 *)RTC_INTR_REQ|=type)
void getTime(Time *t);	
//RTC_end



//frame
//GPIO中断框架
#define IOintrServeFrame \
void(*port0RisingFuncList[32])(void)={0};\
void(*port2RisingFuncList[32])(void)={0};\
void(*port0FallingFuncList[32])(void)={0};\
void(*port2FallingFuncList[32])(void)={0};\
void extIntr3Serve(){\
	byte i;\
	byte port = checkIOintrs(0)?0:2;\
	for(i=0;i<32;i++){\
		if(checkIOintr(port,i,RISING_EDGE)){\
			clrIOIntr(port,i);\
			if(port==0&&port0RisingFuncList[i])\
				port0RisingFuncList[i]();\
			else if(port==2&&port2RisingFuncList[i])\
				port2RisingFuncList[i]();\
		}\
		if(checkIOintr(port,i,FALLING_EDGE)){\
			clrIOIntr(port,i);\
			if(port==0&&port0FallingFuncList[i])\
				port0RisingFuncList[i]();\
			else if(port==2&&port2FallingFuncList[i])\
				port2RisingFuncList[i]();\
		}\
	}\
}\
void setIOintr(byte port,byte pin,byte mode,void(*func)(void)){\
	if(mode == RISING_EDGE){\
		*(reg32 *)(port?IO2_INTR_EN_RISING:IO0_INTR_EN_RISING) |= 0x1UL<<pin;\
		(port?port2RisingFuncList:port0RisingFuncList)[pin&0x1F] = func;\
	}else if(mode == FALLING_EDGE){\
		*(reg32 *)(port?IO2_INTR_EN_FALLING:IO0_INTR_EN_FALLING) |= 0x1UL<<pin;\
		(port?port2FallingFuncList:port0FallingFuncList)[pin&0x1F] = func;\
	}\
	setMode(port,pin,PULL_UP);\
	setSel(port,pin,SEL_GPIO);\
	setDir(port,pin,INPUT);\
	enableIntr(GPIO_INTR_T);\
}

//定时器计数器中断服务框架
#define timer0IntrServeFrame TimerIntrServeFrame(TIMER0_IRQHandler,timer0)
#define timer1IntrServeFrame TimerIntrServeFrame(TIMER1_IRQHandler,timer1)
#define timer2IntrServeFrame TimerIntrServeFrame(TIMER2_IRQHandler,timer2)
#define timer3IntrServeFrame TimerIntrServeFrame(TIMER3_IRQHandler,timer3)

#define TimerIntrServeFrame(intrEntry,timer) \
void intrEntry(){\
	reg32 i;\
	byte intrType = rdTimerIntr(timer);\
	clrTimerIntrs(timer);\
	for(i=0;i<6;i++){\
		if(intrType&0x1UL<<i){\
			clrTimerIntr(timer,i);\
			if((timer)->intrFuncList[i])\
				(timer)->intrFuncList[i]();\
		}\
	}\
}

//RTC中断服务框架
#define RTCintrServeFrame \
void (*RTCincIntrFunc)(void) = NULL;\
void (*RTCalarmIntrFunc)(void) = NULL;\
void setRTCincIntr(byte inc,void (*func)(void)){\
	if(func){\
		*(reg32 *)RTC_INC_INTR = inc;\
		RTCincIntrFunc = func;\
		enableIntr(RTC_INTR_T);\
	}\
}\
void setRTCalarmIntr(byte cmp,void (*func)(void)){\
	if(func){\
		*(reg32 *)RTC_ALARM_INTR = ~cmp;\
		RTCalarmIntrFunc = func;\
		enableIntr(RTC_INTR_T);\
	}\
}\
void RTC_IRQHandler(){\
	if(*(reg32 *)RTC_INTR_REQ&0x1&&RTCincIntrFunc){\
		RTCincIntrFunc();\
		clrRTCintr(RTC_INC_INTR_T);\
	}\
	if(*(reg32 *)RTC_INTR_REQ&0x2&&RTCalarmIntrFunc){\
		clrRTCintr(RTC_ALARM_INTR_T);\
		RTCalarmIntrFunc();\
	}\
}

//UART中断服务框架
#define UART0intrServeFrame UARTintrServeFrame(UART0_IRQHandler,uart0)
#define UART1intrServeFrame UARTintrServeFrame(UART1_IRQHandler,uart1)
#define UART2intrServeFrame UARTintrServeFrame(UART2_IRQHandler,uart2)
#define UART3intrServeFrame UARTintrServeFrame(UART3_IRQHandler,uart3)

#define UARTintrServeFrame(intrEntry,uart) \
void intrEntry(void){\
	byte intrType = rdUARTintr(uart);\
	wrBits(0,15,~0xE,4);\
	if(intrType == UART_RLS_INTR_T&&(uart)->RLSintrFunc){\
		(uart)->RLSintrFunc();\
	}else if(intrType == UART_RDA_INTR_T){\
		if((uart)->RDAintrFunc) (uart)->RDAintrFunc();\
		else if(uart->buffer) RBRintrFunc(uart);\
	}else if(intrType == UART_CTI_INTR_T){\
		if((uart)->CTIintrFunc) (uart)->CTIintrFunc();\
		else if(uart->buffer) RBRintrFunc(uart);\
	}else if(intrType == UART_THRE_INTR_T&&(uart)->THREintrFunc){\
		(uart)->THREintrFunc();\
	}\
}

//SPI中断服务框架
#define SPIintrServeFrame \
void SPI_IRQHandler(){\
	byte intrType = *(reg32 *)SPI_STAT;\
	*(reg32 *)SPI_INTR_REQ |= 0x1;\
	if(intrType&SPI_MODF_INTR_T&&spi->MODFintrFunc) spi->MODFintrFunc();\
	if(intrType&SPI_SPIF_INTR_T&&spi->SPIFintrFunc) spi->SPIFintrFunc();\
}
//frame_end


#endif
