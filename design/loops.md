
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

Block based loops require that loop setup and loop cleanup happens in the single keyword -> BAD.

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

## Sandbox

### New

```forth

	10 times
		...
	loop


	for 0 10 1 do
		...
	loop
	OR
	0 10 1 for
		...
	loop

	0 :i
	while i 10 lt do
		i 4 add :i
	loop

	while x 5 lt do
		...
	loop
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

	while x 5 lt then
		...
	loop
	->
	while.@[ vget.1 push.5 lt.0 breakzero.0
		...
	loop.@]

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
