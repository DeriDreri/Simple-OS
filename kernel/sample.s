	mov	ah, 0x0e		;switch to teletype mode
	mov	al, 'A'			;insert character to print
	int	0x10			;call bios interruption

loop:					;loop printing the whole alphabet
	inc	al

	cmp	al, 0x60
	int	0x10

	cmp	al, 'Z'
	je	end
	cmp	al, 'z'
	je	end
	jmp 	loop

end:
    jmp $