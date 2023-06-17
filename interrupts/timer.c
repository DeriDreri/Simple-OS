#include "timer.h"
#include "isr.h"
#include "../kernel/portOperations.h"
#include "../kernel/print.h"
extern void print(char * string, int column, int row);
unsigned int tick = 0;

static void timer_callback(registers r)
{
    tick++;
}

void print_time()
{
    print("Ticks since start: ", 0, 0);
    int i = 0;
    int ticks = tick;
    char string[256];
    do
    {
       string[i++] = (ticks % 10) + '0';
    } while ((ticks /= 10) > 0);

    char toPrint[i];
    int j = i - 1;
    int k;
    for(k = 0; k < i; k++)
    {
        toPrint[k] = string[j];
        j--;
    }
    print(toPrint, 19, 0);
    
}

void init_timer(unsigned int freq)
{
    register_interrupt_handler(IRQ0, timer_callback);
    unsigned int divisor = 1193180 / freq;
    port_byte_out(0x43, 0x36); //command port
    unsigned char low = (unsigned int)(divisor & 0xFF);
    unsigned char high = (unsigned int)((divisor >> 8) & 0xFF);
    port_byte_out(0x40, low);
    port_byte_out(0x40, high);
}