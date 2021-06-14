#define NEXT op=mem[ip++];arg=mem[ip++]; goto *op_ptr[op];
#define JUMP NEXT

void* op_ptr[] = { &&OP_HALT,&&OP_NOP,&&OP_PUSHA,&&OP_PUSH,&&OP_DUP,&&OP_DROP,&&OP_SWAP,&&OP_STOR,&&OP_RTOS,&&OP_ADD,&&OP_MUL,&&OP_DIV,&&OP_SUB,&&OP_AND,&&OP_OR,&&OP_XOR,&&OP_NZ,&&OP_CALL,&&OP_RET,&&OP_JZ,&&OP_QCALL,&&OP_QRET,&&OP_GET,&&OP_SET,&&OP_ALLOT,&&OP_MOD,&&OP_SHL,&&OP_SHR,&&OP_USHR,&&OP_INVERT,&&OP_NEGATE,&&OP_ABS,&&OP_LE,&&OP_LT,&&OP_GE,&&OP_GT,&&OP_EQ,&&OP_NE,&&OP_XLE,&&OP_XLT,&&OP_XGE,&&OP_XGT,&&OP_XEQ,&&OP_XNE,&&OP_ROT,&&OP_OVER,&&OP_UNROT,&&OP_VGET,&&OP_VSET,&&OP_RADD,&&OP_EMIT,&&OP_DOT,&&OP_ECHO,&&OP_IF,&&OP_IFELSE,&&OP_GOTO,&&OP_LAMBDA,&&OP_MIN,&&OP_MAX,&&OP_PICK,&&OP_GTZ,&&OP_EQZ,&&OP_LTZ,&&OP_TIMES,&&OP_LOOP,&&OP_FOR,&&OP_RSUB,&&OP_RGET,&&OP_BEGIN,&&OP_BREAK,&&OP_CONTINUE,&&OP_ADDI,&&OP_MULI,&&OP_ADDGET,&&OP_ADDSET,&&OP_IADDGET,&&OP_IADDSET,&&OP_LEI,&&OP_LTI,&&OP_GEI,&&OP_GTI,&&OP_EQI,&&OP_NEI,&&OP_IOGET,&&OP_IOSET,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,&&OP_VMINFO };

t_cell op;
t_cell arg;
t_cell v,v2,v3;

NEXT;

// BRANCHING
OP_CALL:   r_push(fp); r_push(ip); ip=arg; fp=rp; JUMP; // TEST ME
OP_RET:    ip=mem[fp]; rp=fp-2; fp=mem[fp-1];     JUMP; // TEST ME
OP_QCALL:  r_push(ip); ip=s_pop();                JUMP;
OP_QRET:   ip = r_pop();                          JUMP;
OP_JZ:     v=s_pop();  if (v==0) ip=arg;          JUMP;
OP_IF:     v=s_pop(); v2=s_pop(); if(v2) { r_push(ip); ip=v; };       JUMP; // REMOVE
OP_IFELSE: v=s_pop(); v2=s_pop(); v3=s_pop(); r_push(ip); ip=v3?v2:v; JUMP; // REMOVE
OP_LAMBDA: s_push(ip); ip=arg;                    JUMP; // REMOVE
OP_GOTO:   ip=arg;                                JUMP;
OP_HALT:   goto halt;                             JUMP;
OP_NOP:                                           NEXT;
// LOOPS
OP_TIMES:  v=s_pop(); r_push(arg); r_push(1); r_push(v-1); r_push(0); NEXT; // with loop frame // TODO zero iterations
OP_FOR:    v=s_pop();v2=s_pop();v3=s_pop(); r_push(arg); r_push(v); r_push(v3); r_push(v2); NEXT; // TODO zero iterations
OP_LOOP:   if (mem[rp]<mem[rp-1]) {ip=arg; mem[rp]+=mem[rp-2];} else {rp-=4;};  JUMP;
OP_BEGIN:  r_push(arg); r_push(0); r_push(0); r_push(0); NEXT; // REMOVE -> replace with for with step=0
OP_BREAK:    ip=mem[rp-3]+2; rp-=4;                      JUMP;
OP_CONTINUE: ip=mem[rp-3];                               JUMP;
// RETURN STACK
OP_STOR:   v=s_pop(); r_push(v);              NEXT;
OP_RTOS:   v=r_pop(); s_push(v);              NEXT;
OP_RADD:   rp+=arg;                           NEXT; // REMOVE
OP_RSUB:   rp-=arg;                           NEXT; // REMOVE
OP_RGET:   v=mem[rp-arg]; s_push(v);          NEXT; // for loop variables
OP_VGET:   v=mem[fp+arg]; s_push(v);          NEXT; // local variables
OP_VSET:   v=s_pop(); mem[fp+arg]=v;          NEXT; // local variables
// DATA STACK
OP_SWAP:   v=tos; tos=mem[sp]; mem[sp]=v; NEXT;
OP_PUSH:   s_push(arg);                   NEXT;
OP_PUSHA:  s_push(arg);                   NEXT;
OP_DUP:    s_push(tos);                   NEXT;
OP_DROP:   v=s_pop();                     NEXT;
OP_OVER:   s_push(mem[sp]);               NEXT;
OP_ROT:    v=mem[sp-1]; mem[sp-1]=mem[sp]; mem[sp]=tos; tos=v; NEXT;
OP_UNROT:  v=tos; tos=mem[sp]; mem[sp]=mem[sp-1]; mem[sp-1]=v; NEXT;
// ALU
OP_MUL:    v=s_pop(); tos *= v;           NEXT;
OP_DIV:    v=s_pop(); tos /= v;           NEXT;
OP_ADD:    v=s_pop(); tos += v;           NEXT;
OP_SUB:    v=s_pop(); tos -= v;           NEXT;
OP_AND:    v=s_pop(); tos &= v;           NEXT;
OP_OR:     v=s_pop(); tos |= v;           NEXT;
OP_XOR:    v=s_pop(); tos ^= v;           NEXT;
OP_MOD:    v=s_pop(); tos %= v;           NEXT;
OP_SHL:    v=s_pop(); tos <<= v;          NEXT;
OP_SHR:    v=s_pop(); tos >>= v;          NEXT;
OP_INVERT: tos= ~tos;                     NEXT; 
OP_NEGATE: tos= -tos;                     NEXT; 
OP_ABS:    tos= tos<0 ? -tos:tos;         NEXT;
OP_USHR:   v=s_pop(); uv=tos; tos=uv>>v;  NEXT; // NOT CORE
OP_ADDI:   tos += arg;                    NEXT; // TURBO
OP_MULI:   tos *= arg;                    NEXT; // TURBO
// COMPARATORS - MAIN
OP_LT:     v=s_pop(); tos= tos<v  ? 1:0;  NEXT;
OP_LE:     v=s_pop(); tos= tos<=v ? 1:0;  NEXT;
OP_GT:     v=s_pop(); tos= tos>v  ? 1:0;  NEXT;
OP_GE:     v=s_pop(); tos= tos>=v ? 1:0;  NEXT;
OP_EQ:     v=s_pop(); tos= tos==v ? 1:0;  NEXT;
OP_NE:     v=s_pop(); tos= tos!=v ? 1:0;  NEXT;
// COMPARATORS - IMMEDIATE
OP_LTI:    tos= tos<arg  ? 1:0;  NEXT; // TURBO
OP_LEI:    tos= tos<=arg ? 1:0;  NEXT; // TURBO
OP_GTI:    tos= tos>arg  ? 1:0;  NEXT; // TURBO
OP_GEI:    tos= tos>=arg ? 1:0;  NEXT; // TURBO
OP_EQI:    tos= tos==arg ? 1:0;  NEXT; // TURBO
OP_NEI:    tos= tos!=arg ? 1:0;  NEXT; // TURBO				
// COMPARATORS - EXPERIMENTAL (NON DESTRUCTIVE)
OP_XLT:    s_push(mem[sp]<tos  ? 1:0);  NEXT; // NOT CORE
OP_XGT:    s_push(mem[sp]>tos  ? 1:0);  NEXT; // NOT CORE
OP_XLE:    s_push(mem[sp]<=tos ? 1:0);  NEXT; // NOT CORE
OP_XGE:    s_push(mem[sp]>=tos ? 1:0);  NEXT; // NOT CORE
OP_XEQ:    s_push(mem[sp]==tos ? 1:0);  NEXT; // NOT CORE
OP_XNE:    s_push(mem[sp]!=tos ? 1:0);  NEXT; // NOT CORE
// COMPARATORS - AUX
OP_NZ:     tos = tos!=0 ? 1:0;            NEXT; // REMOVE
OP_GTZ:    tos = tos>0  ? 1:0;            NEXT; // REMOVE
OP_EQZ:    tos = tos==0 ? 1:0;            NEXT; // REMOVE
OP_LTZ:    tos = tos<0  ? 1:0;            NEXT; // REMOVE
OP_MIN:    v=s_pop(); tos=tos<v ? tos:v;  NEXT;
OP_MAX:    v=s_pop(); tos=tos>v ? tos:v;  NEXT;
OP_PICK:   tos= tos ? mem[sp-1]:mem[sp]; sp-=2; NEXT;
// MEMORY
OP_GET:    tos = mem[tos];                NEXT;
OP_SET:    v=s_pop(); mem[v]=s_pop();     NEXT;
OP_ALLOT:  v=s_pop(); s_push(hp); hp+=v;  NEXT;
OP_ADDGET: v=s_pop(); tos+=v; tos=mem[tos];              NEXT;
OP_ADDSET: v=s_pop(); tos+=v; v=s_pop(); mem[v]=s_pop(); NEXT;
OP_IADDGET: v=mem[rp-arg]; s_push(v); v=s_pop(); tos+=v; tos=mem[tos];              NEXT; // TURBO OPTIMIZE COMBO
OP_IADDSET: v=mem[rp-arg]; s_push(v); v=s_pop(); tos+=v; v=s_pop(); mem[v]=s_pop(); NEXT; // TURBO OPTIMIZE COMBO
// DEBUG
OP_VMINFO:
	printf("T:%d\tSP:%d\tRP:%d\tFP:%d\tIP:%d\tHP:%d\tSDMX:%d\tRDMX:%d\tDDMX:%d\tOLI:%d\tILC:%d\tdt:%d ms \n",tos,sp,rp,fp,ip,hp,sd_max,rd_max,hd_max,oli,ilc,ms_clock()-ts_vminfo);
	ts_vminfo = ms_clock();
	NEXT;
// PRIMITIVE OUTPUT
OP_EMIT: v=s_pop(); printf("%c",v);  NEXT;
OP_DOT:  v=s_pop(); printf("%d ",v); NEXT;
OP_ECHO: v=s_pop(); print_word(v);   NEXT;
//
OP_IOGET: NEXT;
OP_IOSET: NEXT;
//printf("T:%d\tR:%d\tSP:%d\tRP:%d\tFP:%d\tIP:%d\tDP:%d\tMEM[SP]:%d\tOP:%d\tdt:%d ms \n",tos,mem[rp],sp,rp,fp,ip,hp,mem[sp],op,ms_clock()-ts_vminfo); // XXX debug
