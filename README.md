# Simple-OS

The project is a part of Architektura Komputerów 2 (Computer Architecture 2) class on WrocTech's Computer Engineering field realised by Dominik Cybulski (DeriDreri) and Olga Tryk.
The current representant of the group is Dominik Cybulski.

The projects aims to create a simple operating system avaliable to be booted on qemu simulator. The functions of the OS include:
- Functional bootloader
- 32-bit protected mode
- Memory segmentation
- Memory managment
- Video management
- Hardware interrupts

The project requires a Linux system with qemu simulator, nasm and as assemblers and gcc installed to be possible to compile. Compilation process is simplified with the use of MakeFile and requires to run a simple command in the project folder: 

```
make all
```
The command creates a bootable file "disk_img.bin" in the "build" directory. To run the operating system in simulation, stay in the main project directory and use command:
```
make qemu
```
It is also possible and easy to run the simulation by hand by changing directory to "build" directory and using command similiar to:
```
qemu-system-x86_64 -fda disk_img.bin
```
Use of -fda parameter is required as the system is built to be read as a floppy disc by the machine.

## Using OS at the current state
The current version of OS is above everything else a demo version with few functions meant to showcase the inner working of the code. 

During the boot process in qemu simulator, the program display messages informing of the bootloader's operations - the user has to press Enter to continue untill sucessful loading of the Kernel.

Kernel welcomes the user and demonstrates a script with few functions of the operating system - such as writing to the memory, printing message from the memory and printing a map of first 256 bytes of the memory. Kenrel then enters "User Input Mode", where it awaits for the user to press a command key.

Current commands include:
- H - Pritning first 256 bytes of memory on the screen as hexmap
- < and > - Navigating through the memory parts separated into 256 bytes and priting them as hexmap on the screen
- S - Opening a simple text editor which allows to input the letters codes in ASCII and save it at the start of the memory by pressing ENTER. The only way to switch between upper case and lower case letters is by using CAPS LOCK. Special signs are not implemented.
- P - Printing the first bytes of memory as string
- ESC - Halting the system
