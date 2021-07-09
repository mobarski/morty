function ms_clock() {
	return Math.floor(Date.now())
}

vm = {}
vm.mem = new Array(2048)
vm.mem.fill(0)
vm.fp = 0
vm.ip = 0
vm.rp = 0
vm.sp = 0
vm.gp = 0
vm.hp = 0
vm.ts_vminfo = 0

cfg = {}
cfg.data_stack_size = 100
cfg.return_stack_size = 100
cfg.globals_cnt = 100

// --- OPCODES (.js) ---
const HALT=0, NOP=1, PUSHA=2, PUSH=3, DUP=4, DROP=5, SWAP=6, STOR=7, RTOS=8, ADD=9
const MUL=10, DIV=11, SUB=12, AND=13, OR=14, XOR=15, NZ=16, CALL=17, RET=18, JZ=19
const QCALL=20, QRET=21, GET=22, SET=23, ALLOT=24, MOD=25, SHL=26, SHR=27, USHR=28, INVERT=29
const NEGATE=30, ABS=31, LE=32, LT=33, GE=34, GT=35, EQ=36, NE=37, XLE=38, XLT=39
const XGE=40, XGT=41, XEQ=42, XNE=43, ROT=44, OVER=45, UNROT=46, VGET=47, VSET=48, RADD=49
const GGET=50, GSET=51, EMIT=52, DOT=53, ECHO=54, IF=55, IFELSE=56, GOTO=57, LAMBDA=58, MIN=59
const MAX=60, PICK=61, GTZ=62, EQZ=63, LTZ=64, TIMES=65, LOOP=66, FOR=67, RSUB=68, RGET=69
const BEGIN=70, BREAK=71, CONTINUE=72, ADDI=73, MULI=74, ADDGET=75, ADDSET=76, IADDGET=77, IADDSET=78, LEI=79
const LTI=80, GEI=81, GTI=82, EQI=83, NEI=84, IOGET=85, IOSET=86, VMINFO=255
// ----------------------------------------------------------------------------

function boot(code) {
	vm.ip = cfg.ip = 0
	for (i in code) {
		vm.mem[i] = code[i]
		//console.log(`boot code[${i}] -> ${code[i]} mem[${i}] -> ${vm.mem[i]}`) // XXX
	}
	pos = 0
	pos += code.length + 10
	vm.sp = cfg.sp = pos; pos += cfg.data_stack_size
	vm.fp = cfg.fp = pos
	vm.rp = cfg.rp = pos; pos += cfg.return_stack_size
	vm.gp = cfg.gp = pos; pos += cfg.globals_cnt
	vm.hp = cfg.hp = pos
}

function s_pop() { return vm.mem[vm.sp--] }
function r_pop() { return vm.mem[vm.rp--] }
function s_push(v) { vm.mem[++vm.sp]=v }
function r_push(v) { vm.mem[++vm.rp]=v }

function run() {
	mem = vm.mem
	vm.ts_vminfo = ms_clock()
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
			// COMPARATORS - AUX
			case MIN:    v=s_pop(); mem[vm.sp]=mem[vm.sp]<v ? mem[vm.sp]:v;    break;
			case MAX:    v=s_pop(); mem[vm.sp]=mem[vm.sp]>v ? mem[vm.sp]:v;    break;
			case PICK:   mem[vm.sp-2]=mem[vm.sp] ? mem[sp-2]:mem[sp-1]; sp-=2; break;
			// MEMORY
			case GET:    mem[vm.sp] = mem[mem[vm.sp]];        break;
			case SET:    v=s_pop(); mem[v]=s_pop();           break;
			case GGET:   v=mem[vm.gp+arg]; s_push(mem[v]);    break; // global variables
			case GSET:   v=s_pop(); mem[mem[vm.gp+arg]]=v;    break; // global variables
			case ALLOT:  v=s_pop(); s_push(vm.hp); vm.hp+=v;  break;
			// DEBUG
			case VMINFO:
				console.log(`TOS:${mem[vm.sp]}\tIP:${vm.ip}\tSD:${vm.sp-cfg.sp}\tRD:${vm.rp-cfg.rp}\tHD:${vm.hp-cfg.hp}\tdt:${ms_clock()-vm.ts_vminfo} ms`)
				vm.ts_vminfo = ms_clock()
				break;
			// PRIMITIVE OUTPUT
			case EMIT:   v=s_pop(); console.log(String.fromCharCode(v));  break; // TODO: screen -> textbox
			case DOT:    v=s_pop(); console.log(v);                       break; // TODO: screen -> textbox
			// IO
			case IOGET:  io_get(arg);  break;
			case IOSET:  io_set(arg);  break;
		}
	}	
	console.log('HALT')
}

// ---[ IO ]-------------------------------------------------------------------

function io_get(dev) {
	switch (dev) {
		case 1: screen_get(); break;
	}
}

function io_set(dev) {
	switch (dev) {
		case 1: screen_set(); break;
	}
}

// ---[ SCREEN ]---------------------------------------------------------------

function screen_get() {
	fun = s_pop()
	switch (fun) {
		case 3: s_push(scr.c);                     break; // scr-get-color
		case 4: s_push(scr.i);                     break; // scr-get-i
		case 5: s_push(0);                         break; // TODO: scr-get (pixel-color)
		case 6: s_push(scr.i % scr.w);             break; // scr-get-x
		case 7: s_push(Math.floor(scr.i / scr.h)); break; // scr-get-y
	}
}

function screen_set() {
	fun = s_pop()
	switch (fun) {
		case 1: flip();             break; // scr-flip
		case 2: cls(scr.c);         break; // scr-cls
		case 3: scr.c = s_pop();    break; // scr-color
		case 4: scr.i = s_pop();    break; // scr-set-i
		case 5: pset(scr.i++);      break; // scr-put
		case 6: // scr-set-x
			x = s_pop()
			x0 = scr.i % scr.w
			scr.i += x-x0
			break
		case 7: // scr-set-y
			y = s_pop()
			y0 = Math.floor(scr.i / scr.h)
			scr.i += (y-y0)*scr.h
			break
	}
}
