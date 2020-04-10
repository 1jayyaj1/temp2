#define DEFAULT_QUANTA 2

struct CPU { 
    int IP;
    int offset; 
    char IR[40]; 
    int quanta; 
} CPU;

int run(int);
