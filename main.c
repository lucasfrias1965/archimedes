#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char * argv[]){
	
    /*Code by Lucas Frias — make cool stuff!!
    Copyright 2026 MIT License 2.0
    Archimedes: "Give me a point to stand on and I will
    rotate the Earth"
    A simple compilier for boolean logic to x86
    Given simple declarations, will make a precompilied program
    that ouputs the result of logic
    */

    FILE * fptr_read;
    char * out_file;
    
    
    printf("%d is res\n", argc);
    if (argc < 2){
        printf("ERR: must specify file");
        return 1;
    }
    if (access(argv[1], R_OK)){
        printf("ERR: cannot read file");
        return 1;
    }

    fptr_read = fopen(argv[0], "r");
    out_file = argv[2];
    
    
    
    
    return 0;
}
