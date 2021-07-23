# Transpiler

## JS Patterns

### Branching
CALL -> fun()
RET  -> return
JZ   -> ??? <------------------------------------
GOTO -> ??? <------------------------------------
HALT -> break outer_loop
NOP  -> /*NOP*/

### Loops
TIMES    -> for(i=0; i<x; i++) {
FOR      -> for(i=i0; i<hi; i+=step) {
begin    -> for(;;) {
break    -> break
continue -> continue

### Return Stack
STOR -> mem[++rp] = mem[sp--]
RTOS -> mem[++sp] = mem[rp--]
RGET -> mem[++sp] = mem[rp-arg]
VGET -> mem[++sp] = mem[fp+arg]
VSET -> mem[fp+arg] = mem[sp--]

### Data Stack
SWAP  -> v=mem[sp]; mem[sp]=mem[sp-1]; mem[sp-1]=v
PUSH  -> mem[++sp]=arg
PUSHA -> mem[++sp]=arg
DUP   -> mem[++sp]=mem[sp-1]
DROP  -> sp--
OVER  -> mem[++sp]=mem[sp-2]

### ALU
MUL -> mem[--sp] *= mem[sp+1]
DIV -> mem[--sp] /= mem[sp+1]
ADD -> mem[--sp] += mem[sp+1]
SUB -> mem[--sp] -= mem[sp+1]
...
INVERT -> mem[sp] = ~mem[sp]
NEGATE -> mem[sp] = -mem[sp]
ABS    -> mem[sp] = mem[sp]<0 ? -mem[sp] : mem[sp]

### Comparators - main
LT -> mem[--sp] = mem[sp] <  mem[sp+1] ? 1 : 0
LE -> mem[--sp] = mem[sp] <= mem[sp+1] ? 1 : 0
GT -> mem[--sp] = mem[sp] >  mem[sp+1] ? 1 : 0
GE -> mem[--sp] = mem[sp] >= mem[sp+1] ? 1 : 0
EQ -> mem[--sp] = mem[sp] == mem[sp+1] ? 1 : 0
NE -> mem[--sp] = mem[sp] != mem[sp+1] ? 1 : 0

### Comparators - aux
MIN  -> mem[--sp] = mem[sp]<mem[sp+1] ? mem[sp] : mem[sp+1]
MAX  -> mem[--sp] = mem[sp]>mem[sp+1] ? mem[sp] : mem[sp+1]
PICK -> ???

### Memory
GET   -> mem[sp] = mem[mem[sp]]
SET   -> mem[sp] = mem[sp-1]; sp-=2
GGET  -> mem[++sp] = mem[gp+arg]
GSET  -> ???
ALLOT -> v=mem[sp]; mem[sp] = hp; hp+=v
