		( this is a comment
			vincr	3
			push	21
		
		(
		(	this
		(	is
		(	multi line
		(	comment
		(
		
			push	11
			jz		@start
		
		mul4:
			push 2
			push 2
			mul 0
			mul 0
			ret 0
		
		div4:
			push 4
			div 0
			ret 0
			
( --- comment ---------------------- )

		start:
			push 44
		
			call @mul4		( comment)
		
			push '*'
			
			emit 0
		( ---------- )
			stop 0
