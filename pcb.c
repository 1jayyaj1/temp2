#include <stdio.h>
#include <stdlib.h>

/*
PCB has 6 fields
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
This method creates a PCB
*/
PCB* makePCB(int pages_max, int PID){
    PCB* pcb = (PCB*)malloc(sizeof(PCB));
    pcb->PC = pcb->PC_page + pcb->PC_offset;
    pcb->PC_page = 0;
    pcb->PC_offset = 0;
    pcb->PID = PID;
    pcb->pages_max = pages_max;
    for (int i = 0; i < 10; i++) {
        pcb->pageTable[i] = -1; // Initializes all entries of page table to -1 which means empty
    }
    return pcb;
}
