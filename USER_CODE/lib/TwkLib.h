#ifndef TWK_LIB_H
#define TWK_LIB_H 1

#ifndef NULL
#define NULL 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

//typedef
#ifndef TWK_FLAG
#define TWK_FLAG 1
typedef signed char flag;
#endif
#ifndef BYTE
#define BYTE 1
typedef unsigned char byte;
#endif
//typedef_end

//global_value
extern const char numeralList[16];
//global_value_end

//math
#define twkAbs(a) ((a)<0?-(a):(a))

//byte
#define bitsByteLen(len) ((len)?((len)-1>>3)+1:0)
#define bitsFullByteLen(len) ((len)>>3)
#define bitsRemainInByte(len) ((len)&0x7)
#define bitsByteWidthInP(headBit,len) ((len)?((headBit)+(len)-1>>3)-((headBit)>>3)+1:0)
byte revByte(byte b);
void wrBitsToBits(byte *parent,byte *child,int headBit,int endBit);

//compare
flag compareLong(long *a,long *b);
flag compareUlong(unsigned long *a,unsigned long *b);
flag compareFloat(float *a,float *b);
flag compareDouble(double *a,double *b);
flag comparePtr(void *a,void *b);
flag strCmp(const char *str0,const char *str1,unsigned int len);



//char
#define isBlankChar(ch) ((ch)==' '||(ch)=='\n'||(ch)=='\t')
#define isBinNumral(ch) ((ch)=='0'||(ch)=='1')
#define isOctNumeral(ch) ('0'<=(ch)&&(ch)<='7')
#define isHexNumeral(ch) ('0'<=(ch)&&(ch)<='9'||'A'<=(ch)&&(ch)<='F')
#define isDecNumeral(ch) ('0'<=(ch)&&(ch)<='9')
#define getNumeralVal(ch) ('0'<=(ch)&&(ch)<='9'?(ch)-'0':(ch)-'A'+10)
char toLower(char ch);
char toUpper(char ch);

#endif
