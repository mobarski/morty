t_cell op  = mem[ip++];
t_cell arg = mem[ip++];
t_cell v,v1,v2,v3;

#ifdef DEBUG_FREQ
	final.op_freq[op] += 1;
	final.op_cnt += 1;
#endif
#ifdef DEBUG_TRACE
	printf("T:%-6d SD:%-4d RD:%-4d IP:%-4d %-8s %-4d\n",tos,sp-state.sp,rp-state.rp,ip,OP_NAME[op],arg); // XXX debug
	//printf("T:%d\tR:%d\tSP:%d\tRP:%d\tFP:%d\tIP:%d\tDP:%d\tMEM[SP]:%d\tOP:%d\tdt:%d ms \n",tos,mem[rp],sp,rp,fp,ip,hp,mem[sp],op,ms_clock()-ts_vminfo); // XXX debug
#endif

switch(op) {
	// BRANCHING
	case CALL:   r_push(fp); r_push(ip); ip=arg; fp=rp; break; // TEST ME
	case RET:    ip=mem[fp]; rp=fp-2; fp=mem[fp-1];     break; // TEST ME
	case QCALL:  r_push(ip); ip=s_pop();                break;
	case QRET:   ip = r_pop();                          break;
	case JZ:     v=s_pop();  if (v==0) ip=arg;          break;
	case GOTO:   ip=arg;                                break;
	case HALT:   goto halt;                             break;
	case NOP:                                           break;
	// LOOPS
	case TIMES:  v=s_pop(); r_push(arg); r_push(1); r_push(v-1); r_push(0); break; // with loop frame // TODO zero iterations
	case FOR:    v3=s_pop();v2=s_pop();v1=s_pop(); r_push(arg); r_push(v3); r_push(v2); r_push(v1); break; // TODO zero iterations
	case LOOP:   if (mem[rp]<mem[rp-1]) {ip=arg; mem[rp]+=mem[rp-2];} else {rp-=4;};  break;
	case BEGIN:  r_push(arg); r_push(0); r_push(1); r_push(0); break; // REMOVE? -> replace with "0 1 0 for"
	case BREAK:    ip=mem[rp-3]+2; rp-=4;                      break;
	case CONTINUE: ip=mem[rp-3];                               break;
	// RETURN STACK
	case STOR:   v=s_pop(); r_push(v);              break;
	case RTOS:   v=r_pop(); s_push(v);              break;
	case RGET:   v=mem[rp-arg]; s_push(v);          break; // loop variables
	case VGET:   v=mem[fp+arg]; s_push(v);          break; // local variables
	case VSET:   v=s_pop(); mem[fp+arg]=v;          break; // local variables
	// DATA STACK
	case SWAP:   v=tos; tos=mem[sp]; mem[sp]=v; break;
	case PUSH:   s_push(arg);                   break;
	case PUSHA:  s_push(arg);                   break;
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
	case USHR:   v=s_pop(); uv=tos; tos=uv>>v;  break; // NOT CORE ? -> for hash?
	// COMPARATORS - MAIN
	case LT:     v=s_pop(); tos= tos<v  ? 1:0;  break;
	case LE:     v=s_pop(); tos= tos<=v ? 1:0;  break;
	case GT:     v=s_pop(); tos= tos>v  ? 1:0;  break;
	case GE:     v=s_pop(); tos= tos>=v ? 1:0;  break;
	case EQ:     v=s_pop(); tos= tos==v ? 1:0;  break;
	case NE:     v=s_pop(); tos= tos!=v ? 1:0;  break;
	// COMPARATORS - AUX
	case MIN:    v=s_pop(); tos=tos<v ? tos:v;        break;
	case MAX:    v=s_pop(); tos=tos>v ? tos:v;        break;
	case PICK:   tos= tos ? mem[sp-1]:mem[sp]; sp-=2; break;
	// MEMORY
	case GET:    tos = mem[tos];                break;
	case SET:    v=s_pop(); mem[v]=s_pop();     break;
	case GGET:   v=mem[gp+arg]; s_push(mem[v]); break; // global variables
	case GSET:   v=s_pop(); mem[mem[gp+arg]]=v; break; // global variables
	case ALLOT:  v=s_pop(); s_push(hp); hp+=v;  break;

	// DEBUG
	case VMINFO:
		fprintf(stderr,"T:%d\tSP:%d\tRP:%d\tFP:%d\tIP:%d\tHP:%d\tSDMX:%d\tRDMX:%d\tDDMX:%d\tOLI:%d\tILC:%d\tdt:%d ms \n",tos,sp,rp,fp,ip,hp,sd_max,rd_max,hd_max,oli,ilc,ms_clock()-ts_vminfo);
		fflush(stderr);
		ts_vminfo = ms_clock();
		break;
	// PRIMITIVE OUTPUT
	case EMIT: v=s_pop(); printf("%c",v);  break;
	case DOT:  v=s_pop(); printf("%d ",v); break;
	case ECHO: v=s_pop(); print_word(v);   break;
	//
	case IOGET: break; // TODO
	case IOSET: break; // TODO
	
	// === TURBO EXTENSION ====================================================
	
	// ALU
	case ADDI:   tos += arg;                    break;
	case MULI:   tos *= arg;                    break;
	// MEMORY
	case ADDGET: v=s_pop(); tos+=v; tos=mem[tos];              break;
	case ADDSET: v=s_pop(); tos+=v; v=s_pop(); mem[v]=s_pop(); break;
	case IADDGET: v=mem[rp-arg]; s_push(v); v=s_pop(); tos+=v; tos=mem[tos];              break; // OPTIMIZE COMBO
	case IADDSET: v=mem[rp-arg]; s_push(v); v=s_pop(); tos+=v; v=s_pop(); mem[v]=s_pop(); break; // OPTIMIZE COMBO	
	// COMPARATORS - IMMEDIATE
	case LTI:    tos= tos<arg  ? 1:0;  break;
	case LEI:    tos= tos<=arg ? 1:0;  break;
	case GTI:    tos= tos>arg  ? 1:0;  break;
	case GEI:    tos= tos>=arg ? 1:0;  break;
	case EQI:    tos= tos==arg ? 1:0;  break;
	case NEI:    tos= tos!=arg ? 1:0;  break;
	// COMPARATORS - EXPERIMENTAL (NON DESTRUCTIVE)
	case XLT:    s_push(mem[sp]<tos  ? 1:0);  break;
	case XGT:    s_push(mem[sp]>tos  ? 1:0);  break;
	case XLE:    s_push(mem[sp]<=tos ? 1:0);  break;
	case XGE:    s_push(mem[sp]>=tos ? 1:0);  break;
	case XEQ:    s_push(mem[sp]==tos ? 1:0);  break;
	case XNE:    s_push(mem[sp]!=tos ? 1:0);  break;
	// COMPARATORS - ZERO -> REMOVE vs IMMEDIATE
	case NZ:     tos = tos!=0 ? 1:0;          break;
	case GTZ:    tos = tos>0  ? 1:0;          break;
	case EQZ:    tos = tos==0 ? 1:0;          break;
	case LTZ:    tos = tos<0  ? 1:0;          break;
}
