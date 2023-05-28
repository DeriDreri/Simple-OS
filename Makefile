BUILD_DIR = build
BOOTLOADER=$(BUILD_DIR)/bootloader.o
KERNEL=$(BUILD_DIR)/kernel.o
DISK_IMG=$(BUILD_DIR)/disk.img

.PHONY: all bootdisk bootloader kernel

all: bootdisk

bootloader:
	nasm bootloader/bootloader.asm -f bin -o $(BUILD_DIR)/bootloader.o

kernel:
	gcc kernel/kernel.cpp kernel/kernel_entry.s -m32 -o $(BUILD_DIR)/kernel.o 

bootdisk: bootloader kernel
	dd if=/dev/zero of=$(DISK_IMG) bs=512 count=2880
	dd conv=notrunc if=$(BOOTLOADER) of=$(DISK_IMG) bs=512 count=1 seek=0
	dd conv=notrunc if=$(KERNEL) of=$(DISK_IMG) bs=512 count=1 seek=1

qemu:
	qemu-system-i386 -machine q35 -fda $(DISK_IMG)

clear:
	rm $(BUILD_DIR)/*
	rm disk.img