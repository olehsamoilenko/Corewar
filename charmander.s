.name "Charmander"
.comment "Char-Char!"

ld %50, r3 # position
ld %4, r16 # delta
ld %0, r2 # value
print:
live %-1
sti r2, %0, r3
add r3, r16, r3
add r7, r7, r7
zjmp %:print
