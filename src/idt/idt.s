section .asm

extern no_interrupt_handler
extern int21h_handler

global idt_load
global no_interrupt
global int21h

idt_load:
    push ebp
    mov ebp, esp

    mov ebx, [ebp+8]
    lidt [ebx]
    pop ebp
    ret

no_interrupt:
    cli
    pushad
    call no_interrupt_handler
    popad
    sti
    iret

; Keyboard interrupt
int21h:
    cli
    pushad
    call int21h_handler
    popad
    sti
    iret
    

