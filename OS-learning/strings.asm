[org 0x7c00]	

READ equ 0x16
WRITE equ 0x10
ENTER_CODE equ 0x1c
WRITE_AH equ 0x0e

.text:	

start:	
	;Initialises reading
	mov	ax, 0
	mov	bx, buffer

readStringLoop:
	int	READ
	mov	[bx], al

	;Checks if enter was pressed	
	cmp	ah, ENTER_CODE
	je	endOfReading

	;Shows entered characters
	mov	ah, WRITE_AH
	int	WRITE

	;Clears buffers and prepares for next reading
	mov	ax, 0
	inc	bx

	;Checks for buffer limit
	cmp	bx, buffer+50
	je	endOfReading

	jmp 	readStringLoop

endOfReading:
	;Puts into a new line
	mov	ah, WRITE_AH
	mov	bx, buffer
	mov 	al, 10
	int	WRITE
	mov	al, 13
	int 	WRITE

printStringLoop:
	mov	al, [bx]

	;Checks for end of characters
	cmp	al, 0
	je	end

	;Checks for limit of the characters
	cmp	bx, buffer+50
	je	end

	;Writes and moves to another loop stage
	int 	WRITE
	inc 	bx
	jmp	printStringLoop

end:	
	mov	al,10
	int	WRITE
	mov	bx, buffer
	mov	ax, 0
	
cleanBuffer:
	mov	[bx], ax
	inc	bx
	cmp	bx, buffer+51
	jne	cleanBuffer

	jmp 	start			;loops back to the start
	
buffer:
	times 50 db 0

	times	510-($-$$) db 0 	;marking as boot file
	dw	0xaa55
