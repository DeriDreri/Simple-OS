#include "idt.h"

idt_gate_t idt[256];
idt_reg_t idt_reg;

void set_idt_gate(int n, int handler)
{
    idt[n].low_offset = (unsigned short)((handler) & 0xFFFF);
    idt[n].selector = KERNEL_CS;
    idt[n].zero = 0;
    idt[n].flags = 0b10001110;
    idt[n].high_offset = (unsigned short)(((handler) >> 16) & 0xFFFF);
}

void set_idt()
{
    idt_reg.base = (unsigned int) &idt;
    idt_reg.limit = 256 * sizeof(idt_gate_t) - 1;
    __asm__ __volatile__ ("lidt (%0)" :: "r" (&idt_reg));
}