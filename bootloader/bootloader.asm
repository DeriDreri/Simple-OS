[org 0x7c00]
KERNEL equ 0x1000
ENTER_ASCII equ 0x1c
[bits 16]
section .text
	cli ; no interrupts
	cld ; all that we need to init
	;clear screen

	mov bx, MSG_REAL_MODE
	call print_string_rm
	call newline_rm

	mov bx, KERNEL
	mov al, 50 ; read number of sectors
	mov ch, 0 ; track 0
	mov cl, 2 ; sector to read (The second sector)
	mov dh, 0 ; head number
	;mov dl, 0 ; drive number
	mov ah, 0x02 ; read sectors from disk
	int 0x13 ; call the BIOS routine
	
	; clear the screen
	mov ah, 0x00
	mov al, 0x03
	int 0x10

	mov bx, MSG_LOAD_KERNEL
	call print_string_rm
	call newline_rm

	lgdt[gdt_descriptor]
	mov bx, MSG_GDT
	call print_string_rm
	call newline_rm
	;wait for key press
	mov ah, 0
	int 0x16

	mov ah, 0x00
	mov al, 0x03
	int 0x10

	mov eax, cr0
	or al, 0x01
	mov cr0, eax


	jmp CODE_SEG:_main32
	%include "bootloader/gdt.asm"
	%include "bootloader/print_rm.asm"

[bits 32]
_main32:
	;set up segment registers again
	mov ax, DATA_SEG
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	;set up the stack
	mov ebp, 0x90000
	mov esp, ebp
	
	;enable the A20 line
	in al, 0x92
	or al, 0x02
	out 0x92, al

	;mov al, 'A'
    ;mov ah, 0x0f
    ;mov [0xb8000], ax  

	jmp KERNEL ; jump to kernel location



	jmp	$

;print in real mode
MSG_REAL_MODE db "Started in 16-bit real mode", 0 
MSG_LOAD_KERNEL db "Loaded kernel into memory", 0
MSG_GDT db "Loaded Global Descriptor Table", 0
;print in protected mode
MSG_PROT_MODE db "Entered 32-bit protected mode", 0

times	510-($-$$) db 0 	;marking as boot file
db	0x55, 0xaa	
