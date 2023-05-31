#define REG_SCREEN_CTRL 0x3D4
#define REG_SCREEN_DATA 0x3D5
#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0f

char * getVideoAdress(int, int);
void print(char *, int, int);
void clearScreen();

int main(){
    
    print("Kernel loaded sucessfuly!", 0, 0);

    return 0;
}

char * getVideoAdress(int column, int row){
    char * toReturn = (char *) (VIDEO_ADDRESS + 2 * (column + row * 80));
    return toReturn;
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