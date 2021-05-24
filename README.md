# Morty

TODO

## Idea

- language for simple game dev (demakes)
- language that is small (like Go)
- language that is easy to implement (close to the stack machine like Forth)
- language that is easy to read (like Python, Logo or Basic; named arguments, dot properties, less symbols)

## Targets

- Python
- JS
- MicroPython
- C
- Go

# VM Architecture

R - return stack pointer
S - (data) stack pointer
I - instruction pointer
F - frame pointer

T - top of stack register

CODE - program memory
RAM - main memory for both stacks

# VM Instructions

VM instructions are based on the p-code machine.

## minimal VM

### main instructions

| name   | effect     | info | 
| ------ | ---------- | ---- |
| oper X |            | extension operation X (see below) |
| push X | (--x)      | push X onto stack |  
| jz   X | (v--)      | add X to the instruction pointer if v==0 |
| call X | (--)(--fr) | call function X |

### operations

| name   | effect     | info | 
| ------ | ---------- | ---- |
| dup    | (a--aa)    |  |
| drop   | (a--)      |  |
| swap   | (ab--ba)   |  |
| stor   | (a--)(=a)  |  |
| rtos   | (--a)(a=)  |  |
| add    | (ab--c)    |  |
| xor    | (ab--c)    |  |
| and    | (ab--c)    |  |
| nz     | (a--b)     |  |
| get    | (a--b)     |  |
| set    | (va--)     |  |
| frame  | (--a)      |  |

## basic VM

### additional operations

| name   | effect     | info | 
| ------ | ---------- | ---- |
| rot    | (abc--bca) |  |
| over   | (ab--aba)  |  |
| sub    | (ab--c)    |  |
| mul    | (ab--c)    |  |
| div    | (ab--c)    |  |
| mod    | (ab--c)    |  |
| or     | (ab--c)    |  |
| ltz    | (a--b)     |  |
| ralloc | (n--a)     |  |
| eq     | (ab--c)    |  |
| ne     | (ab--c)    |  |
| le     | (ab--c)    |  |
| ge     | (ab--c)    |  |
| lt     | (ab--c)    |  |
| gt     | (ab--c)    |  |
| neg    | (a--b)     |  |
| clock  | (--a)      |  |
| dot    | (a--)      |  |
| emit   | (a--)      |  |


# Language

Morty language is similar to Morty VM instruction set.
VM operations can be used directly in the language.
VM instructions are not directly available - they are used by the compiler.

## Language Examples

```
def kinetic_energy (m v -- e)
    dup (m v v) mul mul
end

def potential_energy (m h -- e)
    g (m h g) mul mul
end

def total_energy (m v h -- e) :h :v :m
    m v kinetic_energy   (ek)
    m h potential_energy (ek ep)
    add
end
```

## Named local variables

Morty allows creating local variables.
To capture a value prefix variable name with a colon (ie :my-var).
To use a variable just use its name. 

```
(x = a*a * b + c)
def example (a b c -- x) :c :b
    (a) dup mul b mul c add
end
```

Captured values are stored on the return stack (the capture is just >R plus compiler logic)
On function end or early return they will be automatically discarded - this is possible
by the use of the F register which stores the "frame" of the return stack.
On function call both F and I registers are stored on the return stack.

Using a variable compiles into "frame" operation ie "frame 1 add get"
First captured value has index 1, second 2 etc.


# References

|                     Title | URL                                                            |
| ------------------------: | -------------------------------------------------------------- |
| **Generic stack machine** | https://users.ece.cmu.edu/~koopman/stack_computers/sec3_2.html |
|        **Permacomputing** | http://viznut.fi/texts-en/permacomputing.html                  |
|                **Fabris** | https://github.com/mobarski/fabris                             |
|                **UCBLogo**| https://en.wikipedia.org/wiki/UCBLogo                          |
|                   **Uxn** | https://100r.co/site/uxn.html                                  |

[//]: # (online .md editor: https://markdown-editor.github.io/ )
