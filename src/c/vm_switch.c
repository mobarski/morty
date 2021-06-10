t_cell op  = mem[ip++];
t_cell arg = mem[ip++];
t_cell v,v2,v3;
switch(op) {
	// BRANCHING
	case CALL:   r_push(fp); r_push(ip); ip=arg; fp=rp; break; // TEST ME
	case RET:    ip=mem[fp]; rp=fp-2; fp=mem[fp-1];     break; // TEST ME
	case QCALL:  r_push(ip); ip=s_pop();                break;
	case QRET:   ip = r_pop();                          break;
	case JZ:     v=s_pop();  if (v==0) ip=arg;          break;
	case IF:     v=s_pop(); v2=s_pop(); if(v2) { r_push(ip); ip=v; };       break; // REMOVE
	case IFELSE: v=s_pop(); v2=s_pop(); v3=s_pop(); r_push(ip); ip=v3?v2:v; break; // REMOVE
	case LAMBDA: s_push(ip); ip=arg;                    break; // REMOVE
	case GOTO:   ip=arg;                                break;
	case STOP:   goto stop;                             break; // TODO RENAME: HALT
	// LOOPS
	//case TIMES:  v=s_pop(); r_push(arg); r_push(v-1); break; // with loop frame // TODO zero iterations
	//case LOOP:   if (mem[rp]>0) {ip=arg; mem[rp]-=1;} else {rp-=2;};  break;
	//case TIMES:  v=s_pop(); r_push(v-1); break; // no loop frame
	//case LOOP:   if (mem[rp]>0) {ip=arg; mem[rp]-=1;} else {rp-=1;};  break;
	//case TIMES:  v=s_pop(); r_push(arg); r_push(0); r_push(v-1); break; // with loop frame
	//case LOOP:   if (mem[rp]>mem[rp-1]) {ip=arg; mem[rp]-=1;} else {rp-=3;};  break;
	//case TIMES:  v=s_pop(); r_push(arg); r_push(1); r_push(0); r_push(v-1); break; // with loop frame
	//case LOOP:   if (mem[rp]>mem[rp-1]) {ip=arg; mem[rp]-=mem[rp-2];} else {rp-=4;};  break;
	case TIMES:  v=s_pop(); r_push(arg); r_push(1); r_push(v-1); r_push(0); break; // with loop frame // TODO zero iterations
	case FOR:    v=s_pop();v2=s_pop();v3=s_pop(); r_push(arg); r_push(v); r_push(v3); r_push(v2); break; // TODO zero iterations
	case LOOP:   if (mem[rp]<mem[rp-1]) {ip=arg; mem[rp]+=mem[rp-2];} else {rp-=4;};  break;
	case BEGIN:  r_push(arg); r_push(0); r_push(0); r_push(0); break; // REMOVE -> replace with for with step=0
	case BREAK:    ip=mem[rp-3]+2; rp-=4;                      break;
	case CONTINUE: ip=mem[rp-3];                               break;
	// RETURN STACK
	case STOR:   v=s_pop(); r_push(v);              break;
	case RTOS:   v=r_pop(); s_push(v);              break;
	case RADD:   rp+=arg;                           break; // REMOVE
	case RSUB:   rp-=arg;                           break; // REMOVE
	case RGET:   v=mem[rp-arg]; s_push(v);          break; // for loop variables
	case VGET:   v=mem[fp+arg]; s_push(v);          break; // local variables
	case VSET:   v=s_pop(); mem[fp+arg]=v;          break; // local variables
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
	case MOD:    v=s_pop(); tos %= v;           break;
	case SHL:    v=s_pop(); tos <<= v;          break;
	case SHR:    v=s_pop(); tos >>= v;          break;
	case INVERT: tos= ~tos;                     break; 
	case NEGATE: tos= -tos;                     break; 
	case ABS:    tos= tos<0 ? -tos:tos;         break;
	case USHR:   v=s_pop(); uv=tos; tos=uv>>v;  break; // NOT CORE
	case ADDI:   tos += arg;                    break; // TURBO
	case MULI:   tos *= arg;                    break; // TURBO
	// COMPARATORS - MAIN
	case LT:     v=s_pop(); tos= tos<v  ? 1:0;  break;
	case LE:     v=s_pop(); tos= tos<=v ? 1:0;  break;
	case GT:     v=s_pop(); tos= tos>v  ? 1:0;  break;
	case GE:     v=s_pop(); tos= tos>=v ? 1:0;  break;
	case EQ:     v=s_pop(); tos= tos==v ? 1:0;  break;
	case NE:     v=s_pop(); tos= tos!=v ? 1:0;  break;
	// COMPARATORS - IMMEDIATE
	case LTI:    tos= tos<arg  ? 1:0;  break; // TURBO
	case LEI:    tos= tos<=arg ? 1:0;  break; // TURBO
	case GTI:    tos= tos>arg  ? 1:0;  break; // TURBO
	case GEI:    tos= tos>=arg ? 1:0;  break; // TURBO
	case EQI:    tos= tos==arg ? 1:0;  break; // TURBO
	case NEI:    tos= tos!=arg ? 1:0;  break; // TURBO				
	// COMPARATORS - EXPERIMENTAL (NON DESTRUCTIVE)
	case XLT:    s_push(mem[sp]<tos  ? 1:0);  break; // NOT CORE
	case XGT:    s_push(mem[sp]>tos  ? 1:0);  break; // NOT CORE
	case XLE:    s_push(mem[sp]<=tos ? 1:0);  break; // NOT CORE
	case XGE:    s_push(mem[sp]>=tos ? 1:0);  break; // NOT CORE
	case XEQ:    s_push(mem[sp]==tos ? 1:0);  break; // NOT CORE
	case XNE:    s_push(mem[sp]!=tos ? 1:0);  break; // NOT CORE
	// COMPARATORS - AUX
	case NZ:     tos = tos!=0 ? 1:0;            break; // REMOVE
	case GTZ:    tos = tos>0  ? 1:0;            break; // REMOVE
	case EQZ:    tos = tos==0 ? 1:0;            break; // REMOVE
	case LTZ:    tos = tos<0  ? 1:0;            break; // REMOVE
	case MIN:    v=s_pop(); tos=tos<v ? tos:v;  break;
	case MAX:    v=s_pop(); tos=tos>v ? tos:v;  break;
	case PICK:   tos= tos ? mem[sp-1]:mem[sp]; sp-=2; break;
	// MEMORY
	case GET:    tos = mem[tos];                break;
	case SET:    v=s_pop(); mem[v]=s_pop();     break;
	case ALLOT:  v=s_pop(); s_push(hp); hp+=v;  break;
	case ADDGET: v=s_pop(); tos+=v; tos=mem[tos];              break; // TURBO
	case ADDSET: v=s_pop(); tos+=v; v=s_pop(); mem[v]=s_pop(); break; // TURBO
	case IADDGET: v=mem[rp-arg]; s_push(v); v=s_pop(); tos+=v; tos=mem[tos];              break; // TURBO OPTIMIZE COMBO
	case IADDSET: v=mem[rp-arg]; s_push(v); v=s_pop(); tos+=v; v=s_pop(); mem[v]=s_pop(); break; // TURBO OPTIMIZE COMBO

	// DEBUG
	case VMINFO:
		fprintf(stderr,"T:%d\tSP:%d\tRP:%d\tFP:%d\tIP:%d\tHP:%d\tSDMX:%d\tRDMX:%d\tDDMX:%d\tOLI:%d\tILC:%d\tdt:%d ms \n",tos,sp,rp,fp,ip,hp,sd_max,rd_max,hd_max,oli,ilc,ms_clock()-ts_vminfo);
		fflush(stderr);
		ts_vminfo = ms_clock();
		break;
	// PRIMITIVE OUTPUT
	case EMIT: v=s_pop(); printf("%c",v);  break;
	case DOT:  v=s_pop(); printf("%d ",v); break;
	case ECHO: v=s_pop(); word_buf[0]=0;
		word_buf[0]=v; // TODO: propper byte-by-byte for LITTLE/BIG endian handling
		printf("%s",(char*)(&word_buf));
		break;
	//
	case IOGET: break; // TODO
	case IOSET: break; // TODO
}

//printf("T:%d\tR:%d\tSP:%d\tRP:%d\tFP:%d\tIP:%d\tDP:%d\tMEM[SP]:%d\tOP:%d\tdt:%d ms \n",tos,mem[rp],sp,rp,fp,ip,hp,mem[sp],op,ms_clock()-ts_vminfo); // XXX debug
