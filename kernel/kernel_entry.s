.text
.global kernel_start
kernel_start:
    mov $'B', %al
    mov $0x0f, %ah
    mov %ax, 0xb8000
   
    call main
loop:
    jmp loop
