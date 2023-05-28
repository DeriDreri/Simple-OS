[org 0x7c00]
[bits 16]

section .text
boot:
	cli ; no interrupts
	cld ; all that we need to init
	mov ax, 0x50
;; set the buffer
	mov es, ax
	xor bx, bx
;	mov al, 2 ; read number of sectors
;	mov ch, 0 ; track 0
;	mov cl, 2 ; sector to read (The second sector)
;	mov dh, 0 ; head number
;	mov dl, 0 ; drive number
;	mov ah, 0x02 ; read sectors from disk
;	int 0x13 ; call the BIOS routine

; clear the screen
	mov ah, 0x00
	mov al, 0x03
	int 0x10

	cli
	lgdt[gdt_descriptor]
	mov eax, cr0
	or al, 0x01
	mov cr0, eax

	jmp CODE_SEG:_main32
	%include "bootloader/gdt.asm"

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

	mov al, 'A'
    mov ah, 0x0f
    mov [0xb8000], ax 

	mov al, 'B'
    mov ah, 0x0f
    mov [0xb8010], ax 

;	jmp 0x50:0x00 ; jump to kernel location


	jmp	$

times	510-($-$$) db 0 	;marking as boot file
db	0x55, 0xaa	
