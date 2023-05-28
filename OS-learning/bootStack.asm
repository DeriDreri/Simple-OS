[org 0x7c00]
BIOS_WRITE equ 0x10
WRITE_AH equ 0x0e

numbers:
	db	10,20,30,15,25,20

	mov	bx, numbers
	mov	cx, 5
	call	printIntegers	

	jmp	$			;endless loop at the end of the code




;Function reading text from 

;Function that prints ASCII characters placed at bx with length at cx
print:
	mov	ah, WRITE_AH
printLoop:
	mov	al, [bx]
	int	BIOS_WRITE
	
	inc	bx
	dec	cx

	cmp	cx, 0
	jne	prinsLoop
	
	ret 

	times	510-($-$$) db 0 	;marking as boot file
	db	0x55, 0xaa
