#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[]){
	
    /*Code by Lucas Frias — make cool stuff!!
    Copyright 2026 MIT License 2.0
    Archimedes: “Give me a point to stand on and I will
    rotate the Earth”
    A simple compilier for C like byte logic
    Given simple declarations, will make a precompilied program
    that ouputs the result of logic
    */

    FILE * fptr;
    char * out_file;

    if (argc == 0){
        printf(“ERR: must specify file”);
        return 1;
    }
    if (access(argv[0]) != F_OK){
        printf(“ERR: accessed forbidden”);
        return 1;
    }

    FILE * fptr = fopen(argv[0]);
    char * out_file = argv[1];
    
    
    
    
    
    return 0;
}
