#include "../interrupts/isr.h"
#include "../interrupts/idt.h"

int main(){
    isr_install();
    return 0;
}