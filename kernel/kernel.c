#define REG_SCREEN_CTRL 0x3D4
#define REG_SCREEN_DATA 0x3D5
#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0f
#define DATA_LIMIT 65536
#include "portOperations.c"

extern void write_to_memory(int, char);
extern char load_from_memory(int);
extern int get_memory_address(int);
char * getVideoAdress(int, int);
void print(char *, int, int);
void printC(char, int, int);
void clearScreen();
void scrollDown();
void write_string_to_memory(char * string, int memory_address);
void print_head(int, int);
int getCursorPositonOffset();

int main(){
    
    print("Kernel loaded sucessfuly!", 0, 0);
    //write_string_to_memory("Hello, world!", 0);
    //printC(load_from_memory(0), 0, 1);
    //printC(load_from_memory(1), 0, 2);
    //print((char *) get_memory_address(0), 0, 3);
    //scrollDown();
    //print_head(0);
    print_head(1,0);


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
        int data = ( int) load_from_memory(i+data_section*256);

        if(data < 0)
            data = 256 + data;
        int first =  data % 16;
        if(first <= 9){
            first += '0';
        }
        else{
            first += 'A';
            first -= 10;
        }
        int second =  data / 16;
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