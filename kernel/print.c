#include "print.h"
#include "symbols.h"
extern int cursor_offset;

int get_row(int memory_address){
    return (memory_address - VIDEO_ADDRESS) / 80;
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

void prints(char * string){
    print(string, -1, -1);
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
    row = get_row(cursor_offset+VIDEO_ADDRESS);
    if(row > 0){
        cursor_offset -= 160;
        set_cursor_offset(cursor_offset);
    }
}