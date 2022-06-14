section .text
align 4

global load_idt
extern idt_desc
load_idt:
	lidt [idt_desc]		; load the IDT
	ret					; return to the calling function

%macro no_error_code_interrupt_handler 1
global interrupt_handler_%1
interrupt_handler_%1:
	push dword 0					; push 0 as error code
	push dword %1					; push the interrupt number
	jmp common_interrupt_handler	; jump to the common handler
%endmacro

%macro error_code_interrupt_handler 1
global interrupt_handler_%1
interrupt_handler_%1:
	push dword %1					; push the interrupt number
	jmp common_interrupt_handler	; jump to the common handler
%endmacro


extern interrupt_handler
common_interrupt_handler:			; the common parts of the generic interrupt handler
	; save the registers
	push eax
	push ebx
	push ecx
	push edx
	push esi
	push edi
	push ebp
	push esp

	; call the C function
	call interrupt_handler

	; restore the registers
	pop eax
	pop ebx
	pop ecx
	pop edx
	pop esi
	pop edi
	pop ebp
	pop esp

	; restore the esp
	add esp, 8

    ; return to the code that got interrupted
	iret

no_error_code_interrupt_handler 0
no_error_code_interrupt_handler 1

error_code_interrupt_handler 7