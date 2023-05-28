end:

	jmp	$			;endless loop at the end of the code
	times	510-($-$$) db 0		;marking as boot file
	db	0x55, 0xaa
