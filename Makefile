BUILD_DIR = build
BOOTLOADER=$(BUILD_DIR)/bootloader.bin
KERNEL=$(BUILD_DIR)/kernel.bin
DISK_IMG=$(BUILD_DIR)/disk.img

.PHONY: all bootdisk bootloader kernel

all: bootdisk

bootloader:
	nasm bootloader/bootloader.asm -f bin -o $(BUILD_DIR)/bootloader.bin

kernel:
	gcc -ffreestanding -c kernel/kernel.c  -m32 -o build/kernel.o
	as kernel/kernel_entry.s -o build/kernel_entry.o 
	ld -o $(KERNEL) -Ttext 0x0 --format binary $(BUILD_DIR)/kernel.o $(BUILD_DIR)/kernel_entry.o

bootdisk: bootloader kernel
	dd if=/dev/zero of=$(DISK_IMG) bs=512 count=2880
	dd conv=notrunc if=$(BOOTLOADER) of=$(DISK_IMG) bs=512 count=1 seek=0
	dd conv=notrunc if=$(KERNEL) of=$(DISK_IMG) bs=512 count=40 seek=1

qemu:
	qemu-system-i386 -machine q35 -fda $(DISK_IMG)

clear:
	rm $(BUILD_DIR)/*