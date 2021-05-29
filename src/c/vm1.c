// Simple switch-based token-threaded MortyVM

#include "stdio.h"

// TODO: cleanup
#include <sys/time.h>
long long ms_clock() {
    struct timeval tv; 
    gettimeofday(&tv, NULL);
    long long ms = tv.tv_sec*1000LL + tv.tv_usec/1000;
    return ms;
}

// AUTO GENERATED OPCODES
#include "ops.h"

// VM INTERNAL INSTRUCTIONS
#define s_pop()   tos; tos = mem[sp--]
#define s_push(x) mem[++sp] = tos; tos = x
#define r_push(x) mem[++rp] = x
#define r_pop()   mem[rp--]

typedef struct {
	int fp;
	int ip;
	int rp;
	int sp;
	int tos;
	int dp;
} vm_state;

// TODO: propper mem[] and code[]
int mem[1000]  = {0};
//int code[100] = { PUSH,400, PUSH,-1, ADD,0, DUP,0, JZ,14, PUSH,0, JZ,2, STOP,0 };
//int code[100] = { CLOCK,0, PUSH,40, PUSH,2, ADD,0, PUSH,42, OK,0, DROP,0, CLOCK,0, SWAP,0, SUB,0, DOT,0, VMINFO,0, STOP,0 };
//int code[100] = {PUSH,-42, PUSH,1, USHR,0, DOT,0, STOP,0};
int code[100] = { PUSH,1, ALLOT,0, DUP,0, PUSH,40, SWAP,0, SET,0, PUSH,2, SWAP,0, GET,0, ADD,0, VMINFO,0, STOP,0 };

// VM RUN
vm_state run(int* mem, vm_state state) {
	
	// VM REGISTERS
	int tos = state.tos;
	int ip  = state.ip;
	int sp  = state.sp;
	int rp  = state.rp;
	int fp  = state.fp;
	int dp  = state.dp; // for ALLOT
	
	// FINAL STATE
	vm_state final;

	// AUX VARS
	unsigned int uv=0; // for USHR
	int ts_vminfo = ms_clock();
	
	// TODO: propper main loop
	// MAIN LOOP
	for(int i=0; i<100; i++) {
		int op = code[ip++];
		int arg = code[ip++];
		int v;
		switch(op) {
			// BRANCHING
			case CALL:  r_push(fp); r_push(ip); ip=arg; fp=rp; break; // TEST ME
			case RET:   ip=mem[fp]; rp=fp-2; fp=mem[fp-1];     break; // TEST ME
			case STOP:  ip -= 2;                               break;
			case QCALL: r_push(ip); ip=s_pop();                break;
			case QRET:  ip = r_pop();                          break;
			case JZ:    v=s_pop();  if (v==0) ip=arg;          break;
			// RETURN STACK
			case STOR:  v=s_pop(); r_push(v);              break;
			case RTOS:  v=r_pop(); s_push(v);              break;
			case RADD:  rp+=arg;                           break; // local variables
			case VGET:  v=mem[fp+arg]; s_push(v);          break; // local variables
			case VSET:  v=s_pop(); mem[fp+arg]=v;          break; // local variables
			// DATA STACK
			case SWAP:  v=tos; tos=mem[sp]; mem[sp]=v; break;
			case PUSH:  s_push(arg);                   break;
			case DUP:   s_push(tos);                   break;
			case DROP:  v=s_pop();                     break;
			// ALU
			case MUL:   v=s_pop(); tos *= v;           break;
			case DIV:   v=s_pop(); tos /= v;           break;
			case ADD:   v=s_pop(); tos += v;           break;
			case SUB:   v=s_pop(); tos -= v;           break;
			case AND:   v=s_pop(); tos &= v;           break;
			case OR:    v=s_pop(); tos |= v;           break;
			case XOR:   v=s_pop(); tos ^= v;           break;
			case MOD:   v=s_pop(); tos %= v;           break; // NOT CORE
			case SHL:   v=s_pop(); tos <<= v;          break; // NOT CORE
			case SHR:   v=s_pop(); tos >>= v;          break; // NOT CORE
			case USHR:  v=s_pop(); uv=tos; tos=uv>>v;  break; // NOT CORE
			case LT:    v=s_pop(); tos= tos<v  ? 1:0;  break; // NOT CORE
			case LE:    v=s_pop(); tos= tos<=v ? 1:0;  break; // NOT CORE
			case GT:    v=s_pop(); tos= tos>v  ? 1:0;  break; // NOT CORE
			case GE:    v=s_pop(); tos= tos>=v ? 1:0;  break; // NOT CORE
			case EQ:    v=s_pop(); tos= tos==v ? 1:0;  break; // NOT CORE
			case NE:    v=s_pop(); tos= tos!=v ? 1:0;  break; // NOT CORE
			case MIN:   v=s_pop(); tos= tos<v ? tos:v; break; // NOT CORE
			case MAX:   v=s_pop(); tos= tos>v ? tos:v; break; // NOT CORE
			case NZ:    tos = tos!=0 ? 1:0;            break;
			// MEMORY
			case GET:   tos = mem[tos];                break;
			case SET:   v=s_pop(); mem[v]=s_pop();     break;
			case ALLOT: v=s_pop(); s_push(dp); dp+=v;  break;
			// DEBUG
			case VMINFO:
				printf("T:%d  SP:%d  RP:%d  FP:%d  IP:%d  DP:%d  dt:%d ms \n",tos,sp,rp,fp,ip,dp,ms_clock()-ts_vminfo);
				ts_vminfo = ms_clock();
				break;
		}
		//printf("T:%d  SP:%d  RP:%d  FP:%d  IP:%d  DP:%d  dt:%d ms \n",tos,sp,rp,fp,ip,dp,ms_clock()-ts_vminfo); // XXX debug
	}
	
	// RETURN FINAL VM STATE
	final.tos = tos;
	final.ip  = ip;
	final.sp  = sp;
	final.rp  = rp;
	final.fp  = fp;
	final.dp  = dp;
	return final;
}

// TODO: take vm_config as input
// VM BOOT
vm_state boot(int* mem, int* code, int code_len) {
	vm_state state;
	
	state.tos = 0;
	state.ip  = 0;
	
	for (int i=0; i<code_len; i++) {
		mem[state.ip+i] = code[i];
		// printf("mem[%02d] -> %d\n", state.ip+i, code[i]); // XXX debug
	}
	
	state.sp  = state.ip + code_len + 10; // 10 -> GAP
	state.rp  = state.sp + 20; // 20 -> S-SIZE
	state.dp  = state.rp + 40; // 40 -> R-SIZE
	state.fp  = state.rp;
	
	return state;
}

// TODO: boot_from_file()

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

int main() {
	vm_state initial;
	vm_state final;
	
	initial = boot(mem, code, 40);
	final   = run(mem, initial);
}
