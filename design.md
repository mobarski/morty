# Index

- [core](#core-vs-extension)
- [memory](#memory)
- [conditionals](#conditionals)
- [loops](#loops)
- [threading](#threading)
- [other](#other)

# Core vs Extension

The question is - should Morty aim for the "true" core or rather more practical one?

True core is just being Turing-complete - it's too close to the eso-langs.

The core should be based on practical minimalistic instruction sets: UXN, AnotherWorld VM, etc

In practice it's not about instruction count. All ALU instructions are trivial to implement, so ALU
should not be minimalistic. It's better to provide expressive power similar to C operators with small additions
to handle some low-cost CPU architectures (ie mulshr for CPUs without divide operation).


## Current core:

```
branch:  4 + 3 (loops)
stack:   10
mem:     8 
alu-ari: 7 + 2?
alu-bit: 6 + 1?
alu-cmp: 9 + 4?
io:      2
halt:    1  
nop:     1  

TOTAL:   51 + 7?
```

# Memory

This will change.

Layout:
- code
- global read-only
- global read-write
- return stack + data stack
- heap

```
def foo (--a)
    { 11 22 33 }
end

( --- COMPILES TO --- )
foo: (--a)
    goto.@[ #11 #22 #33 pusha.@] (better locality)
ret
( --- OR --- )
foo: (--a)
    pusha.@foo_data1
ret
foo_data_1: #11 #22 #33


{ 11 22 33 } $arr
{ 42 }       $the-answer  (TODO: special syntax?)
10 allot     $buffer
10 20 0 new-array $array

{ 11 22 33 } as arr
{ 42 }       as the-answer (TODO: special syntax?)
10 allot     as buffer
10 20 0 new-array as array

foo:
	arr 1 add get (22)
	the-answer add (64)
end

*5#0 -> #0 #0 #0 #0 #0

```

# Compiler:

The compiler is separated into several layers:
- language processor
- high level assembler
- peephole optimizer
- assembler
- linker?

# Function arguments

```

	def ack (mn--a) :n :m
		m 0 eq then n 1 add ret do
		n 0 eq then m 1 sub 1 ack ret do
		m 1 sub
			m n 1 sub ack
		ack
	end

	def ack | m n -- a |
		m 0 eq then n 1 add ret do
		n 0 eq then m 1 sub 1 ack ret do
		m 1 sub
			m n 1 sub ack
		ack
	end


	def ack ( m n -- a ) | m n |
		m 0 eq then n 1 add ret do
		n 0 eq then m 1 sub 1 ack ret do
		m 1 sub
			m n 1 sub ack
		ack
	end

	def ack | m n | (--a)
		m 0 eq then n 1 add ret do
		n 0 eq then m 1 sub 1 ack ret do
		m 1 sub
			m n 1 sub ack
		ack
	end

```

# Conditionals

Language without "else" is also acceptable!

Enclosing the block in [ & ] helps in code editors (highlight matching bracket)
but it creates some noise (any non letter looks noisy).

For the simple "if" case the ASM should be just one "jump-zero" instruction.

Lambda based "if" is acceptable but it has worse performance on the "fibo" benchmark:
- 10% longer for "lambda.@[ ... ]: " based
- 28% longer for "0 jz.@[ ...  push.@]" based
- 14% longer for "goto.@[ ...  push.@]" based

Maybe it can be optimized by mutating the cell before the block by the "if" cell?
It's hard with "lambda.@[" as there is no start of the block addr! So "lambda.@[" was replaced with "goto.@[" and "push.@]".

Forth like "if ... then" solution doesn't require code mutation - it can be compiled in one pass.

**DECISIONS**
1. Contitionals are based on words not brackets. 
2. In the simplest version only "if-then" is provided, "else" and "case" might be added later.
3. The words for conditionals are "if ... then" or "then ... do" TBD.

```forth
( exmaple from fibo.hla )
2 lt [ 1 ret ] if

( "]" is "qret.0 push.@]", so "if" must remove 2 previous tokens )
push.2 lt.0 goto.@[ push.1 ret.0 qret.0 push.@] ( tokens before if )
push.2 lt.0   jz.@[ push.1 ret.0 ]:             ( tokens after  if )
push.2 lt.0 goto.@[ push.1 ret.0 /jz]:          ( new stack effect - replace )

( "]" is "push.@]", so "if" must remove 1 previous tokens )
push.2 lt.0 goto.@[ push.1 ret.0 push.@] ( tokens before if )
push.2 lt.0   jz.@[ push.1 ret.0 ]:      ( tokens after  if )
push.2 lt.0 goto.@[ push.1 ret.0 /jz]:   ( new stack effect - replace )


( forth like if ... then )
2 lt if 1 ret then
2 lt then 1 ret do

push.2 lt.0 jz.@[ push.1 ret.0 push.@]   ( no replacement required )
```

## Sandbox

### New

```forth
distance 10 or-less then [ 10 times [ beep ] ]
distance 10 or-less then 10 times beep loop do
distance 10 or-less if 10 times beep loop then
distance 10 or-less if 10 times do beep loop then

distance 10 or-less then 
    10 times
        beep
    loop
do

distance 10 or-less then [
    10 times [
        beep
    ]
]

distance 10 or-less if
    10 times
        beep
    loop
then

distance 10 or-less if
    10 times do
        beep
    loop
then
```

### Old

```forth
distance 10 or-less then collision-warning end-then
distance 10 or-less then collision-warning end
distance 10 or-less if   collision-warning then       (Forth)
distance 10 or-less if   collision-warning end
distance 10 or-less if   collision-warning end-if

( with brackets )

distance 10 or-less [ collision-warning ] if          (Factor)
distance 10 or-less [ collision-warning ] then
distance 10 or-less then [ collision-warning ]
distance 10 or-less then [ collision-warning ] do

( mixed )
distance 10 or-less then[ collision-warning ]
distance 10 or-less   if[ collision-warning ]
```

```forth
age 18 or-more [ show-content ] [ show-restriction ] if-else

age 18 or-more if-else [ show-content ] [ show-restriction ] 

age 18 or-more then [ show-content ]
               else [ show-restriction ]
```

```forth
( ala FORTH ) :age
    age 18 or-more  if adult           else
    age 13 or-more  if teen            else
    age  7 or-more  if gradeschooler   else
    age  4 or-more  if preschooler     else
    age  1 or-more  if toddler         else
                       baby            
    then then then then then then

vget.1 push.18 gte.0 jz.@[  call.@adult        goto.@[2 ]:
vget.1 push.13 gte.0 jz.@[  call.@teen         goto.@[2 ]:
vget.1 push.7  gte.0 jz.@[  call.@gradschooler goto.@[2 ]:
vget.1 push.4  gte.0 jz.@[  call.@prescholer   goto.@[2 ]:
vget.1 push.1  gte.0 jz.@[  call.@toddler      goto.@[2 ]:
                            call.@baby
]2: ]2: ]2: ]2: ]2: ]2: 

( case requires procedure )

def age-range (n--) :age
    age 18 or-more  then adult         ret  end-then
    age 13 or-more  then teen          ret  end-then
    age  7 or-more  then gradeschooler ret  end-then
    age  4 or-more  then preschooler   ret  end-then
    age  1 or-more  then toddler       ret  end-then
                         baby
end

def age-range (n--) :age
    age 18 or-more  then[ adult         ret ]then
    age 13 or-more  then[ teen          ret ]then
    age  7 or-more  then[ gradeschooler ret ]then
    age  4 or-more  then[ preschooler   ret ]then
    age  1 or-more  then[ toddler       ret ]then
                          baby          ret
end


    age 18 or-more then [ adult         ] else
    age 13 or-more then [ teen          ] else
    age  7 or-more then [ gradeschooler ] else
    age  4 or-more then [ preschooler   ] else
    age  1 or-more then [ toddler       ] else
                        [ baby          ]
( push end of case onto R ??? )


def age-range (n--) :age
    age 18 or-more  [ adult         ret ] then
    age 13 or-more  [ teen          ret ] then
    age  7 or-more  [ gradeschooler ret ] then
    age  4 or-more  [ preschooler   ret ] then
    age  1 or-more  [ toddler       ret ] then
                      baby          ret
end

cnd [a] [b] if-else
if-else -> rot pick call

( "then" chenges the bahaviour of the next [ and ] )
    age 18 or-more  then [ adult ret ]
    age 13 or-more  then [ teen  ret ]

( "then[" chenges the bahaviour of the next ] ??? )
    age 18 or-more  then[ adult ret ]
    age 13 or-more  then[ teen  ret ]
    var.1 push.13 gt.0 jz.@[ call.@teen  ret.0 ]:
    
    [ teen ret ]
    push.0 jz.@[ call.@teen ret.0 push.@]

```

# Loops

## Performance

Measured on loops.hla with n=28.

| implementation            | times [ms]                         | mean | stdev |
| ------------------------: | ---------------------------------- | ---- | ----- |
|               frame + cnt | 1886,1895,1878,1890,1886,1900,1890 | 1889 |  7.04 |
|            no frame + cnt | 1939,1927,1938,1934,1950,1948,1975 | 1944 | 15.61 |
|         frame + from + to | 2010,2000,1995,2001,2083,2012,2045 | 2020 | 31.99 |
| frame + from + to + delta | 1999,1987,1996,1994,2073,2000,2067 | 2016 | 36.78 |

## Lang

Enclosing the block in [ & ] helps in code editors (matching bracket can be highlighted) and helps to distinguish loop start and end.
A similar effect can be achieved with a proper indention and/or loop keywords highlighting.

Block based loops require that loop setup and loop cleanup happens in the single keyword.

```forth
5 times 
    i dot
loop

5 times 
    i dot
repeat

10 2 1 for
    i dot
loop

( vs )

5 [ i dot ] repeat

10 2 1 [ i dot ] loop

( vs )

5 [
    i dot
] repeat

10 2 1 [
    i dot
] loop


```


Definite:
```forth

5 times [ 1 add ]

5 times [ 1 add ] loop

5 times 1 add loop

```

Indefinite:
```forth
loop [ ... ]

do [ ... ] loop

do [ ... ] repeat

for [ ... ] loop

loop [ ... 10 ge then [ break ] ... ]

```

```forth
5 times
    i print
loop

10 0 do
    i print
loop

10 0 2 for
    i print
loop

```

Mako
```forth
loop
    ...
while / until / again

10 for
    ... ( supports i and j loop indices )
next
```

Forth
```forth
limit index do
    ...
loop / +loop

begin
    ...
f until

begin
    ...
again

begin xx f while yyy repeat
```

## ASM

The loop requires setup code, repetition code and cleanup code.

Setup code creates loop frame on the return stack.
Cleanup code drops the loop frame from the return stack.
Loop frame enables "break" and "continue" instructions.
Loop frame should contain only one address so it can be created with one asm instruction.
The rules for performing "break" and "continue" must base on only one addres.
Continue should be jump @addr and break jump @addr+2.

```
(n) times.@]1 [1: (code) ]1: loop.@[1 rsub.2
(n) times.@[ 1 add loop.@]

```

# Threading

To benchmark new Threading Model following instructions must be implemented:
- **loops.mrh**: push, stor, vget, times, add, loop, call, ret, vminfo, (addi)
- **fibo.mrh**: stor, vget, push, lt, jz, ret, ((qret)), sub, call, add, vminfo, (lti), (reti), (subi), (subcall), (addret)

Following threading models are implemented:
- switch based threading
- indirect threading
- replicated switch threading

# Other

## File Extensions

| extension | alternative | info                       |
| --------- | ----------- | -------------------------- |
| .mra      | .moa        | Morty assembly             |
| .mrb      | .mob        | Morty binary executable    |
| .mrh      | .moh        | Morty high-level assembly  |
| .mro      | .moo        | Morty optimized executable |
| .mrp      | .mop        | Morty portable executable  |
| .mrt      | .mo         | Morty source code          |
