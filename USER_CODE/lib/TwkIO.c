#include "TwkIO.h"

static flag UARTavailableShell(UART *uart){
	return UARTavailable(uart);
}

ByteStream UARTbyteStream = {NULL,
(flag(*)(void *))UARTavailableShell,
(byte(*)(void *))UARTpeekByte,
(byte(*)(void *))UARTrdByte,
(void(*)(void *,byte))UARTwrByte};

unsigned int byteStreamGets(ByteStream *stream,char *str,byte mode){
	unsigned int charNum = 0;
	char ch;
	while(byteStreamAvailable(stream));
	ch = byteStreamPeekByte(stream);
	while( (!isBlankChar(ch)||mode) && ch){
		*str++ = byteStreamRdByte(stream);
		charNum++;
		while(byteStreamAvailable(stream));
		ch = byteStreamPeekByte(stream);
	}
	return charNum;
}

unsigned int byteStreamPuts(ByteStream *stream,const char *str){
	unsigned int charNum = 0;
	while(*str){
		byteStreamWrByte(stream,*str++);
		charNum++;
	}
	return charNum;
}

void byteStreamWrObj_type(ByteStream *stream,const void *obj,unsigned int typeSize){
	unsigned int i;
	const byte *ptr = (const byte *)obj;
	for(i = 0;i<typeSize;i++) byteStreamWrByte(stream,ptr[i]);
}

void byteStreamRdObj_type(ByteStream *stream,void *obj,unsigned int typeSize){
	unsigned int i;
	byte *ptr = (byte *)obj;
	for(i = 0;i<typeSize;i++) ptr[i] = byteStreamRdByte(stream);
}

/*
把整型integer按照格式转化为字符串，
integer为要转化的整数，signBit是指示integer的符号位的位置，如果signBit为0则指定integer为无符号数，base为目标进制，
numericalMinLen是数字字符的最少个数，如果原数字字符少于numeralMinLen,则用前导0来填充至numeralMinLen，
tabMinLen是格式化字符串的最小总长度，如果原数字总长度少于tabMinLen,则用空格来填充至tabMinLen，
flushRight为真，则格式化字符串右对齐，否则，格式化字符串左对齐，
dstStr用于保存格式化字符串的地址
函数返回转化字符的个数
*/
unsigned int integerToStr(reg32 integer,byte signBit,byte base,byte numeralMinLen,byte tabMinLen,flag flushRight,char*dstStr,const char *numeral,char postfix){
	byte i=0,j=0;
	char numBuffer[20];
	unsigned int charNum=0,numeralNum=0;

	//计划
	if( postfix ) charNum++;//如果带后缀字符，字符数加一
	if( signBit && integer&0x1<<signBit ) {
		charNum++;
		integer = ~integer+1;
	}else signBit = 0;//如果不是符号数或者非负数，signBit置为0，从此以后signBit的含义由是否为符号数转变为是否为正数
	while(integer!=0){
		charNum++;
		numeralNum++;
		numBuffer[i++] = numeral[integer%base];
		integer/=base;	
	}
	if(i == 0){
		charNum++;
		numeralNum++;
	}
	if(numeralMinLen>numeralNum) charNum+=numeralMinLen-numeralNum;
	if(flushRight) 
		while( tabMinLen > charNum) {
			charNum++;
			dstStr[j++] = ' ';
		}
	//执行
	while(numeralMinLen > numeralNum){
		numeralNum++;
		dstStr[j++] = numeral[0];
	}
	
	if(i == 0){
		dstStr[j++] = numeral[0];
	}else{
		if( signBit ) dstStr[j++] = '-';
		while(i) dstStr[j++] = numBuffer[--i];		
	}
	if(postfix) dstStr[j++] = postfix;
	
	if(!flushRight) 
		while( tabMinLen > charNum){
			charNum++;
			dstStr[j++] = ' ';
		}
		
	dstStr[j] = '\0';
	return charNum;
}

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
unsigned int floatToStr(double f,byte decimalLen,byte tabMinLen,byte tabMaxLen,flag flushRight,char*dstStr){
	byte i=0,j=0;
	flag neg = FALSE;
	flag inf = FALSE;
	char numBuffer[40];
	unsigned long integer;
	double decimal;
	unsigned int charNum=0,numeralNum=0;
	if( tabMaxLen < tabMinLen) tabMinLen = tabMaxLen;
	
	//计划
	//获取f的绝对值
	if( f<0 ) {
		f = -f;
		neg = TRUE;
		charNum++;
	}
	
	if(f>~0UL) {
		inf = TRUE;
		charNum = 3+(neg?1:0);
	}else{
		//获得整数部分和小数部分
		integer = f;
		decimal = f - integer;
		
		//把计算整数部分的字符串，同时计数总字符数和数字字符数，若整数部分为0，则循环次数为0
		while(integer!=0){
			charNum++;//计数总字符数
			numeralNum++;//计数数字字符数
			if(charNum > tabMaxLen) return 0;
			numBuffer[i++] = numeralList[integer%10];
			integer/=10;	
		}
		if(i == 0){//如果整数部分为0，上文的循环执行次数为0，但是要强制输出一个0作为整数数字字符
			charNum++;
			numeralNum++;//计数数字字符数
		} 
		
		//计算可用于小数字符的空间,如果tabMaxLen-charNum<=1,则无法存储小数点字符和小数字符
		if((signed char)(tabMaxLen-charNum)<=1){
			decimalLen = 0;
		}else if(decimalLen){//若可存储小数点字符和小数字符，且小数位数设定不为0位，计数小数点字符和小数字符
			charNum++;//计数小数点字符
			//计算是否能存储decimalLen个小数字符，如果不能，则截取一部分小数字符
			if(tabMaxLen-charNum < decimalLen) decimalLen = tabMaxLen-charNum;
			charNum+=decimalLen;//计数总字符数
			numeralNum+=decimalLen;//计数数字字符数
		}
		
		if(decimalLen == 0 && i == 0 && neg) {
			charNum--;//不能表示小数部分且整数部分为0且浮点数为0，则去掉号负号表示  
			neg = FALSE;
		}
		//若charNum > tabMaxLen，表示无法存储整数部分，浮点数转化字符串失败，函数返回
		if(charNum > tabMaxLen) return 0;
	}

	//执行
	if(flushRight) 
		while( tabMinLen > charNum) {
			charNum++;
			dstStr[j++] = ' ';
		}
	if(inf){
		if(neg) dstStr[j++] = '-';
		dstStr[j++] = 'I';
		dstStr[j++] = 'N';
		dstStr[j++] = 'F';
	}else{
		//整数部分写入目标字符串
		if(i == 0 ){//如果整数部分为0，上文的循环执行次数为0，但是要强制输出一个0作为整数数字字符
			if(neg) dstStr[j++] = '-';
			dstStr[j++] = numeralList[0];
		}else{//如果整数部分为非0，上文的循环执行次数大于0，从numBuffer取整数部分
			if( neg ) dstStr[j++] = '-';
			while(i) dstStr[j++] = numBuffer[--i];		
		}
		//小数部分写入目标字符串
		if(decimalLen){
			dstStr[j++] = '.';
			while(decimalLen--){
				decimal*=10;
				integer = decimal;
				decimal	= decimal-integer;
				dstStr[j++] = numeralList[integer];
			}
		}
	}
	if(!flushRight) 
		while( tabMinLen > charNum){
			charNum++;
			dstStr[j++] = ' ';
		}
	dstStr[j] = '\0';
	return charNum;
}

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
unsigned int byteStreamPrintf(ByteStream *stream,const char *format,byte *argPtr){
	char buffer[71];//数值类型的格式化字符串最大70
	const char *strPtr;
	flag flushRight;
	unsigned int charNum = 0;	
	while(*format){
		if(*format == '%'){
			buffer[0] = 0 ;
			buffer[1] = 4 ;//默认小数输出位数
			flushRight = FALSE;
			format++; 
			if(*format == '-'){
				flushRight = TRUE;
				format++;
			}
			if(isDecNumeral(*format)){
				buffer[0] = getNumeralVal(*format);
				format++;
				if(isDecNumeral(*format)){
					buffer[0] = buffer[0]*10+getNumeralVal(*format);
					format++;
					if(buffer[0]>70) buffer[0] = 70;
				}
			}
			if(*format=='.'){
				format++;
				if(isDecNumeral(*format)){
					buffer[1] = getNumeralVal(*format);
					format++;
					if(isDecNumeral(*format)){
						buffer[1] = buffer[1]*10+getNumeralVal(*format);
						format++;	
						if(buffer[1]>70) buffer[1] = 70;
					}
				}
			}
			switch(*format){
				case '\0': return charNum;
				case '%': 
					charNum++;
					byteStreamWrByte(stream,'%');
					break;
				case 'd': 
					charNum += integerToStr(*(signed int*)argPtr,(sizeof(signed int)<<3)-1,10,0,buffer[0],flushRight,buffer,numeralList,0);
					byteStreamPuts(stream,buffer);
					argPtr += sizeof(int);
					break;
				case 'D': 
					charNum += integerToStr(*(signed long*)argPtr,(sizeof(signed long)<<3)-1,10,0,buffer[0],flushRight,buffer,numeralList,0);
					byteStreamPuts(stream,buffer);
					argPtr += sizeof(int);
					break;
				case 'u': 
					charNum += integerToStr(*(unsigned int*)argPtr,0,10,0,buffer[0],flushRight,buffer,numeralList,0);
					byteStreamPuts(stream,buffer);
					argPtr += sizeof(int);
					break;
				case 'U': 
					charNum += integerToStr(*(unsigned long*)argPtr,0,10,0,buffer[0],flushRight,buffer,numeralList,0);
					byteStreamPuts(stream,buffer);
					argPtr += sizeof(int);
					break;
				case 'b': 
					charNum += integerToStr(*(signed int*)argPtr,(sizeof(signed int)<<3)-1,2,0,buffer[0],flushRight,buffer,numeralList,'b');
					byteStreamPuts(stream,buffer);
					argPtr += sizeof(int);
					break;	
				case 'q': 
					charNum += integerToStr(*(signed int*)argPtr,(sizeof(signed int)<<3)-1,8,0,buffer[0],flushRight,buffer,numeralList,'q');
					byteStreamPuts(stream,buffer);
					argPtr += sizeof(int);
					break;	
				case 'h': 
					charNum += integerToStr(*(signed int*)argPtr,(sizeof(signed int)<<3)-1,16,0,buffer[0],flushRight,buffer,numeralList,'h');
					byteStreamPuts(stream,buffer);
					argPtr += sizeof(int);
					break;
				case 'p': 
					charNum += integerToStr(*(unsigned long*)argPtr,0,16,sizeof(unsigned long)<<1,buffer[0],flushRight,buffer,numeralList,'h');
					byteStreamPuts(stream,buffer);
					argPtr += sizeof(int);
					break;
				case 'F':
					charNum += floatToStr(**(const double **)argPtr,buffer[1],buffer[0],70,flushRight,buffer);
					byteStreamPuts(stream,buffer);
					argPtr += sizeof(int);
					break;
				case 'f':
					charNum += floatToStr(**(const float **)argPtr,buffer[1],buffer[0],70,flushRight,buffer);
					byteStreamPuts(stream,buffer);
					argPtr += sizeof(int);
					break;
				case 'c':
					charNum++;
					byteStreamWrByte(stream, *(char*)argPtr );
					argPtr += sizeof(int);
					break;
				case 's':
					strPtr = *(const char**)argPtr;
					if(buffer[0])
						while( *strPtr && buffer[0]--){
							charNum++;
							byteStreamWrByte(stream,*strPtr++);
						} 
					else
						while( *strPtr ){
							charNum++;
							byteStreamWrByte(stream,*strPtr++);
						} 
					argPtr += sizeof(int);
					break;
				default: 
					charNum++;
					byteStreamWrByte(stream,*format);
					break;
			}
		}else{
			charNum++;
			byteStreamWrByte(stream,*format);
		}
		format++;
	}
	return charNum;
}


unsigned long strToInteger(const char *srcStr,byte base){
	unsigned long integer = 0;
	flag neg = FALSE;
	
	if(*srcStr=='-'){
		neg = TRUE;
		srcStr++;
	}

	while(*srcStr){
		integer = integer*base+getNumeralVal(*srcStr);
		srcStr++;
	}
	
	if(neg) integer = ~integer+1;
	return integer;
}

double strToFloat(const char *srcStr){
	double f = 0;
	double decimalWeight = 1;
	flag neg = FALSE;
	
	if(*srcStr=='-'){
		neg = TRUE;
		srcStr++;
		if(!*srcStr) return -f;
	}

	while(isDecNumeral(*srcStr)){
		f = f*10+getNumeralVal(*srcStr);
		srcStr++;
		if(!*srcStr){
			if(neg) f = -f;
			return f;
		}
	}

	if(*srcStr == '.'){
		srcStr++;
		if(!*srcStr){
			if(neg) f = -f;
			return f;
		}
	}else {
		if(neg) f = -f;
		return f;
	}

	while(isDecNumeral(*srcStr)){		
		decimalWeight/=10;
		f += decimalWeight*getNumeralVal(*srcStr);
		srcStr++;
		if(!*srcStr){
			if(neg) f = -f;
			return f;
		}
	}
	if(neg) f = -f;
	return f;
}

flag getIntegerStrFromByteStream(ByteStream *stream,byte base,byte numeralMaxLen,byte strMaxLen,char *dstStr){
	char ch;
	if(numeralMaxLen==0||strMaxLen==0||base>16||base==0) return -2;

	while(!byteStreamAvailable(stream));
	while(isBlankChar(byteStreamPeekByte(stream))){
		byteStreamRdByte(stream);
		while(!byteStreamAvailable(stream));
	}

	ch = byteStreamPeekByte(stream);
	if(ch == '-'){
		*dstStr++ = byteStreamRdByte(stream);
		strMaxLen--;
		if(!strMaxLen) {
			*dstStr = '\0';
			return 0;
		} 
		while(!byteStreamAvailable(stream));
		ch = byteStreamPeekByte(stream);
	}

	while('0' <= ch && ch <= base-1+'0' && base <=10 
			|| ('0' <= ch && ch <= '9' || 'A' <= ch && ch <= base-11+'A') && base >10){
		*dstStr++ = byteStreamRdByte(stream);
		strMaxLen--;
		numeralMaxLen--;
		if(!(numeralMaxLen&&strMaxLen)) {
			*dstStr = '\0';
			return 0;
		} 
		while(!byteStreamAvailable(stream));
		ch = byteStreamPeekByte(stream);
	}
	*dstStr = '\0';
	return 0;
}

flag getFloatStrFromByteStream(ByteStream *stream,byte decimalMaxLen,byte numeralMaxLen,byte strMaxLen,char *dstStr){
	char ch;
	if(numeralMaxLen == 0 || strMaxLen==0) return -2;
		
	while(!byteStreamAvailable(stream));
	while(isBlankChar(byteStreamPeekByte(stream))){
		byteStreamRdByte(stream);
		while(!byteStreamAvailable(stream));
	}

	ch = byteStreamPeekByte(stream);
	if(ch == '-'){
		*dstStr++ = byteStreamRdByte(stream);;
		strMaxLen--;
		if(!strMaxLen){
			*dstStr = '\0';
			return 0;
		} 
		while(!byteStreamAvailable(stream));
		ch = byteStreamPeekByte(stream);
	}

	while(isDecNumeral(ch)){	
		*dstStr++ = byteStreamRdByte(stream);
		strMaxLen--;
		numeralMaxLen--;
		if(!(strMaxLen&&numeralMaxLen)){
			*dstStr = '\0';
			return 0;
		} 
		while(!byteStreamAvailable(stream));
		ch = byteStreamPeekByte(stream);
	}

	if(ch == '.'){
		*dstStr++ = byteStreamRdByte(stream);
		strMaxLen--;
		if(!strMaxLen){
			*dstStr = '\0';
			return 0;
		} 
		while(!byteStreamAvailable(stream));
		ch = byteStreamPeekByte(stream);
	}else {
		*dstStr = '\0';
		return 0;
	}
	
	while(isDecNumeral(ch)){	
		*dstStr++ = byteStreamRdByte(stream);
		strMaxLen--;
		numeralMaxLen--;
		decimalMaxLen--;
		if(!(strMaxLen&&numeralMaxLen&&decimalMaxLen)){
			*dstStr = '\0';
			return 0;
		} 
		while(!byteStreamAvailable(stream));
		ch = byteStreamPeekByte(stream);
	}
	
	*dstStr = '\0';
	return 0;
}


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
unsigned int byteStreamScanf(ByteStream *stream,const char*format,byte *argPtr){
	char buffer[71];
	char *strPtr;
	flag limitLess;
	unsigned int assign = 0;
	while(*format){
		if(*format == '%'){
			buffer[0] = 0xFF;
			buffer[1] = 0xFF;//默认小数接收位数
			limitLess = FALSE;
			format++;
			if(isDecNumeral(*format)){
				buffer[0] = getNumeralVal(*format);
				format++;
				if(isDecNumeral(*format)){
					buffer[0] = buffer[0]*10+getNumeralVal(*format);
					format++;
					if(buffer[0]>70) buffer[0] = 70;
				}
			}
			if(*format=='.'){
				format++;
				if(isDecNumeral(*format)){
					buffer[1] = getNumeralVal(*format);
					format++;
					if(isDecNumeral(*format)){
						buffer[1] = buffer[1]*10+getNumeralVal(*format);
						format++;	
						if(buffer[1]>70) buffer[1] = 70;
					}
				}
			}
			switch(*format){
				case '\0': return assign;
				case '%': 
					while(!byteStreamAvailable(stream));
					if(byteStreamRdByte(stream) != '%') return assign;
				case 'd':
				case 'u':
					getIntegerStrFromByteStream(stream,10,buffer[0],70,buffer);
					**(int **)argPtr = strToInteger(buffer,10);
					assign++;
					argPtr += sizeof(int);
					break;
				case 'D':
				case 'U':
					getIntegerStrFromByteStream(stream,10,buffer[0],70,buffer);
					**(long **)argPtr = strToInteger(buffer,10);
					assign++;
					argPtr += sizeof(int);
					break;
				case 'b':
					getIntegerStrFromByteStream(stream,2,buffer[0],70,buffer);
					**(signed int **)argPtr = strToInteger(buffer,2);
					assign++;
					argPtr += sizeof(int);
					break;
				case 'q':
					getIntegerStrFromByteStream(stream,8,buffer[0],70,buffer);
					**(signed int **)argPtr = strToInteger(buffer,8);
					assign++;
					argPtr += sizeof(int);
					break;
				case 'h':
					getIntegerStrFromByteStream(stream,16,buffer[0],70,buffer);
					**(signed int **)argPtr = strToInteger(buffer,16);
					assign++;
					argPtr += sizeof(int);
					break;
				case 'p':
					getIntegerStrFromByteStream(stream,16,buffer[0],70,buffer);
					**(void ***)argPtr = (void *)strToInteger(buffer,16);
					assign++;
					argPtr += sizeof(int);
					break;
				case 'F':
					getFloatStrFromByteStream(stream,buffer[1],buffer[0],70,buffer);
					**(double **)argPtr = strToFloat(buffer);
					assign++;
					argPtr += sizeof(int);
					break;
				case 'f':
					getFloatStrFromByteStream(stream,buffer[1],buffer[0],70,buffer);
					**(float **)argPtr = strToFloat(buffer);
					assign++;
					argPtr += sizeof(int);
					break;
				case 'c':
					while(!byteStreamAvailable(stream));
					**(char **)argPtr = byteStreamRdByte(stream);
					assign++;
					argPtr += sizeof(int);
					break;
				case 's':
					strPtr = *(char **)argPtr;
					if(buffer[0] == 0) limitLess = TRUE; 

					while(!byteStreamAvailable(stream));
					while(isBlankChar(byteStreamPeekByte(stream))){
						byteStreamRdByte(stream);
						while(!byteStreamAvailable(stream));
					}

					while(1){
						if(isBlankChar(byteStreamPeekByte(stream))) break;
						*strPtr++ = byteStreamRdByte(stream);
						if(!(--buffer[0]||limitLess)) break;
						while(!byteStreamAvailable(stream));
					}
					*strPtr = '\0';
					assign++;
					argPtr += sizeof(int);
					break;
				default:
					while(!byteStreamAvailable(stream));
					if(byteStreamRdByte(stream) != *format) return assign;
					break;
			}
		}else{
			while(!byteStreamAvailable(stream));
			if(byteStreamRdByte(stream) != *format) return assign;
		}
		format++;
	}
	return assign;
}

/*
向UART格式化输出
*/
unsigned int UARTprintf(UART *uart,char *format,...){
	unsigned int charNum;
	void *tmp = UARTbyteStream.streamObj;
	UARTbyteStream.streamObj = uart;
	charNum = byteStreamPrintf(&UARTbyteStream,format,(byte*)&format+sizeof(int));
	UARTbyteStream.streamObj = tmp;
	return charNum;
}

/*
从UART格式化输入，若uart启动时没有配置软缓存，则格式化输入使用硬缓存，
硬缓存的大小只有16字节，这意味着UART连续的输入超过16字节的数据，会导致数据丢失，
若uart启动时配置了软缓存，且使用UART中断服务框架，则格式化输入使用硬缓存和软缓存，通常软缓存是大容量的，
UART连续的输入的字节数在不超过软缓存的容量，就不会导致数据丢失。
为了使格式化输入使用大容量的缓存，用户需要完成3件配置：
定义一个缓存对象、使用UART中断服务框架、在UART启动时，把缓存对象的指针加入UARTbegin函数的参数
*/
unsigned int UARTscanf(UART *uart,char *format,...){
	unsigned int assign;
	void *tmp = UARTbyteStream.streamObj;
	UARTbyteStream.streamObj = uart;
	assign = byteStreamScanf(&UARTbyteStream,format,(byte*)&format+sizeof(int));
	UARTbyteStream.streamObj = tmp;
	return assign;
}

unsigned int byteStreamPrintfShell(ByteStream *stream,char *format,...){
	return byteStreamPrintf(stream,format,(byte*)&format+sizeof(int));
}

unsigned int byteStreamScanfShell(ByteStream *stream,char *format,...){
	return byteStreamScanf(stream,format,(byte*)&format+sizeof(int));
}
