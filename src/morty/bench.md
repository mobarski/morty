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

debug
    8 nested-loops
debug
```


## Fibonnaci Numbers

http://dada.perl.it/shootout/fibo.html

```
def fib (n--f)
       (n) dup 2 lt [ drop 1 ret ] if
       (n) dup 2 sub fib (n f2) swap
    (f2 n) 1 sub fib (f2 f1) add (f)
end

def fib2 (n--f) :n
    n 2 lt [ 1 ret ] if
    n 2 sub fib (f2)
    n 1 sub fib (f2 f1) add (f)
end

debug
    8 fib 
debug
    8 fib2
debug
```


## Random Number Generator

http://dada.perl.it/shootout/random.html

```
(TODO: this is temporary global rstack variable notation)
  139968 $IM
    3877 $IA
   29573 $IC
      42 $LAST

def gen-random (max--n)
    LAST IA mul IC add IM mod dup $LAST
    (max last) mul IM div
end

(TODO: this is Fabris loop notation, replace it when Morty notation is ready)
def main (n)
    (n) times 100 gen-random drop loop
end

debug
    1000 main
debug
```


## Sieve of Erathostenes

http://dada.perl.it/shootout/sieve.html

```
```


# Results

TODO


