#ifndef PRINT_H
#define PRINT_H

int get_row(int memory_address);
char * getVideoAdress(int column, int row);
void printC(char character, int column, int row);
void print(char * string, int column, int row);
void prints(char * string);
void scrollDown();

#endif