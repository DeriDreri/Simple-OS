BUILD_DIR = build
BOOTLOADER=$(BUILD_DIR)/bootloader.o
KERNEL=$(BUILD_DIR)/kernel.o

.PHONY:all bootloader kernel combine

all: bootloader kernel combine

bootloader:
	nasm bootloader/bootloader.asm -f bin -o $(BUILD_DIR)/bootloader.bin

kernel:
	gcc -fno-pie -ffreestanding -c kernel/kernel.c -m32 -o $(BUILD_DIR)/kernel.o
	gcc -fno-pie -ffreestanding -c interrupts/idt.c -m32 -o $(BUILD_DIR)/idt.o
	gcc -fno-pie -ffreestanding -c interrupts/isr.c -m32 -o $(BUILD_DIR)/isr.o
	nasm interrupts/interrupts.asm -f elf32 -o $(BUILD_DIR)/interrupts.o
	nasm kernel/kernel_entry.asm -f elf32 -o $(BUILD_DIR)/kernel_entry.o
	as kernel/memory_operations.asm --32 -o $(BUILD_DIR)/memory.o
	
	ld -m elf_i386 -o $(BUILD_DIR)/kernel.bin -Ttext 0x1000 $(BUILD_DIR)/kernel_entry.o $(BUILD_DIR)/memory.o $(BUILD_DIR)/kernel.o $(BUILD_DIR)/idt.o $(BUILD_DIR)/isr.o $(BUILD_DIR)/interrupts.o --oformat binary
	
	
combine: 
	cat $(BUILD_DIR)/bootloader.bin $(BUILD_DIR)/kernel.bin > $(BUILD_DIR)/disk_img.bin

qemu:
	qemu-system-i386 -machine q35 -fda $(BUILD_DIR)/disk_img.bin

clear:
	rm $(BUILD_DIR)/*