# Index

- [core](#core-vs-extension)
- [conditionals](#conditionals)
- [loops](#loops)
- [threading](#threading)
- [other](#other)

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
It's hard with "lambda.@[" as there is no start of the block addr! So "lambda.@[" was replaced with "goto.@[" and "push.@]".

Forth like "if ... then" solution doesn't require code mutation - it can be compiled in one pass.


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

distance 10 or-less then [
    10 times [
        beep
    ]
]

distance 10 or-less then 
    10 times
        beep
    loop
do

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

## Benchmark

XG 6254 -> Intel Xeon Gold 6254 @ 3.10GHz  gcc 10.3.0
I5 4590 -> Intel Core I5 4590 @ 3.30GHz    

### loops.hla n=28 for loop

| variant     | machine | times                    | mean | stdev |
| ----------- | ------- | ------------------------ | ---- | ----- |
| switch      | XG 6254 | 3714,3711,3717,3765,3715 | 3724 |    23 |
| switch -O   | XG 6254 | 3056,3075,3108,3088,3077 | 3018 |    19 |
| indirect -O | XG 6254 | 2473,2470,2472,2473,2472 | 2472 |     1 |
| indirect    | XG 6254 | 2804,2912,2912,2946,2878 | 2890 |    54 |
| switch      | I5 4590 | 4254,4215,4081,4092,4187 | 4166 |    76 |
| switch -O   | I5 4590 | 3608,3517,3434,3444,3544 | 3509 |    72 |
| indirect -O | I5 4590 | 2688,2633,2651,2634,2640 | 2649 |    73 |
| indirect    | I5 4590 | 3397,3367,3351,3278,3351 | 3349 |    44 |

### loops.hla n=28 times loop

| variant     | machine | times                    | mean | stdev |
| ----------- | ------- | ------------------------ | ---- | ----- |
| switch      | XG 6254 | 1813,1807,1831,1810,1818 | 1816 |     9 |
| switch -O   | XG 6254 | 1210,1216,1219,1211,1220 | 1215 |     5 |
| indirect -O | XG 6254 | 947,948,943,947,949      |  947 |     2 |
| indirect    | XG 6254 | 1349,1363,1338,1346,1339 | 1347 |    10 |
| switch      | I5 4590 | 2084,2068,2020,1994,2047 | 2043 |    36 |
| switch -O   | I5 4590 | 1465,1445,1421,1419,1463 | 1443 |    22 |
| indirect -O | I5 4590 | 1460,1402,1363,1359,1371 | 1391 |    42 |
| indirect    | I5 4590 | 1595,1630,1596,1619,1594 | 1607 |    17 |

### fibo.hla n=38

| variant     | machine | times                    | mean | stdev |
| ----------- | ------- | ------------------------ | ---- | ----- |
| switch      | XG 6254 | 1901,1885,1924,1897,1896 | 1901 |    14 |
| switch -O   | XG 6254 | 1752,1753,1770,1725,1780 | 1756 |    21 |
| indirect -O | XG 6254 | 1923,1919,1923,1910,1969 | 1929 |    23 |
| indirect    | XG 6254 | 2056,2032,2048,2023,2035 | 2039 |    13 |
| switch      | I5 4590 | 2076,2062,2027,2068,2040 | 2055 |    20 |
| switch -O   | I5 4590 | 1921,1849,1909,1918,1913 | 1902 |    30 |
| indirect -O | I5 4590 | 2300,2296,2289,2241,2293 | 2284 |    24 |
| indirect    | I5 4590 | 2496,2456,2430,2415,2401 | 2440 |    38 |

### array.hla n=100000

| variant     | machine | times                    | mean | stdev |
| ----------- | ------- | ------------------------ | ---- | ----- |
| switch      | XG 6254 | 2183,2181,2189,2193,2196 | 2188 |     6 |
| switch -O   | XG 6254 | 1480,1454,1465,1455,1461 | 1463 |    11 |
| indirect -O | XG 6254 | 1220,1214,1217,1216,1221 | 1218 |     3 |
| indirect    | XG 6254 | 1446,1446,1445,1446,1460 | 1449 |     6 |
| switch      | I5 4590 | 1991,2174,2009,2109,2050 | 2067 |    75 |
| switch -O   | I5 4590 | 1251,1207,1192,1191,1258 | 1220 |    32 |
| indirect -O | I5 4590 | 1101,1181,1098,1092,1152 | 1125 |    40 |
| indirect    | I5 4590 | 2156,2074,1996,1989,1997 | 2042 |    72 |

### array.hla n=100000 no iaddget, no iaddset

| variant     | machine | times                    | mean | stdev |
| ----------- | ------- | ------------------------ | ---- | ----- |
| switch -O   | I5 4590 | 1703,1687,1683,1689,1698 | 1692 |     8 |
| indirect -O | I5 4590 | 1442,1441,1442,1441,1491 | 1451 |    22 |

### sieve.hla n=5000

| variant     | machine | times                    | mean | stdev |
| ----------- | ------- | ------------------------ | ---- | ----- |
| switch      | XG 6254 | 1815,1813,1812,1813,1812 | 1813 |     1 |
| switch -O   | XG 6254 | 1203,1178,1196,1181,1180 | 1188 |    11 |
| indirect -O | XG 6254 | 1044,1063,1035,1034,1035 | 1042 |    12 |
| indirect    | XG 6254 | 1348,1347,1350,1347,1347 | 1348 |     1 |
| switch      | I5 4590 | 2056,2048,2028,2029,2115 | 2055 |    36 |
| switch -O   | I5 4590 | 1360,1355,1332,1336,1409 | 1358 |    31 |
| indirect -O | I5 4590 | 1264,1269,1260,1259,1316 | 1274 |    24 |
| indirect    | I5 4590 | 1751,1738,1746,1740,1801 | 1755 |    26 |
| python3     | I5 4590 | 8167,8153,8335,8458,8180 | 8259 |   134 |
| gcc         | I5 4590 | 422,424,419,422,421      |  422 |     2 |
| gcc -O3     | I5 4590 | 83,83,83,85,85           |   84 |     1 |


# Other

## File Extensions

| extension | info                       |
| --------- | -------------------------- |
| .mra      | Morty assembly             |
| .mrb      | Morty binary executable    |
| .mrh      | Morty high-level assembly  |
| .mro      | Morty optimized executable |
| .mrp      | Morty portable executable  |
| .mrt      | Morty source code          |
