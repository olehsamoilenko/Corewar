.name " NAME"

.comment "#just a basic%l"

loop:



	sti	-21,%:live,%0
	and	r1, %0,r1
live:	live	%1
	zjmp	%:live

live