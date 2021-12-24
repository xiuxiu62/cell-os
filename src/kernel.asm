[BITS 32]
global _start

CODE_SEG equ 0x08
DATA_SEG equ 0x10

_start:
load32:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov ebp, 0x00200000
    mov esp, ebp

enable_a20:
    in al, 0x92
    or al, 2
    out 0x92, al

.loop:
    jmp $
