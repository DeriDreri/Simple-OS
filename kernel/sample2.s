[bits 32]
	mov al, 'B'
    mov ah, 0x0f
    mov [0xb8020], ax
    ret 