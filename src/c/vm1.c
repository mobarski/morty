#include "stdio.h"

// 0-stop 1-push 2-mul 3-add 4-jz 5-dup
int mem[100]  = {0};
//int code[100] = { 1,2, 1,20, 2,0, 1,1, 1,1, 3,0, 3,0, 5,0, 0,0 };
int code[100] = { 1,400, 1,-1, 3,0, 5,0, 4,14, 1,0, 4,2, 0,0 };
int ip = 0;
int sp = 0;
int rp = 10;
int tos = 0;

#define s_pop()   tos; tos = mem[sp--]
#define s_push(x) mem[++sp] = tos, tos = x

int main() {
	for(int i=0; i<10000; i++) {
		int op = code[ip++];
		int arg = code[ip++];
		int v;
		switch(op) {
			case 0:
				ip -= 2;
				break;
			case 1:
				s_push(arg);
				break;
			case 2:
				v = s_pop();
				tos *= v;
				break;
			case 3:
				v = s_pop();
				tos += v;
				break;
			case 4:
				v = s_pop();
				if (v==0) ip=arg;
				break;
			case 5:
				s_push(tos);
				break;
		}
		//printf("T:%d  SP:%d  IP:%d  OP:%d  ARG:%d \n",tos,sp,ip,op,arg);
	}
	printf("T:%d  SP:%d  IP:%d \n",tos,sp,ip);
}
