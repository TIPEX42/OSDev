global loader						; entry symbol

MAGIC_NUMBER	equ 0x1BADB002		; magic number constant
FLAGS			equ 0x0				; multiboot flags
CHECKSUM		equ -MAGIC_NUMBER	; checksum : (magic number + checksum + flags should equal 0)

KERNEL_STACK_SIZE	equ 4096		; the c stack size


section .text:
align 4
	; writing boot info at the start of the file
	dd MAGIC_NUMBER
	dd FLAGS
	dd CHECKSUM

; entry point
loader:
	mov eax, 0xCAFEBABE
	mov esp, kernel_stack + KERNEL_STACK_SIZE	; setup the stack pointer
	
	extern kmain
	call kmain
.loop:
	hlt
	jmp .loop

section .bss
align 4
kernel_stack:							; begining of the kernel stack
	resb KERNEL_STACK_SIZE				; reserving space for the stack
