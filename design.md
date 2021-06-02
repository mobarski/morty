# Conditionals

Language without "else" is also acceptable!

```
distance 10 or-less [ collision-warning ] if

distance 10 or-less [ collision-warning ] then

distance 10 or-less then collision-warning end-then

distance 10 or-less then [ collision-warning ]

distance 10 or-less then [ collision-warning ] do
```

```
age 18 or-more [ show-content ] [ show-restriction ] if-else

age 18 or-more if-else [ show-content ] [ show-restriction ] 

age 18 or-more then [ show-content ]
               else [ show-restriction ]


```

# Loops

## Lang

Definite:
```

5 times [ 1 add ]

5 times [ 1 add ] loop

5 times 1 add loop

```

Indefinite:
```
loop [ ... ]

do [ ... ] loop

do [ ... ] repeat

for [ ... ] loop

loop [ ... 10 ge then [ break ] ... ]

```

```
5 times
	i print
loop

0 10 do
	i print
loop

0 10 2 for
	i print
loop

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
```
