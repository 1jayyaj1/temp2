#include<stdio.h>

/*
Ram structure implemented as an array.
Size is 40 strings
*/
char* ram[40];

/*
This function will delete the content in a ram frame
*/
void removeFromRam (int frameNumber);

/*
Reset the pointer to the free cell back to index 0
*/
void resetRAM();

extern void setRamCell(int ramCell, char* fileLine);
extern int isFrameFree (int frameNumber);
extern void printRam();
