vm = {}
vm.mem = new Array(2048)
vm.mem.fill(0)
vm.fp = 0
vm.ip = 0
vm.rp = 0
vm.sp = 0
vm.gp = 0
vm.hp = 0

cfg = {}
cfg.data_stack_size = 100
cfg.return_stack_size = 100
cfg.globals_cnt = 100

// --- OPCODES ---
const HALT=0
const NOP=1
const PUSHA=2
const PUSH=3
const DUP=4
const DROP=5
const SWAP=6
const STOR=7
const RTOS=8
const ADD=9
const MUL=10
const DIV=11
const SUB=12
const AND=13
const OR=14
const XOR=15
const NZ=16
const CALL=17
const RET=18
const JZ=19
const QCALL=20
const QRET=21
const GET=22
const SET=23
const ALLOT=24
const MOD=25
const SHL=26
const SHR=27
const USHR=28
const INVERT=29
const NEGATE=30
const ABS=31
const LE=32
const LT=33
const GE=34
const GT=35
const EQ=36
const NE=37
const XLE=38
const XLT=39
const XGE=40
const XGT=41
const XEQ=42
const XNE=43
const ROT=44
const OVER=45
const UNROT=46
const VGET=47
const VSET=48
const RADD=49
const GGET=50
const GSET=51
const EMIT=52
const DOT=53
const ECHO=54
const IF=55
const IFELSE=56
const GOTO=57
const LAMBDA=58
const MIN=59
const MAX=60
const PICK=61
const GTZ=62
const EQZ=63
const LTZ=64
const TIMES=65
const LOOP=66
const FOR=67
const RSUB=68
const RGET=69
const BEGIN=70
const BREAK=71
const CONTINUE=72
const ADDI=73
const MULI=74
const ADDGET=75
const ADDSET=76
const IADDGET=77
const IADDSET=78
const LEI=79
const LTI=80
const GEI=81
const GTI=82
const EQI=83
const NEI=84
const IOGET=85
const IOSET=86
const VMINFO=255

// ----------------------------------------------------------------------------

function boot(code) {
	pos = 0
	vm.ip = pos
	for (i in code) {
		vm.mem[pos+i] = code[i]
	}
	pos += code.length + 10
	vm.sp = pos; pos += cfg.data_stack_size
	vm.fp = pos
	vm.rs = pos; pos += cfg.return_stack_size
	vm.gp = pos; pos += cfg.globals_cnt
	vm.hp = pos
}

function s_pop() { return vm.mem[vm.sp--] }
function r_pop() { return vm.mem[vm.rp--] }
function s_push(v) { vm.mem[++vm.sp]=v }
function r_push(v) { vm.mem[++vm.rp]=v }

function run() {
	mem = vm.mem
	outer_loop:
	for (i=0; i<100; i++) { // TODO: replace with while (1)
		op = mem[vm.ip]
		arg = mem[vm.ip+1]
		vm.ip += 2
		//console.log("ip =",vm.ip,"op =",op) // XXX
		switch (op) {
			// BRANCHING
			case CALL: r_push(vm.fp); r_push(vm.ip); vm.ip=arg; vm.fp=vm.rp; break;
			case RET:  vm.ip=mem[vm.fp]; vm.rp=vm.fp-2; vm.fp=mem[vm.fp-1];  break;
			case JZ:   v=s_pop(); if (v==0) vm.ip=arg;                       break;
			case GOTO: vm.ip=arg;                                            break;
			case HALT: break outer_loop;                                     break;
			case NOP:                                                        break;
			// RETURN STACK
			case STOR:   v=s_pop(); r_push(v);          break;
			case RTOS:   v=r_pop(); s_push(v);          break;
			case RGET:   v=mem[vm.rp-arg]; s_push(v);   break; // loop variables
			case VGET:   v=mem[vm.fp+arg]; s_push(v);   break; // local variables
			case VSET:   v=s_pop(); mem[vm.fp+arg]=v;   break; // local variables
			// DATA STACK
			case SWAP:   v=mem[vm.sp]; mem[vm.sp]=mem[vm.sp-1]; mem[vm.sp-1]=v; break;
			case PUSH:   s_push(arg);                                           break;
			case PUSHA:  s_push(arg);                                           break;
			case DUP:    s_push(mem[vm.sp]);                                    break;
			case DROP:   v=s_pop();                                             break;
			case OVER:   s_push(mem[sp-1]);                                     break;
			// ALU
			case MUL:    v=s_pop(); mem[vm.sp] *= v;                        break;
			case DIV:    v=s_pop(); mem[vm.sp] /= v;                        break; // DIV tends to be slow on microcontrollers
			case ADD:    v=s_pop(); mem[vm.sp] += v;                        break;
			case SUB:    v=s_pop(); mem[vm.sp] -= v;                        break;
			case AND:    v=s_pop(); mem[vm.sp] &= v;                        break;
			case OR:     v=s_pop(); mem[vm.sp] |= v;                        break;
			case XOR:    v=s_pop(); mem[vm.sp] ^= v;                        break;
			case MOD:    v=s_pop(); mem[vm.sp] %= v;                        break;
			case SHL:    v=s_pop(); mem[vm.sp] <<= v;                       break;
			case SHR:    v=s_pop(); mem[vm.sp] >>= v;                       break;
			case INVERT: mem[vm.sp]= ~mem[vm.sp];                           break; 
			case NEGATE: mem[vm.sp]= -mem[vm.sp];                           break; 
			case ABS:    mem[vm.sp]= mem[vm.sp]<0 ? -mem[vm.sp]:mem[vm.sp]; break;
			// COMPARATORS - MAIN
			case LT:     v=s_pop(); mem[vm.sp]=mem[vm.sp]<v  ? 1:0;  break;
			case LE:     v=s_pop(); mem[vm.sp]=mem[vm.sp]<=v ? 1:0;  break;
			case GT:     v=s_pop(); mem[vm.sp]=mem[vm.sp]>v  ? 1:0;  break;
			case GE:     v=s_pop(); mem[vm.sp]=mem[vm.sp]>=v ? 1:0;  break;
			case EQ:     v=s_pop(); mem[vm.sp]=mem[vm.sp]==v ? 1:0;  break;
			case NE:     v=s_pop(); mem[vm.sp]=mem[vm.sp]!=v ? 1:0;  break;
		}
	}	
	console.log('HALT')
}

boot([1,0,0,0])
run()
console.log('DONE')


