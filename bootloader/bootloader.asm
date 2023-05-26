[org 0x7c00]
BIOS_WRITE equ 0x10
WRITE_AH equ 0x0e

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
	jmp 0x50:0x0 ; jump and execute the sector!

	jmp	$

times	510-($-$$) db 0 	;marking as boot file
db	0x55, 0xaa	
