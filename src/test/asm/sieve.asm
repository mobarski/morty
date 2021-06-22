((
	http://dada.perl.it/shootout/sieve.html

	def sieve (n) :n
		8193 allot :flags
		n times
			0 :count
			8193 times 1 flags i add set loop
			8193 2 1 for
				flags i add get 0 gt then
					8193 i 2 mul i for
						0 flags i add set
					loop
					count 1 add :count
				do
			loop
		loop
		count dot
	end
))

goto.@start

	sieve:
		stor.0 (1:n)
		push.0 stor.0 (2:flags)
		push.0 stor.0 (3:count)
		push.8139 allot.0 vset.2
		vget.1 (n) times.@[
			push.0 vset.3
			push.8193 times.@[ push.1 vget.2 rget.0 add.0 set.0 loop.@]
			push.8193 push.2 push.1 for.@[
				vget.2 rget.0 add.0 get.0 push.0 gt.0 jz.@[
					push.8193 rget.0 push.2 mul.0 rget.0 for.@[
						push.0 vget.2 rget.0 add.0 set.0
					loop.@]
					vget.3 push.1 add.0 vset.3
				]:
			loop.@]
		loop.@]
		vget.3 dot.0
	ret.0
	
start:
	push.5000 call.@sieve vminfo.0
