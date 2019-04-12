.name "Underscore"
.comment "__UNDERSCORE_S__"

lld     %42, r12					# HERE WE ARE
lld		-10000, r10					# HEY SF
lld		%0, r88						# HEY
lldi	r1, r1, r1					# OVERFLOW
lldi	r1, %10000, r1				# OVERFLOW
lldi	r1, %0, r1					# OVERFLOW
lldi	-10000, r10, r1				# HEY SF
lldi	r88, r1, r1					# HEY
lldi	r1, r1, r88					# HEY

sti		r1, %0, %0

first:
	ld		%0, r2
	zjmp	%-511
defdessus:
	live	%-42
	sti		r10, r11, r13
	add		r12, r13, r13
	xor		r13, %-255, r14
	ld		%0, r2
	zjmp	%-511
	zjmp	%:zero
	ld		%0, r14
	zjmp	%:defdessus
zero:
	ld		%0, r13
	zjmp	%:defdessus
#plein de truck pour remplir
lasertop:
	live	%-42
	ld		%-128, r6
	ld		%190056198, r7
	ld		%0, r5
	ld		%0, r16
