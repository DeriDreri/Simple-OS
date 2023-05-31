[org 0x7c00]
[bits 16]

print_string_rm:
pusha
loop:
;check for string end
mov al, [bx]
cmp al, 0
je end
;print char
mov ah, 0x0e
int 0x10

inc bx
jmp loop

end:
popa
ret

newline_rm:
pusha
mov ah, 0x0e ;print al in tty code
mov al, 0x0a ;newline
int 0x10
mov al, 0x0d ;carriage return (move cursor to start of line)
int 0x10
popa
ret