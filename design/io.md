# IO

IO subsystem uses vectored exectution.

Selected Approach: B+D


## Get+Set vs Cntl

### Option A:
 
IO operations are separated into three categories - get,set,exec
Get/Set changes virtual device memory.
Exec runs virtual device function.

Mimics normal memory.


### Option B:

No distinction between get and set operations.
IO subsytem is a single vector. 


## Arg vs Arg+Stack

### Option C:

Device ID and Function ID is encoded into Arg

Better performance.

### Option D:

Device ID is encoded into Arg. Function ID is passed on the stack.

Better modularity.


## Function Arguments

## Option E

Functions have access to the stack and can push/pop as many values as they want.

Most efficient extensions.

## Option F

Functions can push/pop 0 or 1 value

## Option G

Function always push/pop 1 value 

Most intuitive.

## Sandbox

```
# Option A+D - ioget+ioset + funid on stack

	const I 4
	I ioget.screen -- <screen.I -- I -- <I
	I ioset.screen -- >screen.I -- :I -- $I -- >I
	
# Option B+D - io + funid on stack

	const IGET 4
	const ISET 5
	
	IGET io.screen
	ISET io.screen

# Option B+C - io (extension) + dev+funid in code

	const SCR-IGET 14
	const SCR-ISET 15
	
	io.SCR-IGET -- ext.SCR-IGET
	io.SCR-ISET -- ext.SCR-ISET

```

