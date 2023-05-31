char * getVideoAdress(int, int);
void print(char *, int, int);

int main(){
    //*(char*)0xb8000 = 'Q';  
    char* video_memory = (char*)0xb8000;
    
    //char first_char = 'A';
    //for (int i = 0; i < 1000; i++){
    //    video_memory[2*i] = first_char+(i % 26);
    //}
    print("Hello, world!", 0, 0);
    print("Hello, world!", 0, 1);

    return 0;
}

char * getVideoAdress(int column, int row){
    char * toReturn = (char *)(0xb8000 + 2 * (column + row * 80));
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