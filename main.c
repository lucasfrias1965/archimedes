#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#define NULL_TERM 0
#define ALPHA(N) ( (n+1) )
#define OPER_IS 27
#define OPER_IS_NOT 28
#define OPER_AND 29
#define OPER_OR 30
#define OPER_NAND 31
#define OPER_NOR 32
#define OPER_XOR 33
#define OPER_XNOR 34
#define ARGV(N) ( (N+34) )
#define RETURN_RES

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
    uint8_t tiny_lexer[1024] = {0};
    int res = 0;
    
    if (argc < 2){
        printf("ERR: must specify file");
        return 1;
    }
    if (access(argv[1], R_OK)){
        printf("ERR: cannot read file");
        return 1;
    }

    fptr_read = fopen(argv[1], "r");
    out_file = argv[2];
    
    /*we can always assume that for every statement, we 
    follow this style guide:
            - char 0 is either a namespace (a,b,c....y,z)
            or a return result
            - some statement is or is not
            - some assignment to transfer over, or some value
            - the end of a statement, terminated by a ;
    */

    while (true){
        res = fgetc(fptr_read);
        if (res == EOF) break;
    }
    
    
    return 0;
}
