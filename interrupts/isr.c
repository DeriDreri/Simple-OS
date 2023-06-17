#include "isr.h"
#include "idt.h"
extern void prints();
void isr_install()
{
    set_idt_gate(0, (unsigned)isr0);
    set_idt_gate(1, (unsigned)isr1);
    set_idt_gate(2, (unsigned)isr2);
    set_idt_gate(3, (unsigned)isr3);
    set_idt_gate(4, (unsigned)isr4);
    set_idt_gate(5, (unsigned)isr5);
    set_idt_gate(6, (unsigned)isr6);
    set_idt_gate(7, (unsigned)isr7);
    set_idt_gate(8, (unsigned)isr8);
    set_idt_gate(9, (unsigned)isr9);
    set_idt_gate(10, (unsigned)isr10);
    set_idt_gate(11, (unsigned)isr11);
    set_idt_gate(12, (unsigned)isr12);
    set_idt_gate(13, (unsigned)isr13);
    set_idt_gate(14, (unsigned)isr14);
    set_idt_gate(15, (unsigned)isr15);
    set_idt_gate(16, (unsigned)isr16);
    set_idt_gate(17, (unsigned)isr17);
    set_idt_gate(18, (unsigned)isr18);
    set_idt_gate(19, (unsigned)isr19);
    set_idt_gate(20, (unsigned)isr20);
    set_idt_gate(21, (unsigned)isr21);
    set_idt_gate(22, (unsigned)isr22);
    set_idt_gate(23, (unsigned)isr23);
    set_idt_gate(24, (unsigned)isr24);
    set_idt_gate(25, (unsigned)isr25);
    set_idt_gate(26, (unsigned)isr26);
    set_idt_gate(27, (unsigned)isr27);
    set_idt_gate(28, (unsigned)isr28);
    set_idt_gate(29, (unsigned)isr29);
    set_idt_gate(30, (unsigned)isr30);
    set_idt_gate(31, (unsigned)isr31);
    set_idt();
}

char *exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};


void isr_handler(registers r)
{   
    prints("received interrupt: ");
    prints("\n");
    prints(exception_messages[r.int_num]);
    prints("\n");
    //TODO: print error message to screen (interrupt number = r.int_num)
}