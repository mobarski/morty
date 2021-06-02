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
	int hp;
	int *mem;
} vm_state;

// VM RUN
vm_state
run(vm_state state) {
	
	// VM REGISTERS
	int  tos = state.tos;
	int   ip = state.ip;
	int   sp = state.sp;
	int   rp = state.rp;
	int   fp = state.fp;
	int   hp = state.hp;
	int *mem = state.mem;
	
	// FINAL STATE
	vm_state final;

	// AUX VARS
	unsigned int uv=0; // for USHR
	int ts_vminfo = ms_clock();
	int word_buf[2] = {0,0};
	int sd=0;     // stack depth
	int sd_max=0; // max stack depth
	int rd=0;     // return stack depth
	int rd_max=0; // max return stack depth
	int hd=0;     // heap depth
	int hd_max=0; // max heap depth
	int oli=0;     // outer loop iterations
	int ilc=1000000; // inner loop cnt
	
	// TODO: propper main loop
	// MAIN LOOP
	for(;;) { // outer loop
		//for(int i=0; i<ilc; i++) { // inner loop // +10% time :/
		for(;;) { // inner loop
			int op  = mem[ip++];
			int arg = mem[ip++];
			int v,v2,v3;
			switch(op) {
				// BRANCHING
				case CALL:   r_push(fp); r_push(ip); ip=arg; fp=rp; break; // TEST ME
				case RET:    ip=mem[fp]; rp=fp-2; fp=mem[fp-1];     break; // TEST ME
				case QCALL:  r_push(ip); ip=s_pop();                break;
				case QRET:   ip = r_pop();                          break;
				case JZ:     v=s_pop();  if (v==0) ip=arg;          break;
				case IF:     v=s_pop(); v2=s_pop(); if(v2) { r_push(ip); ip=v; };       break; // TEST ME
				case IFELSE: v=s_pop(); v2=s_pop(); v3=s_pop(); r_push(ip); ip=v3?v2:v; break; // TEST ME
				case LAMBDA: s_push(ip); ip=arg;                    break; // TODO: RENAME -> STATIC ARRAY ???
				case GOTO:   ip=arg;                                break;
				case STOP:   goto stop;                             break;
				case LOOP:   if (mem[rp]>0) {ip=arg; mem[rp]-=1;} else {rp-=2;};  break;
				// RETURN STACK
				case STOR:   v=s_pop(); r_push(v);              break;
				case RTOS:   v=r_pop(); s_push(v);              break;
				case RADD:   rp+=arg;                           break; // REMOVE
				case RSUB:   rp-=arg;                           break; // REMOVE
				case RGET:   v=mem[rp-arg]; s_push(v);          break; // for loop variables
				case VGET:   v=mem[fp+arg]; s_push(v);          break; // local variables
				case VSET:   v=s_pop(); mem[fp+arg]=v;          break; // local variables
				case TIMES:  v=s_pop(); r_push(arg); r_push(v-1); break; // with loop frame
				// DATA STACK
				case SWAP:   v=tos; tos=mem[sp]; mem[sp]=v; break;
				case PUSH:   s_push(arg);                   break;
				case DUP:    s_push(tos);                   break;
				case DROP:   v=s_pop();                     break;
				case OVER:   s_push(mem[sp]);               break;
				case ROT:    v=mem[sp-1]; mem[sp-1]=mem[sp]; mem[sp]=tos; tos=v; break;
				case UNROT:  v=tos; tos=mem[sp]; mem[sp]=mem[sp-1]; mem[sp-1]=v; break;
				// ALU
				case MUL:    v=s_pop(); tos *= v;           break;
				case DIV:    v=s_pop(); tos /= v;           break;
				case ADD:    v=s_pop(); tos += v;           break;
				case SUB:    v=s_pop(); tos -= v;           break;
				case AND:    v=s_pop(); tos &= v;           break;
				case OR:     v=s_pop(); tos |= v;           break;
				case XOR:    v=s_pop(); tos ^= v;           break;
				case MOD:    v=s_pop(); tos %= v;           break; // NOT CORE
				case SHL:    v=s_pop(); tos <<= v;          break; // NOT CORE
				case SHR:    v=s_pop(); tos >>= v;          break; // NOT CORE
				case USHR:   v=s_pop(); uv=tos; tos=uv>>v;  break; // NOT CORE
				case INVERT: tos= ~tos;                     break; // NOT CORE
				case NEGATE: tos= -tos;                     break; // NOT CORE
				case ABS:    tos= tos<0 ? -tos:tos;         break; // NOT CORE
				// COMPARATORS - MAIN
				case LT:     v=s_pop(); tos= tos<v  ? 1:0;  break; // NOT CORE
				case LE:     v=s_pop(); tos= tos<=v ? 1:0;  break; // NOT CORE
				case GT:     v=s_pop(); tos= tos>v  ? 1:0;  break; // NOT CORE
				case GE:     v=s_pop(); tos= tos>=v ? 1:0;  break; // NOT CORE
				case EQ:     v=s_pop(); tos= tos==v ? 1:0;  break; // NOT CORE
				case NE:     v=s_pop(); tos= tos!=v ? 1:0;  break; // NOT CORE
				// COMPARATORS - EXPERIMENTAL (NON DESTRUCTIVE)
				case XLT:    s_push(mem[sp]<tos  ? 1:0);  break; // NOT CORE
				case XGT:    s_push(mem[sp]>tos  ? 1:0);  break; // NOT CORE
				case XLE:    s_push(mem[sp]<=tos ? 1:0);  break; // NOT CORE
				case XGE:    s_push(mem[sp]>=tos ? 1:0);  break; // NOT CORE
				case XEQ:    s_push(mem[sp]==tos ? 1:0);  break; // NOT CORE
				case XNE:    s_push(mem[sp]!=tos ? 1:0);  break; // NOT CORE
				// COMPARATORS - AUX
				case NZ:     tos = tos!=0 ? 1:0;            break; // NOT CORE
				case GTZ:    tos = tos>0  ? 1:0;            break; // NOT CORE
				case EQZ:    tos = tos==0 ? 1:0;            break; // NOT CORE
				case LTZ:    tos = tos<0  ? 1:0;            break; // NOT CORE
				case MIN:    v=s_pop(); tos=tos<v ? tos:v;  break; // NOT CORE
				case MAX:    v=s_pop(); tos=tos>v ? tos:v;  break; // NOT CORE
				case PICK:   tos= tos ? mem[sp-1]:mem[sp]; sp-=2; break; // NOT CORE NEW
				// MEMORY
				case GET:    tos = mem[tos];                break;
				case SET:    v=s_pop(); mem[v]=s_pop();     break;
				case ALLOT:  v=s_pop(); s_push(hp); hp+=v;  break;
				// DEBUG
				case VMINFO:
					printf("T:%d\tSP:%d\tRP:%d\tFP:%d\tIP:%d\tDP:%d\tSDMX:%d\tRDMX:%d\tDDMX:%d\tOLI:%d\tILC:%d\tdt:%d ms \n",tos,sp,rp,fp,ip,hp,sd_max,rd_max,hd_max,oli,ilc,ms_clock()-ts_vminfo);
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
			
			//printf("T:%d\tR:%d\tSP:%d\tRP:%d\tFP:%d\tIP:%d\tDP:%d\tMEM[SP]:%d\tOP:%d\tdt:%d ms \n",tos,mem[rp],sp,rp,fp,ip,hp,mem[sp],op,ms_clock()-ts_vminfo); // XXX debug
		}
		oli++;
		sd = sp-state.sp;
		rd = rp-state.rp;
		hd = hp-state.hp;
		sd_max = sd>sd_max ? sd:sd_max;
		rd_max = rd>rd_max ? rd:rd_max;
		hd_max = hd>hd_max ? hd:hd_max;
	}
	fprintf(stderr,"WARNING: Iterations limit reached!\n");
	
	stop:
	// RETURN FINAL VM STATE
	final.tos = tos;
	final.ip  = ip;
	final.sp  = sp;
	final.rp  = rp;
	final.fp  = fp;
	final.hp  = hp;
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
vm_state
boot(int *mem, int *code, int code_len, config cfg) {
	vm_state state;
	
	state.tos = 0;
	state.ip  = 0;
	
	for (int i=0; i<code_len; i++) {
		mem[state.ip+i] = code[i];
		//printf("BOOT: mem[%02d] -> %d\n", state.ip+i, code[i]); // XXX debug
	}
	
	state.sp  = state.ip + code_len + 10; // 10 -> GAP between CODE and DATA STACK
	state.rp  = state.sp + cfg.data_stack_size;
	state.hp  = state.rp + cfg.return_stack_size;
	state.fp  = state.rp;
	state.mem = mem;
	
	return state;
}


int
load_from_file(char *path, int *code, int max_len) {
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


int
dump_mem(char *path, int *mem, int from, int ncells) {
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

int
parse_args(int argc, char *argv[], char **env, config *cfg) {
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

int
main(int argc, char *argv[], char **env) {
	
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
