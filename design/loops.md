
# Loops

## Loop frame variants

1. val                     - counting down to 0
2. addr  val               - counting down to 0
3. addr  from  val         - counting down to 0
4. addr  target val        - counting up to target
5. addr  step  target  val - counting up to target with step >= 1
6. addr  step  val         - counting down to 0 with step >= 1
7. addr  step  from  val   - counting down to 0 with step >= 1
8. addr                    - indefinite loop with break/continue support

## Performance

Measured on loops.hla with n=28.

| frame variant            | times [ms]                         | mean | stdev |
| -----------------------: | ---------------------------------- | ---- | ----- |
|                      val | 1886,1895,1878,1890,1886,1900,1890 | 1889 |  7.04 |
|                 addr val | 1939,1927,1938,1934,1950,1948,1975 | 1944 | 15.61 |
|            addr from val | 2010,2000,1995,2001,2083,2012,2045 | 2020 | 31.99 |
|  addr  step  target  val | 1999,1987,1996,1994,2073,2000,2067 | 2016 | 36.78 |

While the performance of 4 item frame is only 5% worse than the simplest case it requires 4 times more return stack cells.

## Lang

Enclosing the block in [ & ] helps in code editors (matching bracket can be highlighted) and helps to distinguish loop start and end.
A similar effect can be achieved with a proper indention and/or loop keywords highlighting.

Block based loops require that loop setup and loop cleanup happens in the single keyword -> BAD.

## ASM

The loop requires setup code, repetition code and cleanup code.

Setup code creates loop frame on the return stack.
Cleanup code drops the loop frame from the return stack.
Loop frame enables "break" and "continue" instructions.
Loop frame should contain only one address so it can be created with one asm instruction.
The rules for performing "break" and "continue" must base on only one addres.
Continue should be jump @addr and break jump @addr+2.

Break and continue don't have to require addr in the frame - they can be handled with `goto` and reusable labels:

```
5 times ... break ... loop
->
5 times ... goto.@>brk ... loop brk::

5 times ... continue ... loop
->
5 times ... goto.@>cnt ... cnt:: loop
```

Ideas below will not work as they don't handle nested loops.
```
**NEW** Idea for *break* without addr in the frame - new tokens for the linker:
- `@>>` which is set to the address following next `@]`.
- `@>` which is set to the address before next `@]`.
**PROBLEM** - not only loops use `@]` (conditionals, arrays, etc)
**SOLUTION** - use other markers than `@]` and use them after loop:
- `@>break` + `loop.@] break::`
where `::` denotes non-distinct label, for which we collect all positions
`@>name` will use address of the next occurence (based on current position)
`@<name` will use address of the previous occurence (based on current position) 
**IDEA** we can gather all occurences of labels (not only ending with `::`) and the standard reference `@name` will point to the greatest address. This might enable something similar to "monkey patching".
**ANOTHER IDEA** - loops use `@]! instead of `@]` and `@>` and `@>>` will seek just that - no other changes are neccessary.
```

```
(n) times.@]1 [1: (code) ]1: loop.@[1 rsub.2
(n) times.@[ 1 add loop.@]

```

## Sandbox

### While

```
while x 5 lt do ... loop
->
while.@[ x 5 lt breakzero.0 ... loop.@]

```

### New

```forth

10 times ... loop

for 0 10 1 do ... loop

0 10 1 for ... loop

0 :i
while i 10 lt do
    i 4 add :i
loop

while x 5 lt do ... loop
# HLA -> 0 0 0 loop-frame  [: x 5 lt jnz.@[ break ]: ... jump.@]

always
    if x 5 lt then break do
loop


# optional
repeat
    ...
until x 5 lt loop
OR 
repeat
    ...
x 5 lt until

```

### Old

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


### Definite
```forth

5 times [ 1 add ]

5 times [ 1 add ] loop

5 times 1 add loop

```

### Indefinite
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

## Mako
```forth
loop
    ...
while / until / again

10 for
    ... ( supports i and j loop indices )
next
```

## Forth
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
