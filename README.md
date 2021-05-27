# Morty

TODO

## Idea

Why:
- permacomputing
- internet of things
- stack VM / concatenative language experimentation

Target usage:
- simple game dev (demakes, fantasy consoles/computers)
- education
- robotics

Design principles:
- small and easy to learn (like Go)
- small and easy to implement (close to the stack machine like Forth)
- easy to read (like Python, Logo or Basic; named arguments, dot properties, less symbols)

Target VM implementations:
- Python
- JS
- MicroPython
- C
- Go

# VM Architecture

VM:
- R - return stack pointer
- S - (data) stack pointer
- I - instruction pointer
- F - frame pointer
- T - top of stack register
- CODE - program memory
- MEM - main memory for both stacks

# VM Instructions

## minimal VM

### operations with argument

| name    | effect     | info | 
| ------- | ---------- | ---- |
| push  X | (--x)      | push X onto stack |  
| jz    X | (v--)      | add X to the instruction pointer if v==0 |
| call  X | (--)(=fr)  | call procedure at X |

### operations

| asm    | effect     | morty | info | 
| ------ | ---------- | ----- | ---- |
| ret    | (fr*=)     |       | return from procedure call |
| dup    | (a--aa)    |       |  |
| drop   | (a--)      |       |  |
| swap   | (ab--ba)   |       |  |
| stor   | (a--)(=a)  | >R    |  |
| rtos   | (--a)(a=)  | R>    |  |
| add    | (ab--c)    |       |  |
| xor    | (ab--c)    |       |  |
| and    | (ab--c)    |       |  |
| nz     | (a--b)     | bool  |  |
| get    | (a--b)     |       | aka @ |
| set    | (va--)     |       | aka ! |

## basic VM

### additional operations with argument

| name    | effect     | morty | info | 
| ------- | ---------- | ----- | ---- |
| vget  X | (--n)      | x     |  |
| vset  X | (n--)      | :x    |  |
| radd  X | (=?\*x)    |       | add X to the R register |

### additional operations

| asm    | effect     | morty   | info | 
| ------ | ---------- | ------- | ---- |
| rot    | (abc--bca) |         |  |
| over   | (ab--aba)  |         |  |
| sub    | (ab--c)    |         |  |
| mul    | (ab--c)    |         |  |
| div    | (ab--c)    |         |  |
| mod    | (ab--c)    |         |  |
| or     | (ab--c)    |         |  |
| ltz    | (a--b)     |         |  |
| ralloc | (n--a)     |         |  |
| eq     | (ab--c)    |         |  |
| ne     | (ab--c)    |         |  |
| le     | (ab--c)    | or-less |  |
| ge     | (ab--c)    | or-more |  |
| lt     | (ab--c)    |         |  |
| gt     | (ab--c)    |         |  |
| neg    | (a--b)     |         |  |
| clock  | (--a)      |         |  |
| dot    | (a--)      |         |  |
| emit   | (a--)      |         |  |

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

## Loops

TODO

## Conditionals

TODO

## Structures

TODO

```
(better for grep and syntax highlight)
struct point   .x .y   
struct circle  .r .x .y

(or)

(less writing)
struct point   x y  
struct circle  r x y

def dist-to-circle (c p -- d) :p:point :c:circle
    p.x c.x sub dup mul
    p.y c.y sub dup mul
    add sqrt               (distance to the center)
    c.r sub
end

```

## Macros

TODO

```
macro rot  >R swap <R swap
```

## Lambdas

Morty:
```
21 [ dup add ] execute 
```

MortyVM ASM:
```
    push 21
    push 1             (unconditional jump)
    jnz @@end_of_list  (unconditional jump)
start_of_list:
    dup 0
    add 0
    ret 0
end_of_list:
    push @start_of_list
    call @execute
```

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
|        **Starting Forth** | https://www.forth.com/starting-forth/                          |
|        **P-code machine** | https://en.wikipedia.org/wiki/P-code_machine                   |
|         **Stack machine** | https://en.wikipedia.org/wiki/Stack_machine                    |
|        **Thinking Forth** | http://thinking-forth.sourceforge.net/thinking-forth-ans.pdf   |
|                **Fabris** | https://github.com/mobarski/fabris                             |
|               **UCBLogo** | https://en.wikipedia.org/wiki/UCBLogo                          |
|                   **Uxn** | https://100r.co/site/uxn.html                                  |
|         **Threaded code** | https://en.wikipedia.org/wiki/Threaded_code                    |

[//]: # (online .md editor: https://markdown-editor.github.io/ )
