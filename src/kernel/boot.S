#include <common/sysreg.h>

.section ".text.boot"

.global _start

_start:

    mrs     x1, mpidr_el1
    and     x1, x1, #3
    cbz     x1, 2f
    cbz     x0 ,  master

master:

    ldr     x0, =SCTLR_VALUE_MMU_DISABLED
    msr     sctlr_el1 , x0

    ldr     x0 , =HCR_VALUE
    msr     hcr_el2 , x0

    ldr     x0 , =SCR_VALUE 
    msr     scr_el3 , x0

    ldr     x0 , =SPSR_VALUE
    msr     spsr_el3 , x0 

    adr     x0 , 3
    msr     elr_el3, x0

    eret

1:  wfe
    
    b       1b

2:  

    ldr     x1, =_start
    mov     sp, x1

    ldr     x1, =__bss_start
    ldr     w2, =__bss_size

3:  cbz     w2, 4f
    str     xzr, [x1], #8
    sub     w2, w2, #1
    cbnz    w2, 3b


4:  bl      kernel_main

    b       1b


.globl get_el

get_el:

    mrs x0 , CurrentEL
    lsr x0 , x0 , #2
    ret