#include <stdio.h>
extern int launcher(FILE *p);
extern int findFrame();
extern int isFrameInPageTable(int frameNumber, PCB *p);
extern int findVictim(PCB *p);
extern int updatePageTable(PCB *p, int pageNumber, int frameNumber, int victimFrame);
extern void loadPage(int pageNumber, FILE *f, int frameNumber);