.set IRQ_BASE, 0x20
.section .text

.extern _ZN16InterruptManager15handleInterruptEhj

.global _ZN16InterruptManager22IgnoreInterruptRequestEv
.macro HandleInterruptRequst num 
.global _ZN16InterruptManager26HandleInterruptRequest\num\()Ev
_ZN16InterruptManager26HandleInterruptRequest\num\()Ev:
    movb $\num + IRQ_BASE, (interruptnumber)
    jmp int_bottom
.endm

HandleInterruptRequst 0x00

int_bottom:
    pusha            
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs

    pushl %esp
    push (interruptnumber)
    call _ZN16InterruptManager15handleInterruptEhj
    #pop (interruptnumber)
    #popl %esp
    movl %eax, %esp

    popl %gs
    popl %fs
    popl %es
    popl %ds 
    popa

_ZN16InterruptManager22IgnoreInterruptRequestEv:
    iret 

.data
    interruptnumber: .byte 0
