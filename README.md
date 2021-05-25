# Morty

TODO

## Idea

- language for simple game dev (demakes)
- language that is small (like Go)
- language that is easy to implement (close to the stack machine like Forth)
- language that is easy to read (like Python, Logo or Basic; named arguments, dot properties, less symbols)
- family of VMs to experiment on

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
N - next on stack register

CODE - program memory
MEM - main memory for both stacks

# VM Instructions

## minimal VM

### operations with argument

| name    | effect     | info | 
| ------- | ---------- | ---- |
| push  X | (--x)      | push X onto stack |  
| jz    X | (v--)      | add X to the instruction pointer if v==0 |
| call  X | (--)(=fr)  | call procedure at X |

### operations

| name   | effect     | info | 
| ------ | ---------- | ---- |
| ret    | (fr*=)     | return from procedure call |
| dup    | (a--aa)    |  |
| drop   | (a--)      |  |
| swap   | (ab--ba)   |  |
| stor   | (a--)(=a)  | aka >R |
| rtos   | (--a)(a=)  | aka R> |
| add    | (ab--c)    |  |
| xor    | (ab--c)    |  |
| and    | (ab--c)    |  |
| nz     | (a--b)     | aka bool |
| get    | (a--b)     | aka @ |
| set    | (va--)     | aka ! |

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
| or-less | (ab--c)   | le |
| or-more | (ab--c)   | ge |
| lt     | (ab--c)    |  |
| gt     | (ab--c)    |  |
| neg    | (a--b)     |  |
| clock  | (--a)      |  |
| dot    | (a--)      |  |
| emit   | (a--)      |  |
| frame  | (--a)      |  |

# Language

Morty language is similar to Morty VM instruction set.
VM operations besides push,jz,call can be used directly in the language.

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
    m h potential_energy (ek ep) add
end
```

## Named parameters

Morty allows capturing input parametrs into local constants.
To capture a param prefix its name with a colon (ie :my-param).
Use the param name to push its value onto the stack.

```
(x = a*a * b + c)
def example (a b c -- x) :c :b
    (a) dup mul b mul c add
end
```

Captured values are stored on the return stack.
On function end or early return they will be automatically discarded - this is possible
by the use of the F register which stores the "frame" of the return stack.
On function call both F and I registers are stored on the return stack.

## Attributes

TODO

```
def check-collision (e p -- c)
	(e p) dup .x swap .y (e px py) rot (px py e)
	(..e) dup .x swap .y (px py ex ey) distance (d)
	8 or-less (c)
end

def check-collision (e p -- c) :p :e
	e.x e.y p.x p.y distance (d)
	8 or-less (c)
end

```

## Loops

TODO

## Conditionals

TODO

## Lists

TODO

## Arrays

TODO

## Strings

TODO

# References

|                     Title | URL                                                            |
| ------------------------: | -------------------------------------------------------------- |
| **Generic stack machine** | https://users.ece.cmu.edu/~koopman/stack_computers/sec3_2.html |
|        **Permacomputing** | http://viznut.fi/texts-en/permacomputing.html                  |
|                **Fabris** | https://github.com/mobarski/fabris                             |
|                **UCBLogo**| https://en.wikipedia.org/wiki/UCBLogo                          |
|                   **Uxn** | https://100r.co/site/uxn.html                                  |

[//]: # (online .md editor: https://markdown-editor.github.io/ )
