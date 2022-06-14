global gdt_finalize
extern gdt_desc

section .text
align 4
gdt_finalize:
    lgdt [gdt_desc]
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:finalize2
finalize2:
    ret