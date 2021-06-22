((
	based on http://dada.perl.it/shootout/fibo.html
))

((	Morty:

	def fib (n--f) :n
		n 2 lt then 1 ret do
		n 2 sub fib (f2)
		n 1 sub fib (f2 f1) add (f)
	end
	
	38 fib vminfo
))

goto.@start

fib: (n--f) stor.0
	vget.1  push.2   lt.0  jz.@[  push.1 ret.0  ]:         (38: 2238ms)
	( vget.1  push.2   lt.0  jz.@[  push.1 ret.0  nop.@]   (38: 2321ms)
	vget.1  push.2  sub.0  call.@fib
	vget.1  push.1  sub.0  call.@fib add.0
ret.0

start:

	push.38 call.@fib vminfo.0
