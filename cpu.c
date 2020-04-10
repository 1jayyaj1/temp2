#include<string.h>
#include"ram.h"
#include"interpreter.h"
#include"shell.h"

#define DEFAULT_QUANTA 2

struct CPU { 
    int IP;
    int offset; 
    char IR[40]; 
    int quanta; 
} CPU;

/*
This method passes a quanta.
It will then executes quanta number of lines in RAM as from the address
pointed by the CPU IP.
Returns an errorCode.
*/
int run(int quanta){
    int errorCode = 0;
    // If a quanta greater than the default quanta of 2 is passed,
    // run will instead execute only default quanta of lines.
    if (quanta > DEFAULT_QUANTA ){
        quanta = DEFAULT_QUANTA;
    }
    for (int i = 0; i < quanta; i++) {
        strcpy(CPU.IR,ram[(4*CPU.IP)+CPU.offset]);
        if (CPU.IR[0] != '-' && CPU.IR[0] != '1') {
            errorCode = parse(CPU.IR);
        }
        // Do error checking and if error, return error
        if (errorCode != 0) {
            // Display error message if fatal
            if (errorCode < 0) {
                ram[(4*CPU.IP)+CPU.offset][strlen(ram[(4*CPU.IP)+CPU.offset])-2]='\0';
                displayCode(errorCode,ram[(4*CPU.IP)+CPU.offset]);
            }
            return errorCode;
        }
        CPU.offset++;
    }
    return errorCode;
}
