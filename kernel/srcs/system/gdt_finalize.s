global gdt_finalize
extern desc


section .text
align 4
gdt_finalize:
    lgdt [desc]
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:finalize2
finalize2:
    ret