
.globl put32

put32:

    wfe

    b   put32

.globl get32

get32:

    wfe

    b   get32

.globl delay_m

delay_m:

    wfe

    b   delay_m
