#ifndef TWK_IO_H
#define TWK_IO_H 1

#include "TwkLPC1768.h"

#define MEET_BLANK 0
#define MEET_END 1

//字节流类型，有UART字节流类型、队列字节流类型
typedef struct ByteStream{
	void *streamObj;
	flag (*availableFunc)(void *);
	byte (*peekByteFunc)(void *);
	byte (*rdByteFunc)(void *);
	void (*wrByteFunc)(void *,byte);
	void (*rewindFunc)(void *);
}ByteStream;

//global_value
extern ByteStream UARTbyteStream;
//global_value_end

#define byteStreamAvailable(stream) ((stream)->availableFunc((stream)->streamObj))
#define byteStreamPeekByte(stream) ((stream)->peekByteFunc((stream)->streamObj))
#define byteStreamRdByte(stream) ((stream)->rdByteFunc((stream)->streamObj))
#define byteStreamWrByte(stream,b) ((stream)->wrByteFunc((stream)->streamObj,b))
#define byteStreamRewind(stream) ((stream)->rewindFunc((stream)->streamObj))
#define byteStreamWrObj(stream,obj) byteStreamWrObj_type(stream,obj,sizeof(*(obj)))
#define byteStreamRdObj(stream,obj) byteStreamRdObj_type(stream,obj,sizeof(*(obj)))

//从byteStream字节流中获取一个字符串，获取的字符串存入str中，mode可选MEET_BLANK或MEET_END，返回字符串的长度（不计'\0'字符）
unsigned int byteStreamGets(ByteStream *stream,char *str,byte mode);
unsigned int byteStreamPuts(ByteStream *stream,const char *str);
void byteStreamWrObj_type(ByteStream *stream,const void *obj,unsigned int typeSize);
void byteStreamRdObj_type(ByteStream *stream,void *obj,unsigned int typeSize);


/*
把整型integer按照格式转化为字符串，
integer为要转化的整数，signBit是指示integer的符号位的位置，如果signBit为0则指定integer为无符号数，base为目标进制，
numericalMinLen是数字字符的最少个数，如果原数字字符少于numeralMinLen,则用前导0来填充至numeralMinLen，
tabMinLen是格式化字符串的最小总长度，如果原数字总长度少于tabMinLen,则用空格来填充至tabMinLen，
flushRight为真，则格式化字符串右对齐，否则，格式化字符串左对齐，
dstStr用于保存格式化字符串的地址
函数返回转化字符的个数
*/
unsigned int integerToStr(reg32 integer,byte signBit,byte base,byte numeralMinLen,byte tabMinLen,flag flushRight,char*dstStr,const char *numeral,char postfix);
/*
把浮点数f按照格式转化为十进制字符串，
f为要转化的浮点数，decimalLen是小数部分数字字符的个数，
tabMinLen是格式化字符串的最小总长度，如果原数字总长度少于tabLen,则用空格来填充至tabLen，
tabMaxLen是格式化字符串的最大总长度，由于dstStr指向的字符串容量有限，浮点数的整数部分的数字字符个数在没有计算前又是无法预知的，
在整数部分数字字符很多的时候，受字符串容量限制，可能不能输出指定个数的小数部分数字字符，以保证系统的安全性，
输入一个格式化字符串的最大总长度即字符串容量作为函数的参考，这个容量不包含字符串尾'\0'的容量，
flushRight为真，则格式化字符串右对齐，否则，格式化字符串左对齐，dstStr用于保存格式化字符串的地址，
如果出现tabMaxLen<tabMinLen，tabMinLen将被拉低为tabMaxLen，如果tabMaxLen的长度不足以表示浮点数的整数部分，则判定为无穷大，
函数返回转化字符的个数
*/
unsigned int floatToStr(double f,byte decimalLen,byte tabMinLen,byte tabMaxLen,flag flushRight,char*dstStr);

unsigned long strToInteger(const char *srcStr,byte base);
double strToFloat(const char *srcStr);


flag getIntegerStrFromByteStream(ByteStream *stream,byte base,byte numeralMaxLen,byte strMaxLen,char *dstStr);
flag getFloatStrFromByteStream(ByteStream *stream,byte decimalMaxLen,byte numeralMaxLen,byte strMaxLen,char *dstStr);


/*
向字节流格式化输出，格式化一般形式为"%[-][m][.n]lab",方括号表示可选项，
'-'表示右对齐，不写'-'表示左对齐，
在格式化输入数值型变量时，m表示制表最小长度，即该格式化字符串的最小长度，填充字符为空格，
在格式化输入字符串变量时，m表示字符串输出字符的最大个数，当m=0或者不设定m值时，对字符串输出字符的个数没有限制，
在格式化浮点数时，n表示浮点数的小数部分数字字符的输出个数，当n=0表示不输出小数部分（包括小数点），
m和n的取值不超过70，
lab可选'd'(符号十进制整型)、'u'(无符号十进制整型)、'D'(符号十进制长整型)、'U'(无符号十进制长整型)、
'h'(符号十六进制整型)、'q'(符号八进制整型)、'b'(符号二进制整型)、'p'(十六进制指针)、
'f'(十进制单精度浮点数)、'F'(十进制双精度浮点数)、'c'(字符型)、's'(字符串)，
特殊地，格式化输出变量若为浮点数，需要把浮点数的地址作为参数传入
*/
unsigned int byteStreamPrintf(ByteStream *stream,const char *format,byte *argPtr);

/*
从字节流格式化输入，格式化一般形式为"%[m][.n]lab",方括号表示可选项，
在格式化输入数值型变量时，m表示读取的数字字符的最大个数，m为0或者不设定m值时，表示对读取字符的个数最小限制，m为255，
在格式化输入字符串变量时，m表示读取字符的最大个数，m为0或者不设定m值时，表示对读取字符的个数没有限制，
在格式化浮点数时，n表示读取小数数字字符个数最多的个数，在m过小的情况下，可能会导致接收小数数字字符的个数达不到n，
m和n的取值不超过70，
lab可选'd'(符号十进制整型)、'u'(无符号十进制整型)、'D'(符号十进制长整型)、'U'(无符号十进制长整型)、
'h'(符号十六进制整型)、'q'(符号八进制整型)、'b'(符号二进制整型)、'p'(十六进制指针)、
'f'(十进制单精度浮点数)、'F'(十进制双精度浮点数)、'c'(字符型)、's'(字符串)，
数值型在获取数据前会先吸收所有在第一个非空白符之前的所有空白字符，遇到非法字符就停止取数，非法字符不会被吸收，
字符串类型会在接收所有在第一个空白符之前的所有非空白字符，遇到空白字符就停止接收，空白字符不会被吸收，
格式化输入需要把要格式化的变量的地址作为参数传入
*/
unsigned int byteStreamScanf(ByteStream *stream,const char*format,byte *argPtr);




/*
向UART格式化输出
*/
unsigned int UARTprintf(UART *uart,char *format,...);

/*
从UART格式化输入，若uart启动时没有配置软缓存，则格式化输入使用硬缓存，
硬缓存的大小只有16字节，这意味着UART连续的输入超过16字节的数据，会导致数据丢失，
若uart启动时配置了软缓存，且使用UART中断服务框架，则格式化输入使用硬缓存和软缓存，通常软缓存是大容量的，
UART连续的输入的字节数在不超过软缓存的容量，就不会导致数据丢失。
为了使格式化输入使用大容量的缓存，用户需要完成3件配置：
定义一个缓存对象、使用UART中断服务框架、在UART启动时，把缓存对象的指针加入UARTbegin函数的参数
*/
unsigned int UARTscanf(UART *uart,char *format,...);

unsigned int byteStreamPrintfShell(ByteStream *stream,char *format,...);
unsigned int byteStreamScanfShell(ByteStream *stream,char *format,...);
#endif
