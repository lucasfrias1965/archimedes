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

#define LEXER_CONF_MAX_SIZE 1024
#define LEXER_MODE_START 0
#define LEXER_MODE_ASSIGN 1
#define LEXER_MODE_FUNC 2 
#define LEXER_MODE_PARAM_A 3
#define LEXER_MODE_PARAM_B 4
#define LEXER_MODE_FINISH 5

#define DEBUG 1

bool char_is_in_low_alphabet(char in){
    return 'a' <= in && in >= 'z';
}

int main(int argc, char * argv[]){
	
    /*Code by Lucas Frias — make cool stuff!!
    Copyright 2026 MIT License 2.0
    Archimedes: "Give me a point to stand on and I will
    rotate the Earth"
    A simple compilier for boolean logic to x86 ASM
    Made as a bite size project into making a compilier
    Given simple declarations, will make a precompilied program
    that ouputs the result of logic
    */

    FILE * fptr_read;
    char * out_file;
    uint8_t tiny_lexer[LEXER_CONF_MAX_SIZE] = {0};
    uint16_t tiny_lexer_i = 0;
    uint16_t total_token_count = 0;
    int res = 0;
    unsigned char res_as_char = 0;
    uint8_t lexer_mode = 0;
    
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

    for (total_token_count = 0; 
        total_token_count-1 < LEXER_CONF_MAX_SIZE;
        total_token_count++){
        res = fgetc(fptr_read);
        if (res == EOF) break;
        res_as_char = (unsigned char) res;
        if (res_as_char == ' ' || res_as_char == 10) continue;
        
        switch (lexer_mode){
            case LEXER_MODE_START:
                if (DEBUG == 1) printf("LEXER_MODE_START -> found potential token \n");
                if (char_is_in_low_alphabet(res_as_char)){
                    tiny_lexer[tiny_lexer_i] = res_as_char;
                    printf("Lexer will assign value to namespace %c",res_as_char );
                    tiny_lexer_i++;
                    lexer_mode = LEXER_MODE_ASSIGN;
                }
                else {
                    goto err;
                }
                break;
            case LEXER_MODE_ASSIGN:
                if (DEBUG == 1) printf("LEXER_MODE_ASSIGN");
                break;
        }
    }
    
    
    return 0;

    err:
        printf("ERR: could not compile program");
        printf("trace --> token %d", total_token_count);
        printf(" result");
        
}
