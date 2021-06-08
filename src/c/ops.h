
enum OPS {
	STOP,
	PUSH=1,DUP,DROP,SWAP,STOR=5,RTOS,
	ADD,MUL,DIV,SUB=10,
	AND,OR,XOR,NZ,
	CALL=15,RET=16,JZ=17,
	QCALL,QRET,
	GET=20,SET,ALLOT,
	MOD,SHL,SHR=25,USHR,INVERT,NEGATE,ABS,
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
	ADDI,MULI,
	LEI,LTI,GEI,GTI,EQI,NEI,
	IOGET,IOSET,
	VMINFO=255
};
