((
	based on http://dada.perl.it/shootout/nestedloop.html
))

((	Morty:

	def loops (n--a) :x
		0
		x times x times x times x times x times x times
			1 add
		loop loop loop loop loop loop
	ret
))

goto.@start

((
loops: (28: 2000ms  frame=addr,incr,to,from; loop increments / 1351ms with addi 
	stor.0
	push.0 stor.0
	push.0
	vget.1 (n) times.@[
		vget.1 (n) times.@[
			vget.1 (n) times.@[
				vget.1 (n) times.@[
					vget.1 (n) times.@[
						vget.1 (n) times.@[
							push.1 add.0
							( rget.0 push.1 add.0 vset.2 )
							( rget.0 push.2 mul.0 push.1 add.0 vset.2 )
						loop.@]
					loop.@]
				loop.@]
			loop.@]
		loop.@]
	loop.@]
ret.0
))

loops: (28: 2455ms without addi, 1800ms with addi
	stor.0
	push.0 stor.0
	push.0
	vget.1 (n) push.0 push.1 for.@[
		vget.1 (n) push.0 push.1 for.@[
			vget.1 (n) push.0 push.1 for.@[
				vget.1 (n) push.0 push.1 for.@[
					vget.1 (n) push.0 push.1 for.@[
						vget.1 (n) push.0 push.1 for.@[
							( push.1 add.0 ( 1800 ms with addi )
							rget.0 push.1 add.0 vset.2 ( 3232ms with addi )
							( rget.0 push.2 mul.0 push.1 add.0 vset.2 ( 3812ms with muli addi )
						loop.@]
					loop.@]
				loop.@]
			loop.@]
		loop.@]
	loop.@]
ret.0


start:
	push.28 call.@loops vminfo.0


( 3000ms vs 3200ms -> times+mul vs for )
