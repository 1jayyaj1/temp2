#include <stdio.h>
#include <stdlib.h>
/*
PCB has 3 fields
PC : Points to the the current line reached for that program
start: Points to first line of that program
end: Points to last line of that program
*/
typedef struct PCB
{
    int PC;
    int PC_page;
    int PC_offset;
    int pages_max;
    int PID;
    int pageTable[10];
}PCB;

/*
Passes 2 parameters (start , end)
This method creates a PCB with fields set as this:
PC = start
start = start
end = end
*/
PCB* makePCB(int pages_max, int PID){
    PCB* pcb = (PCB*)malloc(sizeof(PCB));
    pcb->PC = pcb->PC_page + pcb->PC_offset;
    pcb->PC_page = 0;
    pcb->PC_offset = 0;
    pcb->PID = PID;
    pcb->pages_max = pages_max;
    for (int i = 0; i < 10; i++) {
        pcb->pageTable[i] = -1;
    }
    return pcb;
}


