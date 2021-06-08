# Index

- [core](#core-vs-extension)
- [conditionals](#conditionals)
- [loops](#loops)

# Core vs Extension

The question is - should Morty aim for the "true" core or rather more practical one?

True core is just being Turing-complete - it's too close to the eso-langs.

The core should be based on practical minimalistic instruction sets: UXN, AnotherWorld VM, etc


## Current core:

```
branch:  6 + 2 for loops  
stack:   8  
mem:     2 + 1?  
frame:   2  
alu-ari: 5 + 1?  
alu-bit: 5 + 1?  
alu-cmp: 5 + 2?  
io:      2  
halt:    1  
nop:     1  

TOTAL:   37 + 5? + 2(halt,nop)
```

# Conditionals

Language without "else" is also acceptable!

Enclosing the block in [ & ] helps in code editors (highlight matching bracket).

For the simple "if" case the ASM should be just one "jump-zero" instruction.

Lambda based "if" is acceptable but it has worse performance on the "fibo" benchmark:
- 10% longer for "lambda.@[ ... ]: " based
- 28% longer for "0 jz.@[ ...  push.@]" based
- 14% longer for "goto.@[ ...  push.@]" based

Maybe it can be optimized by mutating the cell before the block by the "if" cell?
It's hard with "lambda.@[" as there is no start of the block addr!

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
```

## Sandbox

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
f untill

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

