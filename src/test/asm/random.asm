((
	http://dada.perl.it/shootout/random.html

	{ 42 } $LAST
	def gen-random (max--n)
		LAST 3877 mul 29573 add 139968 mod $LAST
		(max) LAST mul 139968 div (n)
	end

))

goto.@start

goto.@[ 42 pusha.@] gset.0

gen-random: (max--n)
	gget.0 push.3877 mul.0 push.29573 add.0 push.139968 mod.0 gset.0
	(max) gget.0 mul.0 push.139968 div.0
ret.0

main:
	(n) times.@[
		push.100 call.@gen-random drop.0
	loop.@]
	push.100 call.@gen-random dot.0
ret.0

start:
	
	push.70_000_000 call.@main vminfo.0
