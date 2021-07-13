# IO

IO subsystem uses vectored exectution.

Selected Approach: B+D

## Get+Set vs Cntl

### Option A:
 
IO operations are separated into two categories - get and set.
IO subsytem consists of two vectors.

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

## Option F

Functions can push/pop 0 or 1 value

## Option G

Function always push/pop 1 value 

