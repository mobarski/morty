# Morty

TODO

## Idea

- language for simple game dev (demakes)
- language that is small
- language that is easy to implement (close to the stack machine like Forth)
- language that is easy to read (named arguments, dot properties, less symbols)

## Targets

- JS
- Python
- MicroPython
- C
- Go
- Rust

# VM Architecture

R - return stack pointer
S - (data) stack pointer
I - instruction pointer
B - base pointer

TOS - top of stack register

ROM - program memory
RAM - main memory for both stacks

# Instructions

## stack

```
dup  (a -- a a)
drop (a --)
over (a b -- a b a)
swap (a b -- b a)
rot  (a b c -- b c a) ( optional )
```

## ALU

```
add (a b -- c)
sub (a b -- c)
mul (a b -- c) ( optional )
div (a b -- c) ( optional )
mod (a b -- c) ( optional )

and (a b -- c)
or  (a b -- c)
xor (a b -- c)
```

## mem

```
get (a -- v)
set (v a --)
```

## control

```
jz   {} (a --)  # branch when a==0, addr is relative
call {} (--)    # push return addr onto return stack
push {} (-- a)  # push literal onto data stack
ret (--)
```

## registers

not so nice names as these operations should be performed with care

```
>r (a --)
r> (-- a)
```

# Language Examples

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
def example (a b c -- x) :c :b :a
    a dup mul b mul c add
end
```
Variables cannot be reassigned (this might change).

Captured values are stored on the return stack (the capture is just >R plus compiler logic)
On function end or early return they will be automatically discarded - this is possible
by the use of the B register which stores the "base" of the return stack.
On function call both B and I registers are stored on the return stack.

Using a variable is compiled into opcode "local" with the parameter in the next program cell, which stores information about variable index.
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
