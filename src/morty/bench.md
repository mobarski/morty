# Intro

Programs based on Benchmark Tests from http://dada.perl.it/shootout/

# Programs

## Nested Loops

http://dada.perl.it/shootout/nestedloop.html

```
def nested-loops (x--y) :x
    0
    x times x times x times x times x times x times
        1 add
    loop loop loop loop loop loop
end

clock 8 nested-loops
clock swap (c1 c2 n) dot swap sub dot
```


## Fibonnaci Numbers

http://dada.perl.it/shootout/fibo.html

```

(Forth contitionals, TODO: more Morty)

def fib (n--f)
       (n) dup 2 lt if drop 1 ret then
       (n) dup 2 sub fib (n f2) swap
    (f2 n) 1 sub fib (f2 f1) add (f)
end

def fib2 (n--f) :n
    n 2 lt if 1 ret then
    n 2 sub fib (f2)
    n 1 sub fib (f2 f1) add (f)
end

clock 8 fib 
clock swap (c1 c2 n) dot swap sub dot
```


## Random Number Generator

http://dada.perl.it/shootout/random.html

```
(temporary global rstack variable notation)
  139968 $IM
    3877 $IA
   29573 $IC
      42 $LAST

def gen-random (max--n)
    LAST IA mul IC add IM mod dup $LAST
    (max last) mul IM div
end

def main (n)
    (n) times 100 gen-random drop loop
end

clock 1000 main
clock (c1 c2) swap sub dot
```


## Sieve of Erathostenes

http://dada.perl.it/shootout/sieve.html

```
```


# Results

TODO

