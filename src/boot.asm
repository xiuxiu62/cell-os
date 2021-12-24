[BITS 16]
[ORG 0x7c00]

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

_start:
    jmp short start
    nop

times 33 db 0

start:
    jmp 0:step2

step2:
    cli ; Clear interupts
    mov ax, 0x00
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00
    sti ; Enable interupts

.load_protected:
    cli
    lgdt[gdt_descriptor]
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    jmp CODE_SEG:load32

gdt_start:
gdt_null:
    dd 0x0
    dd 0x0

; offset 0x8
gdt_code:       ; cs should point here
    dw 0xffff   ; Segment limit first two bytes
    dw 0x0000   ; Base 0-15        
    db 0x00     ; Base 16-23
    db 0x9a     ; Access byte
    db 0xcf     ; flags
    db 0x00     ; Base 24-31

; offset 0x10
gdt_data:       ; ds, ss, es, fs, gs
    dw 0xffff   ; Segment limit first two bytes
    dw 0x0000   ; Base 0-15        
    db 0x00     ; Base 16-23
    db 0x92     ; Access byte                   
    db 0xcf     ; flags 
    db 0x00     ; Base 24-31

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start-1
    dd gdt_start

[BITS 32]
load32:
    mov eax, 1
    mov ecx, 100
    mov edi, 0x0100000
    call ata_lba_read
    jmp CODE_SEG:0x0100000

ata_lba_read:
    mov ebx, eax    ; Backup LBA

    ; Send upper 8 bits of LBA to hard disk controller
    shr eax, 24     
    or eax, 0xe0    ; Select the main drive
    mov dx, 0x1f6
    out dx, al      

    ; Send total sectors to read
    mov eax, ecx    
    mov dx, 0x1f2
    out dx, al      

    ; Send more of LBA
    mov eax, ebx    ; Restore backup LBA
    mov dx, 0x1f3   
    out dx, al

    ; Send more of LBA
    mov dx, 0x1f4
    mov eax, ebx    ; Restore backup LBA    
    shr eax, 8
    out dx, al

    ; Send upper 16 bits of LBA
    mov dx, 0x1f5
    mov eax, ebx    ; Restore backup LBA    
    shr eax, 16
    out dx, al

    mov dx, 0x1f7
    mov al, 0x20
    out dx, al

; Read all sectors into memory
.next_sector:
    push ecx

; Check if we need to read
.try_again:
    mov dx, 0x1f7
    in al, dx
    test al, 8
    jz .try_again

    ; Read 256 words at a time
    mov ecx, 256
    mov dx, 0x1f0
    rep insw
    pop ecx     ; Restore the sector number
    loop .next_sector
    ret

times 510-($ - $$) db 0
dw 0xaa55









