#include "symbols.h"
#include "portOperations.c"
#include "keyboard.c"
#include "keycodes.h"
#include <stdbool.h>
#include <stdint.h>
#include "../interrupts/isr.h"
#include "../interrupts/idt.h"
#include "print.c"
#include "../interrupts/timer.h"


extern void write_to_memory(int, unsigned char);
extern char load_from_memory(int);
extern int get_memory_address(int);
char * getVideoAdress(int, int);
void print(char *, int, int);
void printC(char, int, int);
void clearScreen();
void scrollDown();
void write_string_to_memory(char * string, int memory_a771ddress);
void print_head(int, int);
int getCursorPositionOffset();
void set_cursor_offset(int offset);
void setStyle(unsigned char style);
unsigned long long get_elapsed_time();
int get_row(int memory_address);
void user_mode();
unsigned char byteToHexChar(unsigned char);
extern void print_time();

void wait();

int cursor_offset;

int main(){
    
    cursor_offset = getCursorPositionOffset();
    irq_install();
    isr_install();

    print("Kernel loaded sucessfuly!\n", 0, 0);
    
    
    __asm__ volatile ("hlt");
}


void write_string_to_memory(char * string, int memory_address){
    char * address = (char *) get_memory_address(memory_address);
    while(1){
        if(*string == 0){
            *address = *string;
            break;
        }
        *address = *string;
        address++;
        string++;
     
    }


}

void setStyle(unsigned char style){
    int row = 0;
    int col = 0;
    for (row = 0; row < MAX_ROWS; row++){
        for (col = 0; col < MAX_COLS; col++){
            char * address = (char *) getVideoAdress(col, row);
            address[1] = style;
        }
    } 
}





void print_head(int row, int data_section){
    row--;
    int col = 6;

    for (int i = 0; i < 256; i++){
        unsigned char data = load_from_memory(i+data_section*256);

  
        unsigned char first =  data % 16;
        first = byteToHexChar(first);
        unsigned char second =  data / 16;
        second = byteToHexChar(second);
        

        if(i % 16 == 0){
            col = 6;
            row++;
            unsigned char sector0 = data_section / 16;
            sector0 = byteToHexChar(sector0);
            printC(sector0, 0, row);
            unsigned char sector = data_section % 16;
            sector = byteToHexChar(sector);
            printC(sector, 1, row);
            unsigned char firstP =  i % 16;
            firstP = byteToHexChar(firstP);
            unsigned char secondP =  i / 16;
            secondP = byteToHexChar(secondP);
            printC(secondP, 2, row);
            printC(firstP, 3, row);
            printC(' ', 4, row);
            printC(' ', 5, row);
        }
        printC(second, col++, row);
        printC(first, col++, row);
        printC(' ', col++, row);
        
    }
    printC('\n', col, row+1);
}

unsigned char byteToHexChar(unsigned char byte){
    unsigned char toReturn = byte;
    if(toReturn <= 9){
        toReturn += '0';
    }
    else{
        toReturn += 'A';
        toReturn -= 10;
    }
    return toReturn;
}

void clearScreen(){
    int row = 0;
    int col = 0;
    for (row = 0; row < MAX_ROWS; row++){
        for (col = 0; col < MAX_COLS; col++){
            print(" ", col, row);
        }
    }
}



unsigned long long get_elapsed_time() {
    unsigned int low, high;
    unsigned long long time;

    __asm__ __volatile__("rdtsc" : "=a" (low), "=d" (high));
    time = ((unsigned long long)high << 32) | low;

    return time;
}

void input_mode(){
    char * address = (char *) get_memory_address(0);
    char * videoAddress = (char *) VIDEO_ADDRESS + cursor_offset;
    bool loop = true;
    bool big = true;
    unsigned char key = 0;
    char character;
    int start_offset = cursor_offset;
    while (loop){
        for (unsigned int i = 0; i < 100000; ++i) {
            asm("nop"); 
        }
        key = read_key();
        if(key == 0) 
            continue;
        if(key == 0x3a){ // CAPS_LOCK
            big = !big;
            continue;
        } 
        if(key == 0x1c){ //ENTER
            *address = 0;
            loop = false;
            break;
        }
        if(key == 0x0e){ 
            if(cursor_offset == start_offset) 
                continue;
            address--;
            *address = 0;
            cursor_offset -= 2;
            set_cursor_offset(cursor_offset);
            printC(' ', -1, -1);
            cursor_offset -= 2;
            set_cursor_offset(cursor_offset);
            continue;
        }
        if(!check_if_letter_key(key)){
            continue;
        }
        for(int i = 0; i < 27; i++){
            if(keycodes[i].code == key){
                character = keycodes[i].symbol;
                if(!big && character != ' ')
                    character += 32;
               
                break;
            }
        }
        *address = character;
        address++;
        printC(character, -1, -1);
    }

}

void user_mode(){
    unsigned char key = 0;
    bool loop = true;
    int segment = 0;
    clearScreen();
    print("User input mode, enter command\n", 0, 0);
    while(loop){
        key = read_key();
        switch(key){
            case 0x23: // H
            segment = 0;
            print_head(1, segment);
            break;

            case 0x34: // >
            if (segment < 255)
                segment++;
            print_head(1, segment);
            break;

            case 0x33: // <
            if (segment > 0)
                segment--;
            print_head(1, segment);
            break;

            case 0x2e: // C
            clearScreen();
            print("User input mode, enter command\n", 0, 0);
            break;

            case 0x01: // ESC
            loop = false;
            clearScreen();
            break;

            case 0x19: // P
            clearScreen();
            print("User input mode, enter command\n", 0, 0);
            print((char *) get_memory_address(0), 0, 1);
            break;

            case 0x1f: // S
            clearScreen();
            print("User input mode, enter command\n", 0, 0);
            print("Write to memory:\n", 0, 1);
            input_mode();
            clearScreen();
            print("User input mode, enter command\n", 0, 0);
            break;

            case 0x0e: // BACKSPACE
            scrollDown();
            break;

            case 0x14: //T
            clearScreen();
            print_time();
            wait();
            clearScreen();
            print("User input mode, enter command\n", 0, 0);
            break;

            default:
            for (unsigned int i = 0; i < 100000; ++i) {
            asm("nop"); 
            }
            break;
        }
    } 
}