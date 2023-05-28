
.text
.global kernel_start
kernel_start:
    mov $'A', %al
    mov $0x0f, %ah
    mov %ax, 0xb8000
   
    call main
