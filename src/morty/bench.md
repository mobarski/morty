# Intro

Programs based on Benchmark Tests from http://dada.perl.it/shootout/

# Programs

## Nested Loops

http://dada.perl.it/shootout/nestedloop.html

```
(TODO: this is Fabris loop notation, replace it when Morty notation is ready)
def nested-loops (x--y) :x
    0
    x times x times x times x times x times x times
        1 add
    loop loop loop loop loop loop
end

debug  8 nested-loops  debug
```


## Fibonnaci Numbers

http://dada.perl.it/shootout/fibo.html

```
def fib (n--f) :n
    n 2 lt [ 1 ret ] if
    n 2 sub fib (f2)
    n 1 sub fib (f2 f1) add (f)
end

debug  8 fib  debug
```


## Random Number Generator

http://dada.perl.it/shootout/random.html

```
(TODO: this is temporary global rstack variable notation)
42 $LAST

def gen-random (max--n)
    (max) LAST 3877 mul 29573 add 139968 mod dup $LAST
    (max last) mul 139968 div
end

(TODO: this is Fabris loop notation, replace it when Morty notation is ready)
def main (n)
    (n) times 100 gen-random drop loop
end

debug  1000 main  debug
```


## Sieve of Erathostenes

http://dada.perl.it/shootout/sieve.html

```
```


# Results

TODO


