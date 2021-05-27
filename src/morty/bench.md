# Intro

Programs based on Benchmark Tests from http://dada.perl.it/shootout/

# Nested Loops

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


# Fibonnaci Numbers

http://dada.perl.it/shootout/fibo.html

```

def fib (n--x)
       (n) dup 2 lt if drop 1 ret then   (Forth contitionals, TODO: Morty)
       (n) dup 2 sub fib (n f2) swap
    (f2 n) 1 sub fib (f2 f1)
           add
end

clock 8 fib 
clock swap (c1 c2 n) dot swap sub dot
```


# Random Number Generator

http://dada.perl.it/shootout/random.html

```
```


# Sieve of Erathostenes

http://dada.perl.it/shootout/sieve.html

```
```

