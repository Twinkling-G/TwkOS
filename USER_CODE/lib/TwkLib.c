#include "TwkLib.h"

const char numeralList[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

byte revByte(byte b){
	byte i,revB;
	for(i = 0,revB = 0x00;i < 8;i++,b >>= 1){
		revB <<= 1;
		revB |= b&0x01;
	}
	return revB;
}

void wrBitsToBits(byte *parent,byte *child,int headBit,int len){
	int head = headBit>>3;
	int end = headBit+len-1>>3;
	int parentWrPtr,childRdPtr;
	byte rShf,lShf,remain;
	rShf = headBit&0x7;
	lShf = 8-rShf;
	remain = (headBit+len-1&0x7)+1;

	if(head == end){
		parent[head] = parent[head]&(0xFF<<lShf|0xFF>>remain)
		|child[0]>>rShf&0xFF<<8-remain;
	}else{
		childRdPtr=1;
		for(parentWrPtr = head+1;parentWrPtr<end;parentWrPtr++){
			parent[parentWrPtr] = child[childRdPtr-1]<<lShf
			|child[childRdPtr]>>rShf;
			parentWrPtr++;
			childRdPtr++;
		}
		parent[head] = parent[head]&0xFF<<lShf|child[0]>>rShf;
		if(bitsByteWidthInP(headBit,len) == bitsByteLen(len)){
			parent[parentWrPtr]
			=child[childRdPtr-1]<<lShf
			|child[childRdPtr]>>rShf&0xFF<<8-remain
			|parent[parentWrPtr]&0xFF>>remain;
		}else{
			parent[parentWrPtr]
			=parent[parentWrPtr]&0xFF>>remain
			|child[childRdPtr-1]<<lShf&0xFF<<8-remain;
		}
	}
}


char toLower(char ch){
	return 'a'>ch && ch>'z'?0:ch-'a'+'A';
}

char toUpper(char ch) {
	return 'A'>ch && ch>'Z'?0:ch-'A'+'a';
}

flag compareLong(long *a,long *b){
	return *a>*b?1:(*a==*b?0:-1);
}

flag compareUlong(unsigned long *a,unsigned long *b){
	return *a>*b?1:(*a==*b?0:-1);
}

flag compareFloat(float *a,float *b){
	return *a>*b?1:(*a==*b?0:-1);
}

flag compareDouble(double *a,double *b){
	return *a>*b?1:(*a==*b?0:-1);
}

flag comparePtr(void *a,void *b){
	return a>b?1:(a==b?0:-1);
}


flag strCmp(const char *str0,const char *str1,unsigned int len){
	flag limitless;
	if(str0==NULL||str1==NULL) return 0;
	limitless = !len;
	while(len--||limitless){
		if(*str0 != *str1) return FALSE;
		if(!(*str0 | *str1)) return TRUE;
		str0++;
		str1++;
	}
	return TRUE;
}
