
.set  MAGIC,0x1badb002
.set  FLAGS,(1 << 0 | 1 << 1)
.set CHECKNUM,  -(MAGIC+  FLAGS)


.section .multiboot
    .long MAGIC
    .long FLAGS
    .long CHECKNUM

.section .text
.extern kernelMain
.extern callConstructors
.global loader



loader:
    mov $kernel_stack,%esp #内存栈的开始地址传送给栈顶指针esp#
    call callConstructors
    push %eax  #调用callConstructors的返回值
    push %ebx  

    call kernelMain 

_stop:
    cli
    hlt
    jmp _stop
    

.section .bss
.space 2*1024*1024 #从高地址到低使用栈#
kernel_stack:
