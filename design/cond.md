
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

"if" as syntax sugar ?
"fi / end / endif / end-if" as end-if ?


```forth

if distance 10 or-less then 10 times beep loop fi

if distance 10 or-less then
	10 times beep loop
else
	1 sec wait
fi
```

### Case - procedures

```forth
# case requires procedure

def age-range (n--) :age
    age 18 or-more  then adult         ret  do
    age 13 or-more  then teen          ret  do
    age  7 or-more  then gradeschooler ret  do
    age  4 or-more  then preschooler   ret  do
    age  1 or-more  then toddler       ret  do
                         baby
end
```

### Else

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

### Case - forth

```forth
( ala FORTH ) :age
    age 18 or-more  then  adult           else
    age 13 or-more  then  teen            else
    age  7 or-more  then  gradeschooler   else
    age  4 or-more  then  preschooler     else
    age  1 or-more  then  toddler         else
                          baby            
    do do do do do


vget.1 push.18 gte.0 jz.@[  call.@adult        goto.@[2 ]:
vget.1 push.13 gte.0 jz.@[  call.@teen         goto.@[2 ]:
vget.1 push.7  gte.0 jz.@[  call.@gradschooler goto.@[2 ]:
vget.1 push.4  gte.0 jz.@[  call.@prescholer   goto.@[2 ]:
vget.1 push.1  gte.0 jz.@[  call.@toddler      goto.@[2 ]:
                            call.@baby
]2: ]2: ]2: ]2: ]2:
```

### Case - goto

```forth
( goto ) :age
    age 18 or-more  then  adult           else
    age 13 or-more  then  teen            else
    age  7 or-more  then  gradeschooler   else
    age  4 or-more  then  preschooler     else
    age  1 or-more  then  toddler         else
                          baby            
    endcase::

vget.1 push.18 gte.0 jz.@[  call.@adult        goto.@endcase ]:
vget.1 push.13 gte.0 jz.@[  call.@teen         goto.@endcase ]:
vget.1 push.7  gte.0 jz.@[  call.@gradschooler goto.@endcase ]:
vget.1 push.4  gte.0 jz.@[  call.@prescholer   goto.@endcase ]:
vget.1 push.1  gte.0 jz.@[  call.@toddler      goto.@endcase ]:
                            call.@baby
endcase::
```

```

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
