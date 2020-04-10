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
    int errorCode = 0;
    if (victimFrame != -1) {
        PCB* p_victim = findVictimPCB(victimFrame);
        for (int i = 0; i < 10; i++) {
            if (p_victim->pageTable[i] == victimFrame) {
                p_victim->pageTable[i] = -1;
            }
        }
    }
    p->pageTable[pageNumber] = frameNumber;
    return errorCode;
}

void loadPage(int pageNumber, FILE *f, int frameNumber) {
    int lineNumberStart = 4 * pageNumber;
    int lineNumberEnd = lineNumberStart + 4;
    char line[256];
    int i = 0;
    int j = 0;
    while (fgets(line, sizeof(line), f)) {
        if(i >= lineNumberStart && i < lineNumberEnd) {
            if (j < 4) {
                setRamCell(4 * frameNumber + j, strdup(line)); 
                j++;
            }
        } else if (i == lineNumberEnd) {
            break;
        }
        i++;
    }
    while (j != 4) {
        setRamCell(4 * frameNumber + j, "-1\n"); // "-1" is used to pad the rest of the frame if there are less than 4 LOC left to add from disk to ram
        j++;
    }
    fclose(f);
}

int launcher(FILE *p) {
    int errorCode = 0;
    char new_file[50];
    snprintf(new_file, sizeof(new_file), "./BackingStore/%d.txt", currPid);
    FILE *new_p = fopen(new_file, "w");
    if (new_p == NULL) { 
        fclose(p);
        errorCode = -3; 
        return errorCode;
    }
    char c;
    while ((c = fgetc(p)) != EOF) {
        fputc(c, new_p);
    }
    fclose(p);
    fclose(new_p);
    FILE *bs_file = fopen(new_file, "r");
    if (bs_file == NULL) { 
        errorCode = -3; 
        return errorCode;
    }
    int pages_max = countTotalPages(bs_file);
    PCB* pcb = myinit(pages_max, currPid);
    int pageNumber = 0;
    int frameNumber = findFrame();
    if (frameNumber == -1) {
        frameNumber = findVictim(pcb);
        errorCode = updatePageTable(pcb, pageNumber, frameNumber, 1);
    } else {
        errorCode = updatePageTable(pcb, pageNumber, frameNumber, -1);
    }
    FILE *to_load = fopen(new_file, "r");
    if (to_load == NULL) { 
        errorCode = -3; 
        return errorCode;
    }
    loadPage(pageNumber, to_load, frameNumber);
    if (pages_max > 1) {
        pageNumber = 1;
        frameNumber = findFrame();
        errorCode = updatePageTable(pcb, pageNumber, frameNumber, -1);
        FILE *to_load = fopen(new_file, "r");
        if (to_load == NULL) { 
            errorCode = -3; 
            return errorCode;
        }
        loadPage(pageNumber, to_load, frameNumber);
    }
    currPid++;
    return errorCode;
}
