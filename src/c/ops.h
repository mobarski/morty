
enum OPS {
	STOP,
	PUSH=1,DUP,DROP,SWAP,STOR=5,RTOS,
	ADD,MUL,DIV,SUB=10,
	AND,OR,XOR,NZ,
	CALL=15,RET=16,JZ=17,
	QCALL,QRET,
	GET=20,SET,ALLOT,
	MOD,SHL,SHR=25,USHR,
	LE,LT,GE,GT,EQ,NE,
	ROT,OVER, //UNROT, // rozwala
	VGET,VSET,RADD,
	EMIT,DOT,ECHO,
	IF,IFELSE,GOTO,
	LAMBDA
	VMINFO=255
};
