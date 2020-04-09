#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include"shell.h"
#include"pcb.h"
#include"ram.h"
#include"cpu.h"
#include"interpreter.h"
#include"memorymanager.h"

/*
This is a node in the Ready Queue implemented as 
a linked list.
A node holds a PCB pointer and a pointer to the next node.
PCB: PCB
next: next node
*/
typedef struct ReadyQueueNode {
    PCB*  PCB;
    struct ReadyQueueNode* next;
} ReadyQueueNode;

ReadyQueueNode* head = NULL;
ReadyQueueNode* tail = NULL;
int sizeOfQueue = 0;

void boot() {
    resetRAM();
    char command[50];
    strcpy(command, "rm -rf BackingStore && mkdir BackingStore");
    system(command);
}

int main(int argc, char const *argv[])
{
    int error = 0;
    boot();
    error = kernel();
    return error;
}
/*
Adds a pcb to the tail of the linked list
*/
void addToReady(struct PCB* pcb) {
    ReadyQueueNode* newNode = (ReadyQueueNode *)malloc(sizeof(ReadyQueueNode));
    newNode->PCB = pcb;
    newNode->next = NULL;
    if (head == NULL){
        head = newNode;
        tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
    sizeOfQueue++;
}

/*
Returns the size of the queue
*/
int size(){
    return sizeOfQueue;
}

/*
Pops the pcb at the head of the linked list.
pop will cause an error if linkedlist is empty.
Always check size of queue using size()
*/
struct PCB* pop(){
    PCB* topNode = head->PCB;
    ReadyQueueNode * temp = head;
    if (head == tail){
        head = NULL;
        tail = NULL;
    } else {
        head = head->next;
    }
    free(temp);
    sizeOfQueue--;
    return topNode;
}

PCB* findVictimPCB(int victimFrame) {
    ReadyQueueNode* curr = head;
    PCB* pcb;
    while (curr) {
        for (int i = 0; i < 10; i++) {
            if (curr->PCB->pageTable[i] == victimFrame) {
                pcb = curr->PCB;
            }
        }
        curr = curr->next;
    }
    return pcb;
}
/*
Passes a filename
Opens the file, copies the content in the RAM.
Creates a PCB for that program.
Adds the PCB on the ready queue.
Return an errorCode:
ERRORCODE 0 : NO ERROR
ERRORCODE -3 : SCRIPT NOT FOUND
ERRORCODE -5 : NOT ENOUGH RAM (EXEC)
*/
PCB* myinit(int pages_max, int PID){
    PCB* pcb = makePCB(pages_max, PID);
    addToReady(pcb);
    return pcb;
}

int interrupt(PCB* pcb) {
    //TO IMPLEMENT
    int next_page = pcb->PC_page + 1;
    if (next_page >= pcb->pages_max) {
        for (int i = 0; i < 10; i++) {
            if (pcb->pageTable[i] != -1) {
                removeFromRam(pcb->pageTable[i]);
            }
        }
        free(pcb);
        return 0;
    } else {
        if (pcb->pageTable[next_page] == -1) {
            int next_frame = findFrame();
            if (next_frame == -1) {
                next_frame = findVictim(pcb);
                updatePageTable(pcb, next_page, next_frame, 1);
            } else {
                updatePageTable(pcb, next_page, next_frame, -1);
            }
            char curr_file[50];
            snprintf(curr_file, sizeof(curr_file), "./BackingStore/%d.txt", pcb->PID);
            FILE *to_load = fopen(curr_file, "r"); 
            loadPage(next_page, to_load, next_frame);
            fclose(to_load);
            //printRam();
        }
        pcb->PC_page = next_page;
        pcb->PC_offset = 0;
        pcb->PC = pcb->PC_page + pcb->PC_offset;
        addToReady(pcb);
    }
    return 0;
}

int scheduler(){
    // set CPU quanta to default, IP to -1, IR = NULL
    CPU.quanta = DEFAULT_QUANTA;
    CPU.IP = -1;
    while (size() != 0){
        //pop head of queue
        PCB* pcb = pop();
        //copy PC of PCB to IP of CPU
        CPU.IP = pcb->pageTable[pcb->PC_page];
        printf("Current IP is: %d\n", CPU.IP);
        CPU.offset = pcb->PC_offset;
        printf("Current Offset is: %d\n", CPU.offset);
        int isOver = FALSE;
        int remaining = 0;
        int quanta = DEFAULT_QUANTA;
        int errorCode = run(quanta);
        if ( errorCode!=0 ){
            for (int i = 0; i < 10; i++) {
                if (pcb->pageTable[i] != -1) {
                    removeFromRam(pcb->pageTable[i]);
                }
            }
            free(pcb);
        } 
        if (CPU.offset == 4) {
            //PC_page = 0, PC_offset = 4, PC = PC_page = 0 (not updated yet)
            interrupt(pcb);
            //PC_page = 1, PC_offset = 0, PC = 1 + 0 = 0
        } else {
            pcb->PC_offset = CPU.offset;
            pcb->PC = pcb->PC_page + pcb->PC_offset;
            addToReady(pcb);
        }
    }
    // reset RAM
    resetRAM();
    return 0;
}

/*
Flushes every pcb off the ready queue in the case of a load error
*/
void emptyReadyQueue(){
    while (head!=NULL){
        ReadyQueueNode * temp = head;
        head = head->next;
        free(temp->PCB);
        free(temp);
    }
    sizeOfQueue =0;
}

