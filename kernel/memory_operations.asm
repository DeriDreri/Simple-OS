.global write_to_memory
.global load_from_memory
.global get_memory_address
.text

write_to_memory:
    push    %ebp
    mov     %esp, %ebp

    mov     8(%ebp), %ebx
    mov     12(%ebp), %dl
    mov     %ds, %eax
    add     %eax, %ebx

    mov     %dl, (%ebx)

    leave
    ret

load_from_memory:
    push    %ebp
    mov     %esp, %ebp

    mov     8(%ebp), %ebx
    mov     %ds, %eax
    add     %eax, %ebx
    mov     (%ebx), %ax

    leave
    ret

get_memory_address:
    push    %ebp
    mov     %esp, %ebp

    mov     8(%ebp), %ebx
    mov     %ds, %eax
    add     %ebx, %eax

    leave
    ret

