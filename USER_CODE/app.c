#include "TwkOS.h"
#include "TwkIO.h"

StackWord_t t0_stack[STACK_SIZE];
StackWord_t t1_stack[STACK_SIZE];
StackWord_t t2_stack[STACK_SIZE];
StackWord_t t3_stack[STACK_SIZE];

TCB_t t0;
TCB_t t1;
TCB_t t2;
TCB_t t3;

void task0(void *arg);
void task1(void *arg);
void task2(void *arg);
void task3(void *arg);

int f0;
int f1;
int f2;
int f3;

int main(void){
	TwkOS_init();
	TCB_dftInit(t0,task0,NULL,0);
	//TCB_dftInit(t1,task1,NULL,0);
	TCB_init(&t1,t1_stack,sizeof(t1_stack),task1,NULL,0,2);
	TCB_dftInit(t2,task2,NULL,0);
	TCB_dftInit(t3,task3,NULL,2);
	TwkOS_createTask(&t0);
	TwkOS_createTask(&t1);
	TwkOS_createTask(&t2);
	TwkOS_createTask(&t3);
	TwkOS_start();
	while(1);
}

void smpDelay(int t){
	while(t--);
}

void task0(void *arg){
	while(1){
		f0 = 1;
		//smpDelay(2048);
		f0 = 0;
		//smpDelay(2048);
	}
}

void task1(void *arg){
	while(1){
		f1 = 1;
		//smpDelay(2048);
		f1 = 0;
		//smpDelay(2048);
	}	
}

void task2(void *arg){
	while(1){
		f2 = 1;
		//smpDelay(2048);
		f2 = 0;
		//smpDelay(2048);
	}	
}

void task3(void *arg){
	while(1){
		f3 = 1;
		//smpDelay(2048);
		f3 = 0;
		//smpDelay(2048);
	}	
}
