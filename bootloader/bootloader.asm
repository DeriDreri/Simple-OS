[org 0x7c00]
[bits 16]

section .data 
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

gdt_start:
gdt_null:
	dd 0x00
	dd 0x00
gdt_code:
	dw 0xFFFF
	dw 0x00
	db 0x00
	db 10011010b
	db 11001111b
	db 0x00
gdt_data:
	dw 0xFFFF
	dw 0x00
	db 0x00
	db 10010010b
	db 11001111b
	db 0x00
gdt_end:
gdt_descriptor:
	dw gdt_end - gdt_start - 1
	dd gdt_start

section .text
boot:
	cli ; no interrupts
	cld ; all that we need to init
	mov ax, 0x50
;; set the buffer
	mov es, ax
	xor bx, bx
	mov al, 35 ; read number of sectors
	mov ch, 0 ; track 0
	mov cl, 2 ; sector to read (The second sector)
	mov dh, 0 ; head number
	mov dl, 0 ; drive number
	mov ah, 0x02 ; read sectors from disk
	int 0x13 ; call the BIOS routine

	cli
	lgdt[gdt_descriptor]
	mov eax, cr0
	or al, 0x01
	mov cr0, eax
	jmp CODE_SEG:_main32


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

	jmp 0x50:0x0 ; jump to kernel location


	jmp	$

times	510-($-$$) db 0 	;marking as boot file
db	0x55, 0xaa	
