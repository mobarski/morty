#define REPL_SWITCH_BODY { \
case STOP: goto OP_STOP; \
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
case LE: goto OP_LE; \
case LT: goto OP_LT; \
case GE: goto OP_GE; \
case GT: goto OP_GT; \
case EQ: goto OP_EQ; \
case NE: goto OP_NE; \
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
case VMINFO: goto OP_VMINFO; \
}

#define NEXT op=mem[ip++]; arg=mem[ip++]; switch(op) REPL_SWITCH_BODY
#define JUMP op=mem[ip++]; arg=mem[ip++]; switch(op) REPL_SWITCH_BODY
