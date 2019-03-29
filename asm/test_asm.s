.name       "Batman"
.comment    "This city needs me"

_loop:
        sti r1, %:live, %1
live:
        live %4
        ld %0, r2
		zjmp %:live
