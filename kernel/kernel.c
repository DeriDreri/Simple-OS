#define REG_SCREEN_CTRL 0x3D4
#define REG_SCREEN_DATA 0x3D5
#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0f
#define GREEN_ON_BLACK 0x02
#define WHITE_ON_RED 0x47
#define RED_ON_BLACK 0x04
#define DATA_LIMIT 65536
#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_DATA_PORT 0x60
#include "portOperations.c"
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

void wait();


int cursor_offset;

int main(){
    
    cursor_offset = getCursorPositionOffset();
    
    setStyle(WHITE_ON_BLACK);
    print("Kernel loaded sucessfuly!", 0, 0);
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
    print_head(0,0);

    return 0;
}

char * getVideoAdress(int column, int row){
    char * toReturn = (char *) (VIDEO_ADDRESS + 2 * (column + row * 80));
    return toReturn;
}

void printC(char characater, int column, int row){
    char * address = getVideoAdress(column, row);
        address[0] = characater;
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
    char * address = getVideoAdress(column, row);
    while (*string != 0){
        address[0] = *string;
        string++;
        address += 2;
    }

}
void print_head(int row, int data_section){
    row--;
    int col = 0;

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
            col = 0;
            row++;
        }
        printC(second, col++, row);
        printC(first, col++, row);
        printC(' ', col++, row);
        
    }
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

int getCursorPositionOffset(){
    port_byte_out(REG_SCREEN_CTRL, 14); /* Requesting byte 14: high byte of cursor pos */
    /* Data is returned in VGA data register (0x3d5) */
    int position = port_byte_in(REG_SCREEN_DATA);
    position = position << 8; /* high byte */

    port_byte_out(REG_SCREEN_CTRL, 15); /* requesting low byte */
    position += port_byte_in(REG_SCREEN_DATA);
    int offset_from_vga = position * 2;
    return offset_from_vga;
}

void set_cursor_offset(int offset) {
    /* Similar to get_cursor_offset, but instead of reading we write data */
    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
}

unsigned long long get_elapsed_time() {
    unsigned int low, high;
    unsigned long long time;

    __asm__ __volatile__("rdtsc" : "=a" (low), "=d" (high));
    time = ((unsigned long long)high << 32) | low;

    return time;
}

bool is_key_pressed()
{
    // Check the keyboard status port (0x64) to determine if a key is pressed
    // Bit 0 of the status port will be set if there is data available
    unsigned char status = port_byte_in(KEYBOARD_STATUS_PORT);
    return (status & 0x01);  // Check the least significant bit
}

unsigned char read_key()
{
    // Wait for a key press by continuously checking the keyboard status
    while (!is_key_pressed()) {
   
    }

    // Read the key code from the data port (0x60)
    unsigned char key = port_byte_in(KEYBOARD_DATA_PORT);
    return key;
}

void wait_for_keypress(unsigned char keyCode)
{   
    for (unsigned int i = 0; i < 1000000; ++i) {
            asm("nop");
        }
    unsigned char key = 0;
    do {
        for (unsigned int i = 0; i < 100000; ++i) {
            asm("nop"); 
        }
        key = read_key();
    } while (key != keyCode);
}
void wait(){
    wait_for_keypress(0x1C);
    //wait_for_keypress();
}