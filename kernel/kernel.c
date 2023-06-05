#include "symbols.h"
#include "portOperations.c"
#include "keyboard.c"
#include "keycodes.h"
#include <stdbool.h>
#include <stdint.h>


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

void wait();


int cursor_offset;

int main(){
    
    cursor_offset = getCursorPositionOffset();
    
    setStyle(WHITE_ON_BLACK);
    print("Kernel loaded sucessfuly!\n", 0, 0);
    wait();
    write_string_to_memory("Hello, world!", 0);
    //printC(load_from_memory(0), 0, 1);
    //printC(load_from_memory(1), 0, 2);
    wait();
    print((char *) get_memory_address(0), 0, 1);
    //scrollDown();
    //print_head(0);
    wait();
    print_head(2,0);    
    wait();      
    setStyle(GREEN_ON_BLACK);
    wait();
    user_mode();

    return 0;
}

char * getVideoAdress(int column, int row){
    char * toReturn = (char *) (VIDEO_ADDRESS + 2 * (column + row * 80));
    return toReturn;
}

void printC(char character, int column, int row){
    char * address;
    if(column < 0 || row < 0){
        address = (char *) (VIDEO_ADDRESS + cursor_offset);
        row = get_row((int) address);
    }
    else{
        address = getVideoAdress(column, row);
        cursor_offset = (int) address - VIDEO_ADDRESS;
    }

    if (character == '\n'){
        cursor_offset = ((int) getVideoAdress(0, row)) - VIDEO_ADDRESS;
    }
    else{
        address[0] = character;
        cursor_offset += 2;
    }

    set_cursor_offset(cursor_offset);
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

int get_row(int memory_address){
    return (memory_address - VIDEO_ADDRESS) / 80;
}

void scrollDown(){
    int row = 1;
    int col = 0;
    for (row = 1; row < MAX_ROWS; row++){
        for (col = 0; col < MAX_COLS; col++){
            char * addressTo = getVideoAdress(col, row-1);
            char * addressFrom = getVideoAdress(col, row);
            *addressTo = *addressFrom;
        }
    }
    for(col = 0; col < MAX_COLS; col++){
        char * addressTo = getVideoAdress(col, row);
        *addressTo = ' ';
    }
}


void print(char * string, int column, int row){
    char * address;
    if(column < 0 || row < 0){
        address = (char *) (VIDEO_ADDRESS + cursor_offset);
    }
    else{
        address = getVideoAdress(column, row);
        cursor_offset = (int) address - VIDEO_ADDRESS;
    }
    while (*string != 0){
        
        if (*string == '\n'){
            address = (char *) getVideoAdress(0, get_row((int) address)+1);
            cursor_offset = (int) address - VIDEO_ADDRESS;
        }
        else{
            address[0] = *string;
            address += 2;
            cursor_offset += 2;

        }
        string++;
    }
  
    set_cursor_offset(cursor_offset);

}
void print_head(int row, int data_section){
    row--;
    int col = 6;

    for (int i = 0; i < 256; i++){
        unsigned char data = load_from_memory(i+data_section*256);

  
        unsigned char first =  data % 16;
        if(first <= 9){
            first += '0';
        }
        else{
            first += 'A';
            first -= 10;
        }
        unsigned char second =  data / 16;
        if(second <= 9){
            second += '0';
        }
        else{
            second += 'A';
            second -= 10;
        }
        

        if(i % 16 == 0){
            col = 6;
            row++;
            unsigned char sector0 = data_section / 16;
            if(sector0 <= 9){
                sector0 += '0';
            }
            else{
                sector0 += 'A';
                sector0 -= 10;
            }
            printC(sector0, 0, row);
            unsigned char sector = data_section % 16;
            
            if(sector <= 9){
                sector += '0';
            }
            else{
                sector += 'A';
                sector -= 10;
            }
            printC(sector, 1, row);
            unsigned char firstP =  i % 16;
            if(firstP <= 9){
                firstP += '0';
            }
            else{
                firstP += 'A';
                firstP -= 10;
            }
            unsigned char secondP =  i / 16;
            if(secondP <= 9){
                secondP += '0';
            }
            else{
                secondP += 'A';
                secondP -= 10;
            }
            printC(secondP, 2, row);
            printC(firstP, 3, row);
        }
        printC(second, col++, row);
        printC(first, col++, row);
        printC(' ', col++, row);
        
    }
    printC('\n', col, row+1);
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
            break;

            default:
            for (unsigned int i = 0; i < 100000; ++i) {
            asm("nop"); 
            }
            break;
        }
    } 
}