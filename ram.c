#include<stdio.h>
#include<string.h>

/*
Stores the index of the next available block of cell
*/
int nextFree = 0;

/*
Ram structure implemented as an array.
Size is 40 strings
*/
char *ram[40] = { NULL }; 

/*
This function will delete the content of a frame in ram
*/
void removeFromRam (int frameNumber) {
    int startFrame = 4 * frameNumber;
    int endFrame = startFrame + 4;
    for (int i = startFrame; i < endFrame; i++) {
        ram[i] = NULL;
    }
}

void setRamCell(int ramCell, char* fileLine) {
    ram[ramCell] = fileLine;
}

int isFrameFree (int frameNumber){
    int startFrame = 4 * frameNumber;
    if (ram[startFrame] == NULL) {
            return 1;
    }
    return -1;
}

/*
Reset the pointer to the free cell back to index 0
*/
void resetRAM(){
    nextFree = 0;
}

/*
Debugging function used to print coontents of ram at any given time
*/
void printRam() {
    for (int i = 0; i < 40; i++) {
        if (ram[i] == NULL) {
            printf("Ram line %d contains: %s\n", i, ram[i]);
        } else {
            printf("Ram line %d contains: %s", i, ram[i]);
        }
    }
}
