
enum OPS {
	STOP,
	NOP,PUSHA,
	PUSH,DUP,DROP,SWAP,STOR,RTOS,
	ADD,MUL,DIV,SUB,
	AND,OR,XOR,NZ,
	CALL,RET,JZ,
	QCALL,QRET,
	GET,SET,ALLOT,
	MOD,SHL,SHR,USHR,INVERT,NEGATE,ABS,
	LE,LT,GE,GT,EQ,NE,
	XLE,XLT,XGE,XGT,XEQ,XNE,
	ROT,OVER, UNROT, // rozwala
	VGET,VSET,RADD,
	EMIT,DOT,ECHO,
	IF,IFELSE,GOTO,
	LAMBDA,
	MIN,MAX,PICK,
	GTZ,EQZ,LTZ,
	TIMES,LOOP,FOR,RSUB,RGET,BEGIN,BREAK,CONTINUE,
	ADDI,MULI,ADDGET,ADDSET,IADDGET,IADDSET,
	LEI,LTI,GEI,GTI,EQI,NEI,
	IOGET,IOSET,
	VMINFO=255
};

char* OP_NAME[] = { "stop","nop","pusha","push","dup","drop","swap","stor","rtos","add","mul","div","sub","and","or","xor","nz","call","ret","jz","qcall","qret","get","set","allot","mod","shl","shr","ushr","invert","negate","abs","le","lt","ge","gt","eq","ne","xle","xlt","xge","xgt","xeq","xne","rot","over","unrot","vget","vset","radd","emit","dot","echo","if","ifelse","goto","lambda","min","max","pick","gtz","eqz","ltz","times","loop","for","rsub","rget","begin","break","continue","addi","muli","addget","addset","iaddget","iaddset","lei","lti","gei","gti","eqi","nei","ioget","ioset","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","vminfo" };
