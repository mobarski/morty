op  = mem[ip++];
arg = mem[ip++];
switch(op) {
	// BRANCHING
	case CALL:   r_push(fp); r_push(ip); ip=arg; fp=rp; break; // TEST ME
	case RET:    ip=mem[fp]; rp=fp-2; fp=mem[fp-1];     break; // TEST ME
	case QCALL:  r_push(ip); ip=s_pop();                break;
	case QRET:   ip = r_pop();                          break;
	case JZ:     v=s_pop();  if (v==0) ip=arg;          break;
	case IF:     v=s_pop(); v2=s_pop(); if(v2) { r_push(ip); ip=v; };       break; // TEST ME
	case IFELSE: v=s_pop(); v2=s_pop(); v3=s_pop(); r_push(ip); ip=v3?v2:v; break; // TEST ME
	case LAMBDA: s_push(ip); ip=arg;                    break; // TODO: RENAME
	case GOTO:   ip=arg;                                break;
	case STOP:   goto stop;                             break;
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
	case OVER:  s_push(mem[sp]);               break;
	case ROT:   v=mem[sp-1]; mem[sp-1]=mem[sp]; mem[sp]=tos; tos=v; break;
	case UNROT: v=tos; tos=mem[sp]; mem[sp]=mem[sp-1]; mem[sp-1]=v; break;
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
	// COMPARATORS
	case LT:    v=s_pop(); tos= tos<v  ? 1:0;  break; // NOT CORE
	case LE:    v=s_pop(); tos= tos<=v ? 1:0;  break; // NOT CORE
	case GT:    v=s_pop(); tos= tos>v  ? 1:0;  break; // NOT CORE
	case GE:    v=s_pop(); tos= tos>=v ? 1:0;  break; // NOT CORE
	case EQ:    v=s_pop(); tos= tos==v ? 1:0;  break; // NOT CORE
	case NE:    v=s_pop(); tos= tos!=v ? 1:0;  break; // NOT CORE
	case NZ:    tos = tos!=0 ? 1:0;            break;
	case MIN:   v=s_pop(); tos=tos<v ? tos:v;  break; // NOT CORE
	case MAX:   v=s_pop(); tos=tos>v ? tos:v;  break; // NOT CORE
	case PICK:  tos= tos ? mem[sp-1]:mem[sp]; sp-=2; break; // NOT CORE NEW
	// MEMORY
	case GET:   tos = mem[tos];                break;
	case SET:   v=s_pop(); mem[v]=s_pop();     break;
	case ALLOT: v=s_pop(); s_push(dp); dp+=v;  break;
	// DEBUG
	case VMINFO:
		printf("T:%d\tSP:%d\tRP:%d\tFP:%d\tIP:%d\tDP:%d\tSDMX:%d\tRDMX:%d\tDDMX:%d\tOLI:%d\tILC:%d\tdt:%d ms \n",tos,sp,rp,fp,ip,dp,sd_max,rd_max,dd_max,oli,ilc,ms_clock()-ts_vminfo);
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