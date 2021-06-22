((
	http://dada.perl.it/shootout/ary3.html
	
	def main (n--) :n
		n allot :x
		n allot :y
		n times
			i 1 add  x i add set
		loop
		1000 times
			n times ( this should count down to 0 !!! )
				x i add get
				y i add get
				add
				y i add set
			loop
		loop
		y get dot (first y)
		y n 1 sub add get dot (last y)
	end
	
))

goto.@start

main:
	stor.0 (1 -> n)
	vget.1 allot.0 stor.0 (2 -> x)
	vget.1 allot.0 stor.0 (3 -> y)
	vget.1 times.@[
		rget.0 push.1 add.0  vget.2 rget.0 add.0  set.0
	loop.@]
	(ok)
	push.1000 times.@[
		vget.1 (n) times.@[
			vget.2 (x) rget.0 add.0 get.0 (TODO: combo rget.X add get)
			vget.3 (y) rget.0 add.0 get.0 
			add.0
			vget.3 rget.0 add.0 set.0     (TODO: combo rget.X add set)
		loop.@]
	loop.@]
	vget.3 get.0 dot.0
	vget.3 vget.1 push.1 sub.0 add.0 get.0 dot.0
ret.0

start:
	push.100000 call.@main vminfo.0
