#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pcb.h"
#include "kernel.h"
#include "ram.h"

int currPid = 0;

int countTotalPages(FILE *f) {
    int count = 0;
    int numPages = 0;
    char c;
    for (c = getc(f); c != EOF; c = getc(f)) {
        if (c == '\n'){
            count = count + 1; 
        }
    }
    numPages = count / 4;
    if (count % 4 != 0) {
        numPages += 1;
    }
    fclose(f);
    return numPages;
}

//IF I HAVE TIME, MAKE IT FIFO INSTEAD
int findFrame() {
    for (int i = 0; i < 10; i++) {
        if (isFrameFree(i) == 1) {
            return i;
        }
    }
    return -1;
}

int isFrameInPageTable(int frameNumber, PCB *p) {
    for (int i = 0; i < 10; i++) {
        if (p->pageTable[i] == frameNumber) {
            return -1;
        }
    }
    return 1;
}

int findVictim(PCB *p) {
    int victimFrameNumber = (rand() % (9 + 1 - 0) + 0);
    if (isFrameInPageTable(victimFrameNumber, p) == -1) {
        while (isFrameInPageTable(victimFrameNumber, p) != 1) {
            victimFrameNumber = (victimFrameNumber + 1) % 9;
        }
    }
    return victimFrameNumber;
}

int updatePageTable(PCB *p, int pageNumber, int frameNumber, int victimFrame) {
    if (victimFrame != -1) {
        PCB* p_victim = findVictimPCB(victimFrame);
        for (int i = 0; i < 10; i++) {
            if (p_victim->pageTable[i] == victimFrame) {
                p_victim->pageTable[i] = -1;
            }
        }
    }
    p->pageTable[pageNumber] = frameNumber;
    return 0;
}

void loadPage(int pageNumber, FILE *f, int frameNumber) {
    int lineNumberStart = 4 * pageNumber;
    int lineNumberEnd = lineNumberStart + 4;
    char line[256];
    int i = 0;
    int j = 0;
    //printf("start: %d\n", lineNumberStart);
    //printf("end: %d\n", lineNumberEnd);
    while (fgets(line, sizeof(line), f)) {
        if(i >= lineNumberStart && i < lineNumberEnd) {
            //printf("Curr i is: %d\n", i);
            //printf("start ram cell num: %d\n", (frameNumber + i));
            if (j < 4) {
                setRamCell(4 * frameNumber + j, strdup(line)); 
                j++;
            }
        } else if (i == lineNumberEnd) {
            //printf("done\n");
            break;
        }
        i++;
    }
    while (j != 4) {
        setRamCell(4 * frameNumber + j, "-1\n");
        j++;
    }
    fclose(f);
}

int launcher(FILE *p) {
    char new_file[50];
    snprintf(new_file, sizeof(new_file), "./BackingStore/%d.txt", currPid);
    FILE *new_p = fopen(new_file, "w");
    if (new_p == NULL) { 
        fclose(p);
        printf("Cannot open the new file from backing store."); 
        exit(0); 
    }
    char c;
    while ((c = fgetc(p)) != EOF) {
        printf("%c",c);
        fputc(c, new_p);
    }
    fclose(p);
    fclose(new_p);
    FILE *bs_file = fopen(new_file, "r");
    int pages_max = countTotalPages(bs_file);
    PCB* pcb = myinit(pages_max, currPid);
    int pageNumber = 0;
    int frameNumber = findFrame();
    if (frameNumber == -1) {
        frameNumber = findVictim(pcb);
        updatePageTable(pcb, pageNumber, frameNumber, 1);
    } else {
        updatePageTable(pcb, pageNumber, frameNumber, -1);
    }
    FILE *to_load = fopen(new_file, "r");
    loadPage(pageNumber, to_load, frameNumber);
    if (pages_max > 1) {
        pageNumber = 1;
        frameNumber = findFrame();
        updatePageTable(pcb, pageNumber, frameNumber, -1);
        FILE *to_load = fopen(new_file, "r");
        loadPage(pageNumber, to_load, frameNumber);
    }
    currPid++;
    return 1; //Don't forget to return a 0 whenever this fct can fuck up so that I can handle it in interpreter.c
}
