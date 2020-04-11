ASSIGNMENT 3: JAY ABI-SAAD (260801368)
THIS ASSIGNMENT WAS BUILT USING: mimi.cs.mcgill.ca

TO COMPILE AND RUN THE CODE:
1) gcc -c shell.c interpreter.c shellmemory.c cpu.c kernel.c pcb.c ram.c memorymanager.c
2) gcc -o mykernel shell.o interpreter.o shellmemory.o cpu.o kernel.o pcb.o ram.o memorymanager.o
3) ./mykernel

 IMPORTANT NOTES:
 - The executable doesn't seem to have access to the .txt files. Again, please just compile and run the program in the regular way (gcc).