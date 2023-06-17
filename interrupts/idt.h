#ifndef IDT_H
#define IDT_H

#define KERNEL_CS 0x08; //code segment selector
typedef struct
{
    unsigned short int low_offset;
    unsigned short int selector;
    unsigned char zero; //always set to 0
    unsigned char flags;
    /*
    Flags byte:
    bit 7 - interrupt is present = 1
    bits 6-5 - priviledge level (0 for kernel)
    bit 4 - set to 0 for interrupt gates
    bits 3-0 - 32-bit interrupt gate = 1110
    */
    unsigned short int high_offset;
}__attribute__((packed)) idt_gate_t;

typedef struct
{
    unsigned short int limit;
    unsigned int base;
}__attribute__((packed)) idt_reg_t;

void set_idt_gate(int number, int handler);
void set_idt();
#endif