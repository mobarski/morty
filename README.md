# Morty

TODO

## Status

This is pre-alpha work in progress!

Status:
- C version of the VM is fully functional.
- The assembler is fully functional BUT error detection and error reporting is very primitive.
- Work on the compiler is not yet started as the language is still in a very early phase.

The system is now able to compile VM ASM and to execute it.
It's ready for running experiments "how feature X should be compiled and how it affects the performance". 

First benchmarks are looking good - VM achieves 0.7GHz on a 3.5GHz machine. 

## Index

- [language](#morty-programming-language)
- [vm](#morty-vitual-machine)
- [assembler](#morty-vm-assembler)
- [kanban](#kanban)
- [references](#references)

## Idea

Why:
- permacomputing
- internet of things
- stack VM / concatenative language experimentation

Target usage:
- simple game/utility dev (demakes, fantasy consoles/computers)
- education
- robotics
- simple backend services

Design principles:
- easy to implement (close to the stack machine like Forth)
- performant
- easy to learn (like Go)
- easy to read (like Python or Logo; named arguments, dot properties, less symbols)

Target VM implementations:
- C (most performant, portable with recompilation)
- JS (portable, GUI)
- Python (portable)
- MicroPython (portable - microcontrolers)
- Go (performant backend, portable with recompilation)

## Morty Philosophy

1. Start from scratch.
2. Add parts one by one observing how they interact.
3. Remove parts that can be cheaply built using other parts.
4. Keep parts that are expensive to build but make building other parts easier.
5. A state should be achieved where removing any single part should have drastic consequences.
6. This is the system's core.
7. Now add parts that will make the system more friendly / usable / performant.
8. Try to cover 80% of requirements with 20% of features.

# Morty Programming Language

Morty language is similar to Morty VM instruction set.
Almost all VM instructions can be used directly.
Morty allows the usage of "goto" for the same reasons it is still in C and GO: it's needed for some special cases (state machines / nested loop escape), but the jump can be made only inside the current procedure.


## Language Examples

```
def fib (n--f) :n
	n 2 lt [ 1 ret ] if
	n 2 sub fib (f2)
	n 1 sub fib (f2 f1) add (f)
end
```

```
def total-energy (m v h -- e) :h :v :m
    m v kinetic-energy   (ek)
    m h potential-energy (ek ep) add
end

def kinetic_energy (m v -- e)
    dup (m v v) mul mul
end

def potential-energy (m h -- e)
    g (m h g) mul mul
end
```

## Named local variables

Morty allows capturing values into named local variables.
To capture a value prefix target variable name with a colon (ie :my-var).
Use the variable name to push its value onto the stack.

```
( y = a*x*x + b*x + c )

def poly2 (x a b c -- y) :c :b :a :x (capture params)
    x x mul a mul (axx)
    x b mul (axx bx) add
    c add
end
```

Local variables are stored on the return stack.
On function end or early return they will be automatically discarded.

## Lambda functions

```
21 [ dup add ] call
```

Lambda functions are using the stack frame of the parent.
They are not closures and should not be shared if they are using parent's local variables.

## Conditionals

WARNING: this may change

Conditionals are based on lambda functions.

```
distance 10 or-less [ collision-warning ] if
age 18 or-more [ show-content ] [ show-restriction ] if-else
```

## Loops

WARNING: this may change

Loops are counted up. In every loop "break" and "continue" can be used.

Counted loops:
```forth
( loop 5 times from 0 to 4 )
5 times
    i dot
loop

( loop to 10 from 2 with step 1 )
10 2 1 for
    i dot
loop
```

Indefinite loops:
```
```

## Structures

TODO

```
struct point   .x .y   
struct circle  .r .x .y

def dist-to-circle (p c -- d) :c:circle :p:point 
    p.x c.x sub dup mul
    p.y c.y sub dup mul
    add sqrt               (distance to the center)
    c.r sub
end
```

Struct definition cannot be longer than 1 line.
This is on purpose to keep the structures simple.
Field names are prefixed with a dot to make them easier to grep and highlight.


## Macros

TODO

```
macro rot  >R swap <R swap
```

Macros cannot be longer than 1 line.
This is on purpose to keep the macros simple.

## Global variables

TODO

## Arrays

TODO

## Strings

TODO


# Morty Vitual Machine

## VM Architecture

VM:
- F - frame pointer
- I - instruction pointer
- R - return stack pointer
- S - (data) stack pointer
- T - top of stack register
- H - heap pointer
- MEM - main memory

## Instruction encoding

Current:
- each cell is 32 bit long
- instructions occupy 1 cell (op without arg) or 2 cells (op + arg)

Other encoding schemes that will be tested:
- instruction in 1 cell, opcode in 8 bits, argument in 24 bits
- instruction in 1 cell, opcode in 6 bits, argument in 26 bits
- bytecode, instruction in 1, 2 or 5 cells

## Instruction Set

Morty VM instructions are divided into core instructions and extensions.
Only the core instruction set must be provided by the VM.
The rest can be assembled by the compiler from the core instructions.
If the extension instruction is available in the VM the compiler should use it for better performance.
Translation from the extended set into the core set is done via simple expansion (ie "neg" -> "0 swap sub").
Translation from the core set into extended set is done by peephole optimization (ie "0 swap sub" -> "neg").

### branching

| name    | effect     | morty     | core | info | 
| ------- | ---------- | --------- | ---- | ---- |
| jz    X | (v--)      | jz @label | yes  | set I to X (next instruction cell) if v==0                              |
| call  X | (--)(=fr)  | x         | yes  | call procedure at X (next instruction cell)                             |
| ret     | (fr*=)     |           | yes  | return from procedure call                                              |
| qcall   | (a--)      | call      | yes  | quick call to address from the stack without changing the frame pointer |
| qret    | (r=)       |           | yes  | quick return without changing the frame pointer                         |
| goto  X | (--)       |           | yes  | set I to X (next instruction cell)                                      |

### stack manipulation

| asm    | effect         | morty | core | info                                        | 
| ------ | -------------- | ----- | ---- | ------------------------------------------- |
| push X | (--x)          | x     | yes  | push X onto the stack                       |  
| stor   | (a--)(=a)      | >R    | yes  | pop top of data stack onto the return stack |
| rtos   | (--a)(a=)      | R>    | yes  | pop top of return stack onto data stack     |
| dup    | (a--aa)        |       | yes  | duplicate top item                          |
| drop   | (a--)          |       | yes  | drop top item                               |
| swap   | (ab--ba)       |       | yes  | swap two top items                          |
| over   | (ab--aba)      |       | yes  |                                             |
| rot    | (abc--bca)     |       | yes  | rotate three items                          |
| unrot  | (abc--cab)     |       |      | unrotate three items                        |
|        |                |       |      |                                             |
| unpack | (v--bcde)      |       |      | unpack bytes from one cell into 4 cells    ??? byte by byte ??? |
| pack   | (abcd--v)      |       |      | pack lowest bytes from 4 cells into 1 cell ??? byte by byte ??? |
|        |                |       |      |                                             |
| 2swap  | (AaBb--BbAa)   |       |      | swap two pairs of items                     |
| 2over  | (AaBb--AaBbAa) |       |      |                                             |


### memory access

| asm    | effect     | morty | core | info | 
| ------ | ---------- | ----- | ---- | ---- | 
| get    | (a--b)     |       | yes  | get value from memory cell a     |
| set    | (va--)     |       | yes  | set memory cell a to value v     |
| allot  | (n--a)     |       | ???  | allot n cells on the heap and return address to the first allocated cell |
| geti X | (a--b)     | N/A   |      | get value from memory cell a+X   | 
| seti X | (va--)     | N/A   |      | set memory cell a+X to value v   | 

### frame - local variables

| name    | effect     | morty | core | info                                   | 
| ------- | ---------- | ----- | ---- | -------------------------------------- |
| ftos    | (--a)      | <F    |      | push frame pointer onto the data stack | 
| vget  X | (--n)      | x     | yes  | get local variable X                   |
| vset  X | (n--)      | :x    | yes  | set local variable X                   |

TODO: ftos or no ftos BUT vget and vset in the core

### primitive output

| asm    | effect     | morty   | core | info                                             | 
| ------ | ---------- | ------- | ---- | ------------------------------------------------ |
| emit   | (c--)      |         |      | print single character                           |
| dot    | (n--)      |         |      | print number from top of the stack and one space |
| echo   | (w--)      |         |      | print word from top of the stack                 |

Word is a short string (0-3 characters OR 0-4 ???) encoded as an integer value.
Words are intended mainly for VMs without propper string support.

### ALU - arithmetic

| asm    | effect     | morty | core | info                 | 
| ------ | ---------- | ----- | ---- | -------------------- |
| add    | (ab--c)    |       | yes  | a + b                |
| sub    | (ab--c)    |       | yes  | a - b                |
| mul    | (ab--c)    |       | yes  | a * b                |
| div    | (ab--c)    |       | yes  | a / b                |
| muldiv | (abc--d)   |       |      | a * b / c            |
| mod    | (ab--c)    |       | yes  | a % b                |
| neg    | (a--b)     |       | ???  | -a                   |
| ushr   | (ab--c)    |       |      | unsigned shift right |
| abs    | (a--b)     |       |      | -a if a<0 else a     |

neg -> 0 swap sub
mod -> (ab) over (aba) swap (aab) div (ac) over (aca) mul (ad) sub

### ALU - bits

| asm    | effect     | morty | core | info                   | 
| ------ | ---------- | ----- | ---- | ---------------------- | 
| and    | (ab--c)    |       | yes  | a & b                  |
| or     | (ab--c)    |       | yes  | a \| b                 |
| xor    | (ab--c)    |       | yes  | a ^ b                  |
| inv    | (a--b)     |       | ???  | ~a  (binary inversion) |
| shl    | (ab--c)    |       | yes  | a << b                 |
| shr    | (ab--c)    |       | yes  | a >> b                 |

TODO: xor vs inv in the core -> xor
a^b -> ~(a&b)&(b|c)
inv -> 0xFFFFFFFF xor

### ALU - comparators

| asm    | effect     | morty   | core | info               | 
| ------ | ---------- | -----   | ---- | ------------------ | 
| nz     | (a--x)     | bool    |      | 1 if a!=0 else 0   |
| eqz    | (a--x)     | 0=      |      | 1 if a==0 else 0   |
| gtz    | (a--x)     | 0>      |      | 1 if a>0  else 0   |
| ltz    | (a--x)     | 0<      |      | 1 if a<0  else 0   |
|        |            |         |      |                    |
| eq     | (ab--x)    | ==      | yes  | 1 if a == b else 0 |
| ne     | (ab--x)    | !=      | yes  | 1 if a != b else 0 |
| lt     | (ab--x)    | below   | yes  | 1 if a < b  else 0 |
| gt     | (ab--x)    | above   | yes  | 1 if a > b  else 0 |
| le     | (ab--x)    | or-less | ???  | 1 if a <= b else 0 |
| ge     | (ab--x)    | or-more | ???  | 1 if a >= b else 0 |
|        |            |         |      |                    |
| xeq    | (ab--abx)  |         |      | 1 if a == b else 0 |
| xne    | (ab--abx)  |         |      | 1 if a != b else 0 |
| xle    | (ab--abx)  |         |      | 1 if a <= b else 0 |
| xge    | (ab--abx)  |         |      | 1 if a >= b else 0 |
| xlt    | (ab--abx)  |         |      | 1 if a < b  else 0 |
| xgt    | (ab--abx)  |         |      | 1 if a > b  else 0 |
|        |            |         |      |                    |
| min    | (ab--x)    |         |      | a if a < b  else b |
| max    | (ab--x)    |         |      | a if a > b  else b |
| pick   | (abc--x)   |         | yes  | a if c != 0 else b |


le -> swap gt
ge -> swap lt

TODO: decide if comparators are destructive or not or both versions are available
TODO: decide about aliases: <= vs or-less vs le

### debugging

| asm     | effect     | morty   | core | info | 
| ------- | ---------- | ------- | ---- | ---- |
| vminfo  | (--)       |         | ???  | print information about VM registers and show time in ms since last vminfo call or start of the program |


### I/O - virtual devices

TODO

I/O is handled via vectored execution (similar to ioctl, fcntl).

| asm    | effect     | morty | core | info                                              | 
| ------ | ---------- | ----- | ---- | ------------------------------------------------- |
| ioget  | (dk--v)    |       | yes  | get value (v) for the key (k) from the device (d) |
| ioset  | (vdk--)    |       | yes  | set key (k) to value (v) on the device (d)        |

TODO: name: set vs put
TODO: name: io vs dev
TODO: api:  dk vs kd

# Morty VM Assembler

## Assembler

TODO

Instructions consist of operation (op) and argument (arg).
All instruction must contain op and arg separated by the dot (.).
Labels end with the colon (:).
Label's addresses are referenced with the at (@) character.

Stack based labels:
- @[ -> push address on the stack, 
- @] -> pop address from the stack, use it here, use current address there
- ]: -> pop address from the stack, use current address there

```
	push.0 jz.@start
	inc:
		push.1 add.0 ret.0
	start:
		push.2 call.@inc
	stop.0
```

## High-Level Assembler

TODO

Instructions with operations that always take 0 as the argument don't have to contain the argument (ie "add" instead of "add.0").
All values are transformed into "push.value".
Character literals are converted into integer literals ("push.value").

```
	0 jz.@start
	inc:
		1 add ret
	start:
		2 :inc
	stop
```

# Kanban

```
Active:
- conditionals
- loops

Next:
- vm text-based compiled format
- global variables
- static arrays
- capture variables in lambdas
- define and document file extensions

To do:
- strings
- parametric cell size (16, 32, 64 bits)
- tail call
- dynamic statistics (op freq)
- little vs big endian
- ASM - token to line number (for error reporting)
- iterators
- cooperative multitasking
- literate programming (compile from markdown)
- multiprocessing
- add Morty to linguist (https://github.com/github/linguist) so it can be detected by GitHub

Done:
- asm text-based compiled format
- stack based label resolution
- high-level assembler
- vm cli
- vm - load from binary file
- primitive output
- vm boot sequence
- asm output to binary file
- asm character literals
- lang - structs
- lang - macros
```



# References

|                     Title | URL                                                            |
| ------------------------: | -------------------------------------------------------------- |
| **Generic stack machine** | https://users.ece.cmu.edu/~koopman/stack_computers/sec3_2.html |
|        **Permacomputing** | http://viznut.fi/texts-en/permacomputing.html                  |
|        **Starting Forth** | https://www.forth.com/starting-forth/                          |
|        **P-code machine** | https://en.wikipedia.org/wiki/P-code_machine                   |
|         **Stack machine** | https://en.wikipedia.org/wiki/Stack_machine                    |
|        **Thinking Forth** | http://thinking-forth.sourceforge.net/thinking-forth-ans.pdf   |
|                **Fabris** | https://github.com/mobarski/fabris                             |
|               **UCBLogo** | https://en.wikipedia.org/wiki/UCBLogo                          |
|                   **Uxn** | https://100r.co/site/uxn.html                                  |
|         **Threaded code** | https://en.wikipedia.org/wiki/Threaded_code                    |
|       **Forth threading** | http://www.complang.tuwien.ac.at/forth/threading/              |
|   **Forth threaded code** | http://www.complang.tuwien.ac.at/forth/threaded-code.html      |
|          **Interpreters** | http://realityforge.org/code/virtual-machines/2011/05/19/interpreters.html |
|    **Cozy design spaces** | https://www.lexaloffle.com/bbs/?tid=31634                      |
|         **Another World** | https://fabiensanglard.net/another_world_polygons/             |
|                  **Mako** | https://github.com/JohnEarnest/Mako                            |

[//]: # (online .md editor: https://markdown-editor.github.io/ )
