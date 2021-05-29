// Simple switch-based token-threaded MortyVM

#define CELL_SIZE 4

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
	int *mem;
} vm_state;

// VM RUN
vm_state run(vm_state state) {
	
	// VM REGISTERS
	int  tos = state.tos;
	int   ip = state.ip;
	int   sp = state.sp;
	int   rp = state.rp;
	int   fp = state.fp;
	int   dp = state.dp; // for ALLOT
	int *mem = state.mem;
	
	// FINAL STATE
	vm_state final;

	// AUX VARS
	unsigned int uv=0; // for USHR
	int ts_vminfo = ms_clock();
	int word_buf[2] = {0,0};
	
	// TODO: propper main loop
	// MAIN LOOP
	for(int i=0; i<100; i++) {
		int op  = mem[ip++];
		int arg = mem[ip++];
		int v,v2,v3;
		switch(op) {
			// BRANCHING
			case CALL:   r_push(fp); r_push(ip); ip=arg; fp=rp; break; // TEST ME
			case RET:    ip=mem[fp]; rp=fp-2; fp=mem[fp-1];     break; // TEST ME
			case STOP:   goto stop;                             break;
			case QCALL:  r_push(ip); ip=s_pop();                break;
			case QRET:   ip = r_pop();                          break;
			case JZ:     v=s_pop();  if (v==0) ip=arg;          break;
			case IF:     v=s_pop(); v2=s_pop(); if(v2) { r_push(ip); ip=v; };       break; // TEST ME
			case IFELSE: v=s_pop(); v2=s_pop(); v3=s_pop(); r_push(ip); ip=v3?v2:v; break; // TEST ME
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
			// PRIMITIVE OUTPUT
			case EMIT: v=s_pop(); printf("%c",v);  break;
			case DOT:  v=s_pop(); printf("%d ",v); break;
			case ECHO: v=s_pop(); word_buf[0]=0;
				word_buf[0]=v; // TODO: propper byte-by-byte for LITTLE/BIG endian handling
				printf("%s",(char*)(&word_buf));
				break;
		}
		//printf("T:%d  SP:%d  RP:%d  FP:%d  IP:%d  DP:%d  dt:%d ms \n",tos,sp,rp,fp,ip,dp,ms_clock()-ts_vminfo); // XXX debug
	}
	
	stop:
	// RETURN FINAL VM STATE
	final.tos = tos;
	final.ip  = ip;
	final.sp  = sp;
	final.rp  = rp;
	final.fp  = fp;
	final.dp  = dp;
	return final;
}


// ----------------------------------------------------------------------------
// ---[ BOOT ]-----------------------------------------------------------------
// ----------------------------------------------------------------------------

typedef struct {
	char *path;
	int data_stack_size;
	int return_stack_size;
	int memory_size;
	int max_code_size;
} config;

// VM BOOT
vm_state boot(int *mem, int *code, int code_len, config cfg) {
	vm_state state;
	
	state.tos = 0;
	state.ip  = 0;
	
	for (int i=0; i<code_len; i++) {
		mem[state.ip+i] = code[i];
		//printf("BOOT: mem[%02d] -> %d\n", state.ip+i, code[i]); // XXX debug
	}
	
	state.sp  = state.ip + code_len + 10; // 10 -> GAP between CODE and DATA STACK
	state.rp  = state.sp + cfg.data_stack_size;
	state.dp  = state.rp + cfg.return_stack_size;
	state.fp  = state.rp;
	state.mem = mem;
	
	return state;
}


int load_from_file(char *path, int *code, int max_len) {
	FILE *in;
	int i; // cell position in the code
	
	in = fopen(path,"r");
	if (!in) {
		fprintf(stderr, "ERROR: Cannot open input file.\n");
		return 0; // TODO: panic
	}
	
	for (i=0; i<max_len; i++) {
		int n = fread(&code[i],CELL_SIZE,1,in);
		if (n==0) {
			break;
		}
	}
	fclose(in);
	if (i>=max_len) {
		fprintf(stderr, "ERROR: Input longer than max_len (%d).\n",max_len);
		return 0;
	}
		
	return i; // code_len
}


int dump_mem(char *path, int *mem, int from, int ncells) {
	FILE *out = fopen(path,"w");
	int i;
	
	if (!out) {
		fprintf(stderr, "ERROR: Cannot open output file.\n");
		return 0; // TODO: panic
	}
	for (i=0; i<ncells; i++) {
		int n = fwrite(&mem[i], CELL_SIZE, 1, out);
		if (!n) {
			fprintf(stderr, "ERROR: Cannot dump memory (i=%d).\n", i);
			fclose(out);
			return 0; // TODO: panic
		}
	}
	fclose(out);
	
	return i; // written cells cnt
}

// ----------------------------------------------------------------------------
// ---[ CLI ]------------------------------------------------------------------
// ----------------------------------------------------------------------------

int parse_args(int argc, char *argv[], char **env, config *cfg) {
	// PRINT USAGE
	if (argc<=1) {
		usage:
		fprintf(stderr, "\nUSAGE: %s filename.mrt [-mem n] [-ds n] [-rs n] [-code n]\n", argv[0]);
		fprintf(stderr, "\n");
		fprintf(stderr, "OPTIONS:\n");
		fprintf(stderr, "  -mem N    memory cells count        (default: %d)\n", cfg->memory_size);
		fprintf(stderr, "   -ds N    data stack cells count    (default: %d)\n", cfg->data_stack_size);
		fprintf(stderr, "   -rs N    return stack cells count  (default: %d)\n", cfg->return_stack_size);
		fprintf(stderr, " -code N    max code cells count      (default: %d)\n", cfg->max_code_size);
		return 1;
	}

	// PARSE ARGUMENTS
	for (int i=1; i<argc; i++) {
		// data stack size (in cells)
		if (strcmp("-ds",argv[i])==0) {
			if (i+1>=argc) goto error_missing;
			int v = atoi(argv[++i]);
			if (!v) goto error_value;
			cfg->data_stack_size = v;
			continue;
		}
		// return stack size (in cells)
		if (strcmp("-rs",argv[i])==0) {
			if (i+1>=argc) goto error_missing;
			int v = atoi(argv[++i]);
			if (!v) goto error_value;
			cfg->return_stack_size = v;
			continue;
		}
		// memory size (in cells)
		if (strcmp("-mem",argv[i])==0) {
			if (i+1>=argc) goto error_missing;
			int v = atoi(argv[++i]);
			if (!v) goto error_value;
			cfg->memory_size = v;
			continue;
		}
		// max code size (in cells)
		if (strcmp("-code",argv[i])==0) {
			if (i+1>=argc) goto error_missing;
			int v = atoi(argv[++i]);
			if (!v) goto error_value;
			cfg->max_code_size = v;
			continue;
		}
		if (argv[i][0]=='-') {
			fprintf(stderr,"ERROR: Invalid option %s\n",argv[i]);
			goto usage;
		} else {
			cfg->path = argv[i];
		}
		continue;
		
		error_missing:
			fprintf(stderr,"ERROR: Missing value after param %s\n",argv[i]);
			return 1;

		error_value:
			fprintf(stderr,"ERROR: Invalid value %s for param %s\n",argv[i],argv[i-1]);
			return 1;

	}
	return 0;
}

int main(int argc, char *argv[], char **env) {
	
	vm_state initial;
	vm_state final;
	config cfg;
	int code_len;
	int *code;
	int *mem;

	// DEFAULT CONFIGURATION
	cfg.memory_size = 1024;
	cfg.max_code_size = 512;
	cfg.data_stack_size = 64;
	cfg.return_stack_size = 32;
	cfg.path = "input.mrt";

	int err = parse_args(argc, argv, env, &cfg);
	if (err) return err;
	//printf("path:%s  ds:%d  rs:%d  mem:%d \n",cfg.path, cfg.data_stack_size, cfg.return_stack_size, cfg.memory_size); // DEBUG
	
	code = calloc(cfg.max_code_size, CELL_SIZE);
	mem  = calloc(cfg.memory_size, CELL_SIZE);
	
	// BOOT & RUN
	//dump_mem("dump.mrt",code,0,40);
	code_len = load_from_file(cfg.path, code, cfg.memory_size);
	initial = boot(mem, code, code_len, cfg);
	free(code);
	final = run(initial);
	free(mem);
	
	return 0;
}
