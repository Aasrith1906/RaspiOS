
.globl put32

put32:

    str r1 , [r0]

    bx  lr

.globl get32

get32:

    ldr r1 , [r0]

    bx  lr

.globl delay_m

delay_m:

    str r1 , [r0]

    sub r1 , r1 , #1

    bne delay_m

    bx  lr