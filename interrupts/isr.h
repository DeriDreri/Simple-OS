#ifndef ISR_H
#define ISR_H

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

typedef struct
{
    unsigned int ds; //data segment selector
    unsigned int edi, esi, ebp, esp, edx, ecx, ebx, eax;
    unsigned int int_num, err_code; //interrupt number, error code
    unsigned eip, cs, eflags, useresp, ss; //pushed automatically with an interrupt
}registers;

void isr_install();
void isr_handler(registers r);

#endif