#include <stdio.h>
#include <stdlib.h>
#include "pcb.h"
#include "kernel.h"

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
    printf("%d", pages_max);
    PCB* pcb = myinit(pages_max, currPid);
    currPid++;
    return 0;
}

void loadPage(int pageNumber, FILE *f, int frameNumber) {
    //TO IMPLEMENT
}

int findFrame() {
    //TO IMPLEMENT
    return 0;
}

int findVictim(PCB *p) {
    //TO IMPLEMENT
    return 0;
}

int updatePageTable(PCB *p, int pageNumber, int frameNumber, int victimFrame) {
    //TO IMPLEMENT
    return 0;
}