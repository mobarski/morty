
// BRANCHING
OP_CALL:   r_push(fp); r_push(ip); ip=arg; fp=rp; JUMP; // TEST ME
OP_RET:    ip=mem[fp]; rp=fp-2; fp=mem[fp-1];     JUMP; // TEST ME
OP_QCALL:  r_push(ip); ip=s_pop();                JUMP;
OP_QRET:   ip = r_pop();                          JUMP;
OP_JZ:     v=s_pop();  if (v==0) ip=arg;          JUMP;
OP_IF:     v=s_pop(); v2=s_pop(); if(v2) { r_push(ip); ip=v; };       JUMP; // TEST ME
OP_IFELSE: v=s_pop(); v2=s_pop(); v3=s_pop(); r_push(ip); ip=v3?v2:v; JUMP; // TEST ME
OP_LAMBDA: s_push(ip); ip=arg;                    JUMP; // TODO: RENAME
OP_GOTO:   ip=arg;                                JUMP;
OP_STOP:   goto stop;                             JUMP;
// RETURN STACK
OP_STOR:  v=s_pop(); r_push(v);              NEXT;
OP_RTOS:  v=r_pop(); s_push(v);              NEXT;
OP_RADD:  rp+=arg;                           NEXT; // local variables
OP_VGET:  v=mem[fp+arg]; s_push(v);          NEXT; // local variables
OP_VSET:  v=s_pop(); mem[fp+arg]=v;          NEXT; // local variables
// DATA STACK
OP_SWAP:  v=tos; tos=mem[sp]; mem[sp]=v; NEXT;
OP_PUSH:  s_push(arg);                   NEXT;
OP_DUP:   s_push(tos);                   NEXT;
OP_DROP:  v=s_pop();                     NEXT;
OP_OVER:  s_push(mem[sp]);               NEXT;
OP_ROT:   v=mem[sp-1]; mem[sp-1]=mem[sp]; mem[sp]=tos; tos=v; NEXT;
OP_UNROT: v=tos; tos=mem[sp]; mem[sp]=mem[sp-1]; mem[sp-1]=v; NEXT;
// ALU
OP_MUL:   v=s_pop(); tos *= v;           NEXT;
OP_DIV:   v=s_pop(); tos /= v;           NEXT;
OP_ADD:   v=s_pop(); tos += v;           NEXT;
OP_SUB:   v=s_pop(); tos -= v;           NEXT;
OP_AND:   v=s_pop(); tos &= v;           NEXT;
OP_OR:    v=s_pop(); tos |= v;           NEXT;
OP_XOR:   v=s_pop(); tos ^= v;           NEXT;
OP_MOD:   v=s_pop(); tos %= v;           NEXT; // NOT CORE
OP_SHL:   v=s_pop(); tos <<= v;          NEXT; // NOT CORE
OP_SHR:   v=s_pop(); tos >>= v;          NEXT; // NOT CORE
OP_USHR:  v=s_pop(); uv=tos; tos=uv>>v;  NEXT; // NOT CORE
// COMPARATORS
OP_LT:    v=s_pop(); tos= tos<v  ? 1:0;  NEXT; // NOT CORE
OP_LE:    v=s_pop(); tos= tos<=v ? 1:0;  NEXT; // NOT CORE
OP_GT:    v=s_pop(); tos= tos>v  ? 1:0;  NEXT; // NOT CORE
OP_GE:    v=s_pop(); tos= tos>=v ? 1:0;  NEXT; // NOT CORE
OP_EQ:    v=s_pop(); tos= tos==v ? 1:0;  NEXT; // NOT CORE
OP_NE:    v=s_pop(); tos= tos!=v ? 1:0;  NEXT; // NOT CORE
OP_NZ:    tos = tos!=0 ? 1:0;            NEXT;
OP_MIN:   v=s_pop(); tos=tos<v ? tos:v;  NEXT; // NOT CORE
OP_MAX:   v=s_pop(); tos=tos>v ? tos:v;  NEXT; // NOT CORE
OP_PICK:  tos= tos ? mem[sp-1]:mem[sp]; sp-=2; NEXT; // NOT CORE NEW
// MEMORY
OP_GET:   tos = mem[tos];                NEXT;
OP_SET:   v=s_pop(); mem[v]=s_pop();     NEXT;
OP_ALLOT: v=s_pop(); s_push(dp); dp+=v;  NEXT;
// DEBUG
OP_VMINFO:
	printf("T:%d\tSP:%d\tRP:%d\tFP:%d\tIP:%d\tDP:%d\tSDMX:%d\tRDMX:%d\tDDMX:%d\tOLI:%d\tILC:%d\tdt:%d ms \n",tos,sp,rp,fp,ip,dp,sd_max,rd_max,dd_max,oli,ilc,ms_clock()-ts_vminfo);
	ts_vminfo = ms_clock();
	NEXT;
// PRIMITIVE OUTPUT
OP_EMIT: v=s_pop(); printf("%c",v);  NEXT;
OP_DOT:  v=s_pop(); printf("%d ",v); NEXT;
OP_ECHO: v=s_pop(); word_buf[0]=0;
	word_buf[0]=v; // TODO: propper byte-by-byte for LITTLE/BIG endian handling
	printf("%s",(char*)(&word_buf));
	NEXT;
