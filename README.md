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
rot (a b c -- b c a) ( optional )
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
poke (val addr --)
peek (addr -- val)
```

## control

```
jz {} (a --) # branch when a==0, addr is relative
call {} (--) # push return addr onto return stack
ret (--)
push {} (-- a)
```

## registers

not so nice names as these operations should be performed with care

```
>r (a --)
r> (-- a)
```


# Language Examples

```
def kinetic_energy :m :v (m v -- e)
	v v mul m mul
end
```

# References

|                     Title | URL                                                            |
| ------------------------: | -------------------------------------------------------------- |
| **Generic stack machine** | https://users.ece.cmu.edu/~koopman/stack_computers/sec3_2.html |
|        **Permacomputing** | http://viznut.fi/texts-en/permacomputing.html                  |
|                **Fabris** | https://github.com/mobarski/fabris                             |
|                   **Uxn** | https://100r.co/site/uxn.html                                  |

[//]: # (online .md editor: https://markdown-editor.github.io/ )
