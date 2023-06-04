#define REG_SCREEN_CTRL 0x3D4
#define REG_SCREEN_DATA 0x3D5
#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0f

extern void write_to_memory(int, char);
extern char load_from_memory(int);
extern int get_memory_address(int);
char * getVideoAdress(int, int);
void print(char *, int, int);
void printC(char, int, int);
void clearScreen();

int main(){
    
    print("Kernel loaded sucessfuly!", 0, 0);



    write_to_memory(0, 'A');
    write_to_memory(1, 'B');
    write_to_memory(2, 'C');
    write_to_memory(3, 'D');
    write_to_memory(4, 0);
    printC(load_from_memory(0), 0, 1);
    printC(load_from_memory(1), 0, 2);
    print((char *) get_memory_address(0), 0, 3);
    

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


void print(char * string, int column, int row){
    char * address = getVideoAdress(column, row);
    while (*string != 0){
        address[0] = *string;
        string++;
        address += 2;
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