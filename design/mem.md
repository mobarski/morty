# Memory

This will change.

Layout:
- code
- global read-only
- global read-write
- return stack + data stack
- heap

```
def foo (--a)
    { 11 22 33 }
end

( --- COMPILES TO --- )
foo: (--a)
    goto.@[ #11 #22 #33 pusha.@] (better locality)
ret
( --- OR --- )
foo: (--a)
    pusha.@foo_data1
ret
foo_data_1: #11 #22 #33


{ 11 22 33 } $arr
{ 42 }       $the-answer  (TODO: special syntax?)
10 allot     $buffer
10 20 0 new-array $array

{ 11 22 33 } as arr
{ 42 }       as the-answer (TODO: special syntax?)
10 allot     as buffer
10 20 0 new-array as array

foo:
	arr 1 add get (22)
	the-answer add (64)
end

*5#0 -> #0 #0 #0 #0 #0

```
