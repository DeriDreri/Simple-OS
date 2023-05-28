	mov	ah, 0x0e		;switch to teletype mode
	mov	al, 'A'			;insert character to print
	int	0x10			;call bios interruption

loop:					;loop printing the whole alphabet
	inc	al

	cmp	al, 0x60
	jl	big_to_small
	jmp	small_to_big
return:

	int	0x10

	cmp	al, 'Z'
	je	end
	cmp	al, 'z'
	je	end
	jmp 	loop

small_to_big:
	sub	al, 0x20
	jmp 	return

big_to_small:
	add	al, 0x20
	jmp	return

end:

	jmp	$			;endless boot loop
	times	510-($-$$) db 0
	db	0x55, 0xaa
