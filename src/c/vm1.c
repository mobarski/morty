#include "stdio.h"

#include <sys/time.h>
long long ms_clock() {
    struct timeval tv; 
    gettimeofday(&tv, NULL);
    long long ms = tv.tv_sec*1000LL + tv.tv_usec/1000;
    return ms;
}


enum OPS {
	STOP=0,
	PUSH,
	MUL,
	ADD,
	JZ,
	DUP,
	DROP,
	SWAP,
	SUB,
	DOT=100,
	EMIT,
	OK,
	CLOCK
};

// 0-stop 1-push 2-mul 3-add 4-jz 5-dup
int mem[100]  = {0};
//int code[100] = { PUSH,400, PUSH,-1, ADD,0, DUP,0, JZ,14, PUSH,0, JZ,2, STOP,0 };
int code[100] = { CLOCK,0, PUSH,40, PUSH,2, ADD,0, PUSH,42, OK,0, DROP,0, CLOCK,0, SWAP,0, SUB,0, DOT,0, STOP,0};
//int code[100] = {PUSH,42, PUSH,1, OK,0, DOT,0, PUSH,42, EMIT,0, STOP,0};
int ip = 0;
int sp = 0;
int rp = 10;
int tos = 0;

#define s_pop()   tos; tos = mem[sp--]
#define s_push(x) mem[++sp] = tos, tos = x

int main() {
	int t_start = current_timestamp();
	for(int i=0; i<100; i++) {
		int op = code[ip++];
		int arg = code[ip++];
		int v;
		switch(op) {
			case STOP:
				ip -= 2;
				break;
			case PUSH:
				s_push(arg);
				break;
			case MUL:
				v = s_pop();
				tos *= v;
				break;
			case ADD:
				v = s_pop();
				tos += v;
				break;
			case JZ:
				v = s_pop();
				if (v==0) ip=arg;
				break;
			case DUP:
				s_push(tos);
				break;
			case SUB:
				v = s_pop();
				tos -= v;
				break;
			case SWAP:
				v = tos;
				tos = mem[sp];
				mem[sp] = v;
				break;
			case DROP:
				v = s_pop();
				break;
			// DEBUG
			case CLOCK:
				v = ms_clock() - t_start;
				s_push(v);
				break;
			case DOT:
				v = s_pop();
				printf("%d ",v);
				break;
			case EMIT:
				v = s_pop();
				printf("%c",v);
				break;
			case OK:
				v = s_pop();
				if (v==tos) {
					printf("%d ok ",v);
				} else {
					printf("\nERROR: %d != %d \n",tos,v);
					return 1;
				}
				break;
		}
		//printf("T:%d  SP:%d  IP:%d  OP:%d  ARG:%d \n",tos,sp,ip,op,arg);
	}
	printf("T:%d  SP:%d  IP:%d \n",tos,sp,ip);
}
