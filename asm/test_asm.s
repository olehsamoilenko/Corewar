   .name "zwq
 hello"
.comment 		"just a basic living prog"
		
l2:	sti	-21,%:live,%0
	and	r1,%0,r1
live:	live	%1
	zjmp	%:live
