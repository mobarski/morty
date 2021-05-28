# Morty

TODO

## Idea

Why:
- permacomputing
- internet of things
- stack VM / concatenative language experimentation

Target usage:
- simple game/utility dev (demakes, fantasy consoles/computers)
- education
- robotics

Design principles:
- small and easy to learn (like Go)
- small and easy to implement (close to the stack machine like Forth)
- easy to read (like Python, Logo or Basic; named arguments, dot properties, less symbols)

Target VM implementations:
- Python (easiest)
- C (most performant)
- JS (portable, GUI)
- MicroPython (portable - microcontrolers)
- Go (performant backend)

## Morty Philosophy

1. Start from scratch.
2. Add parts one by one observing how they interact.
3. Remove parts that can be cheaply built using other parts.
4. Keep parts that are expensive to build but make building other parts easier.
5. A state should be achieved where removing any single part should have drastic consequences.
6. This is the system's core.
7. Now add parts that will make the system more friendly / usable / performant.
8. Try to cover 80% of requirements with 20% of features.

# VM Architecture

VM:
- F - frame pointer
- I - instruction pointer
- R - return stack pointer
- S - (data) stack pointer
- T - top of stack register
- CODE - program memory
- MEM - main memory for both stacks

# VM Instructions

## Core

### branching

| name    | effect     | morty | info | 
| ------- | ---------- | ----- | ---- |
| jz    X | (v--)      | if    | set I to X (next instruction cell) if v==0 |
| call  X | (--)(=fr)  | x     | call procedure at X (next instruction cell) |
| ret     | (fr*=)     |       | return from procedure call |

### stack manipulation

| asm    | effect     | morty | info | 
| ------ | ---------- | ----- | ---- 
| push X | (--x)      | x     | push X (next instruction cell) onto the stack |  
| dup    | (a--aa)    |       |  |
| drop   | (a--)      |       |  |
| swap   | (ab--ba)   |       |  |
| stor   | (a--)(=a)  | >R    |  |
| rtos   | (--a)(a=)  | R>    |  |

### ALU - arithmetic

| asm    | effect     | morty | info | 
| ------ | ---------- | ----- | ---- 
| add    | (ab--c)    |       |  |
| sub    | (ab--c)    |       |  |
| mul    | (ab--c)    |       |  |
| div    | (ab--c)    |       |  |

### ALU - logic

| asm    | effect     | morty | info | 
| ------ | ---------- | ----- | ---- 
| and    | (ab--c)    |       |  |
| or     | (ab--c)    |       |  |
| xor    | (ab--c)    |       |  |
| nz     | (a--b)     | bool  |  |

### memory access

| asm    | effect     | morty | info | 
| ------ | ---------- | ----- | ---- 
| get    | (a--b)     |       | aka @ |
| set    | (va--)     |       | aka ! |
| allot  | (n--a)     |       | allot n cells in the "dictionary" and return address to the first allocated cell |

### lambda functions

| asm    | effect     | morty   | info | 
| ------ | ---------- | ------- | ---- |
| qcall  | (a--)      | call    | quick call to address from the stack without changing the frame pointer |
| qret   | (r=)       |         | quick return without changing the frame pointer |

### local variables

| name    | effect     | morty | info | 
| ------- | ---------- | ----- | ---- |
| vget  X | (--n)      | x     |  |
| vset  X | (n--)      | :x    |  |
| radd  X | (=?\*x)    | N/A   | add X to the R register |

### debugging / simple output

These operations might not be included in the final VM but are essential for debugging early versions of the VM.

| asm    | effect     | morty   | info | 
| ------ | ---------- | ------- | ---- |
| dot    | (n--)      |         | print n (as int) and a single space |
| emit   | (a--)      |         | print character a |
| ok     | (ab--a)    |         | panic when a != b showing a and b, when a==b print b space "ok" space  |
| clock  | (--a)      |         | push number of microsends since the program start |

## Extension

### ALU

| asm    | effect     | morty   | info | 
| ------ | ---------- | ------- | ---- |
| mod    | (ab--c)    |         |  |
| shl    | (ab--c)    |         | shift left |
| shr    | (ab--c)    |         | shift right |
| ushr   | (ab--c)    |         | unsigned shift right |

### ALU - comparators

| asm    | effect     | morty         | info   | 
| ------ | ---------- | ------------- | ------ |
| eq     | (ab--c)    | ==            | a == b |
| ne     | (ab--c)    | !=            | a != b |
| le     | (ab--c)    | <= vs or-less | a <= b |
| ge     | (ab--c)    | >= vs or-more | a >= b |
| lt     | (ab--c)    | <  vs is-less | a < b  |
| gt     | (ab--c)    | >  vs is-more | a > b  |

TODO: decide is-less vs <
TODO: decide or-over vs or-more, or-under vs or-less

### stack manipulation

| asm    | effect     | morty   | info | 
| ------ | ---------- | ------- | ---- |
| rot    | (abc--bca) |         |  |
| over   | (ab--aba)  |         |  |


# Language

Morty language is similar to Morty VM instruction set.
Almost all VM instructions can be used directly.
Only operations that require argument from the next program cell are reserved for the compiler.

## Language Examples

```
def kinetic_energy (m v -- e)
    dup (m v v) mul mul
end

def potential-energy (m h -- e)
    g (m h g) mul mul
end

def total-energy (m v h -- e) :h :v :m
    m v kinetic-energy   (ek)
    m h potential-energy (ek ep) add
end
```

## Named local variables

Morty allows capturing values into named local variables.
To capture a value prefix target variable name with a colon (ie :my-var).
Use the variable name to push its value onto the stack.

```
( y = a*x*x + b*x + c )
def poly2 (x a b c -- x) :c :b :a :x
    x x mul a mul (axx)
    x b mul (axx bx) add
    c add
end
```

Local variables are stored on the return stack.
On function end or early return they will be automatically discarded - this is possible
by the use of the F register which stores the "frame" of the return stack.
On function call both F and I registers are stored on the return stack.

## Lambda functions

```
21 [ dup add ] call
```

MortyVM ASM:
```
    push 21
    push 0               (unconditional jump on [)
    jz   @end_of_lambda  (unconditional jump on [)
start_of_lambda:
    dup  0
    add  0
    qret 0
end_of_lambda:
    push @start_of_lambda
    qcall 0
```

Lambda functions are using the stack frame of the parent.
They are not closures and should not be shared if they are using parent's local variables.

## Conditionals

Conditionals are based on lambda functions.

```
distance 10 or-less [ collision-warning ] if
```

MortyVM ASM
```
    call @distance
    push 10
    le 0
    push 0
    jz @end-of-lambda
start-of-lambda:
    call @callision-warning
    qret 0
end-of-lambda:
    push @start-of-lambda
    swap 0                   (addr bool)
    jz @else
    dup   0                  (for drop after else)
    qcall 0
else:
    drop 0
```

```
age 18 or-more [ show-content ] [ show-restriction ] if-else
```

## Loops

TODO

## Structures

TODO

Struct definition cannot be longer than 1 line.
This is on purpose to keep the structures simple.
Field names are prefixed with a dot to make them easier to grep and highlight.

```
struct point   .x .y   
struct circle  .r .x .y

def dist-to-circle (c p -- d) :p:point :c:circle
    p.x c.x sub dup mul
    p.y c.y sub dup mul
    add sqrt               (distance to the center)
    c.r sub
end
```

MortyVM ASM
```
dist-to-circle:
    (init local variables)
    radd 2
    vset 1  (circle)
    vset 2  (point)
    
    (line1)
    vget 2  (point)
    push 0  (.x offset in point)
    add 0   (.x + offset)
    get 0   (.x value)
    vget 1  (circle)
    push 1  (.x offset in circle)
    add 0
    get 0  (.x value)
    sub 0
    dup 0
    mul 0
    
    (line 2)
    vget 2
    push 1
    add 0
    get 0
    vget 1
    push 2
    add 0
    get 0
    sub 0
    dup 0
    mul 0
    
    (line 3)
    add 0
    call @sqrt
    
    (line 4)
    vget 1
    push 0
    add 0
    get 0 (.r value)
    sub 0
    
    ret 0
```

## Macros

TODO

```
macro rot  >R swap <R swap
```

Macros cannot be longer than 1 line.
This is on purpose to keep the macros simple.

## Arrays

TODO

## Strings

TODO

## Lists

TODO


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

[//]: # (online .md editor: https://markdown-editor.github.io/ )
