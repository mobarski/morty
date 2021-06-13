#define REPL_SWITCH_BODY { \
case STOP: goto OP_STOP; \
case NOP: goto OP_NOP; \
case PUSHA: goto OP_PUSHA; \
case PUSH: goto OP_PUSH; \
case DUP: goto OP_DUP; \
case DROP: goto OP_DROP; \
case SWAP: goto OP_SWAP; \
case STOR: goto OP_STOR; \
case RTOS: goto OP_RTOS; \
case ADD: goto OP_ADD; \
case MUL: goto OP_MUL; \
case DIV: goto OP_DIV; \
case SUB: goto OP_SUB; \
case AND: goto OP_AND; \
case OR: goto OP_OR; \
case XOR: goto OP_XOR; \
case NZ: goto OP_NZ; \
case CALL: goto OP_CALL; \
case RET: goto OP_RET; \
case JZ: goto OP_JZ; \
case QCALL: goto OP_QCALL; \
case QRET: goto OP_QRET; \
case GET: goto OP_GET; \
case SET: goto OP_SET; \
case ALLOT: goto OP_ALLOT; \
case MOD: goto OP_MOD; \
case SHL: goto OP_SHL; \
case SHR: goto OP_SHR; \
case USHR: goto OP_USHR; \
case INVERT: goto OP_INVERT; \
case NEGATE: goto OP_NEGATE; \
case ABS: goto OP_ABS; \
case LE: goto OP_LE; \
case LT: goto OP_LT; \
case GE: goto OP_GE; \
case GT: goto OP_GT; \
case EQ: goto OP_EQ; \
case NE: goto OP_NE; \
case XLE: goto OP_XLE; \
case XLT: goto OP_XLT; \
case XGE: goto OP_XGE; \
case XGT: goto OP_XGT; \
case XEQ: goto OP_XEQ; \
case XNE: goto OP_XNE; \
case ROT: goto OP_ROT; \
case OVER: goto OP_OVER; \
case UNROT: goto OP_UNROT; \
case VGET: goto OP_VGET; \
case VSET: goto OP_VSET; \
case RADD: goto OP_RADD; \
case EMIT: goto OP_EMIT; \
case DOT: goto OP_DOT; \
case ECHO: goto OP_ECHO; \
case IF: goto OP_IF; \
case IFELSE: goto OP_IFELSE; \
case GOTO: goto OP_GOTO; \
case LAMBDA: goto OP_LAMBDA; \
case MIN: goto OP_MIN; \
case MAX: goto OP_MAX; \
case PICK: goto OP_PICK; \
case GTZ: goto OP_GTZ; \
case EQZ: goto OP_EQZ; \
case LTZ: goto OP_LTZ; \
case TIMES: goto OP_TIMES; \
case LOOP: goto OP_LOOP; \
case FOR: goto OP_FOR; \
case RSUB: goto OP_RSUB; \
case RGET: goto OP_RGET; \
case BEGIN: goto OP_BEGIN; \
case BREAK: goto OP_BREAK; \
case CONTINUE: goto OP_CONTINUE; \
case ADDI: goto OP_ADDI; \
case MULI: goto OP_MULI; \
case ADDGET: goto OP_ADDGET; \
case ADDSET: goto OP_ADDSET; \
case IADDGET: goto OP_IADDGET; \
case IADDSET: goto OP_IADDSET; \
case LEI: goto OP_LEI; \
case LTI: goto OP_LTI; \
case GEI: goto OP_GEI; \
case GTI: goto OP_GTI; \
case EQI: goto OP_EQI; \
case NEI: goto OP_NEI; \
case IOGET: goto OP_IOGET; \
case IOSET: goto OP_IOSET; \
case VMINFO: goto OP_VMINFO; \
}

#define NEXT op=mem[ip++]; arg=mem[ip++]; switch(op) REPL_SWITCH_BODY
#define JUMP op=mem[ip++]; arg=mem[ip++]; switch(op) REPL_SWITCH_BODY

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
OP_STOP:   goto stop;                             JUMP; // TODO RENAME: HALT
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
