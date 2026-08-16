#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define NULL_TERM 0
/*alphabet is 1-26*/
#define OPER_IS 27
#define OPER_IS_NOT 28
#define OPER_AND 29
#define OPER_OR 30
#define OPER_NAND 31
#define OPER_NOR 32
#define OPER_XOR 33
#define OPER_XNOR 34
#define RETURN_RES 35
#define END_SM 36
#define TOKEN_TRUE 37
#define TOKEN_FALSE


#define LEXER_CONF_MAX_SIZE 1024
#define LEXER_MODE_START 0
#define LEXER_MODE_ASSIGN 1
#define LEXER_MODE_FUNC 2
#define LEXER_MODE_PARAM_A 3
#define LEXER_MODE_PARAM_B 4
#define LEXER_MODE_FINISH 5


#define LEXER_ASSIGN_TYPE_FUNC 0
#define LEXER_ASSIGN_TYPE_BOOL_CHAR 1
#define LEXER_ASSIGN_TYPE_BOOL_UINT 2
#define LEXER_ASSIGN_RETURN_BUFFER 3
#define DEBUG 1


unsigned int char_is_in_low_alphabet(unsigned char in);
unsigned int type_of_assignment();
void display_lexer_out(uint8_t * tl);

unsigned int char_is_in_low_alphabet(unsigned char in){
    return 'a' <= in && in <= 'z' ? 1 : 0;
}


unsigned int type_of_assignment(){
    return 0;
}

void display_lexer_out(uint8_t * tl){
   uint8_t * tl_cp = tl;
   uint8_t padding = 0;
   for(; *tl_cp !=0; tl_cp++){
       switch (*tl_cp){
          case 27:
              printf("ASSIGN_IS");
              break;
          case 28:
              printf("ASSIGN_IS_NOT");
              break;
          case 29:
             printf("FUNC_AND");
             break;
          case 30:
              printf("FUNC_OR");
              break;
          case 31:
              printf("FUNC_NAND");
              break;
          case 32:
              printf("FUNC_NOR");
              break;
          case 33:
              printf("FUNC_XOR");
              break;
          case 34:
              printf("FUNC_XNOR");
              break;
          case 35:
              printf("RETURN_RES");
              break;
          case 36:
              printf("END_SM");
           default:
             if (*tl_cp+96 >= 'a' && *tl_cp+96 <= 'z'){
                 printf("NAMESPACE_LETTER_%c", *tl_cp+64);
             }
       }
       printf(" -> ");
       padding++;
       if (padding % 3 == 0) printf("\n");
   }
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
    FILE * fptr_write;
    char * out_file;
    uint8_t tiny_lexer[LEXER_CONF_MAX_SIZE] = {0};
    uint16_t tiny_lexer_i = 0;
    uint16_t total_token_count = 0;
    int res = 0;
    unsigned char res_as_char = 0;
    uint8_t lexer_mode = 0;
    char err_message [256] = {0};
    char func_buffer [10] = {0};
    int i = 0;
    uint16_t line_number = 1;

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
        total_token_count++)
    {
        /*get the result, and if it's the EOF let's stop.
         if it's whitespace or a newline, let's also ignore it*/
        res = fgetc(fptr_read);
        if (res == EOF) break;
        res_as_char = (unsigned char) res;
        if (res_as_char == ' ' || res_as_char == 10) continue;

        switch (lexer_mode){
            case LEXER_MODE_START:
                if (DEBUG == 1) printf("LEXER_MODE_START -> found potential token %d %c \n", res_as_char, res_as_char);
                /*if the letter is within the low alphabet, it's a valid boolean*/

                if (char_is_in_low_alphabet(res_as_char)){
                    tiny_lexer[tiny_lexer_i] = (res_as_char - 96);
                    if (DEBUG == 1) printf("Lexer will assign value to namespace %c\n",res_as_char );
                    tiny_lexer_i++;
                    lexer_mode = LEXER_MODE_ASSIGN;
                }

                /* if something when wrong, then let's go to the error resolution */
                else {
                    strcpy("Invalid character namespace for assignment. Must be one lowercase a-z with spacing atfer", err_message);
                    goto err;
                }

                if (' ' != ( (unsigned char ) fgetc(fptr_read)) ){
                    strcpy("You must put at least one space character between the statement and the next token", err_message);
                    goto err;
                }
                break;

            case LEXER_MODE_ASSIGN:
                if (DEBUG == 1) printf("LEXER_MODE_ASSIGN -> found potential token %d %c \n", res_as_char, res_as_char);

                if(res_as_char == 'i'){
                    /*we must have an 'is' statement
                     let's discard the characters 's' and
                     then we have to see whether the next character is n.
                     if it is then we will insert the "isnt" token,
                     otherwise we will insert the "is" */
                    printf("\nDEBUG: char is the start");
                    fgetc(fptr_read);
                    res_as_char = (unsigned char ) fgetc(fptr_read);
                    printf("res as char is %c", res_as_char);
                    if ('n' == res_as_char){
                        tiny_lexer[tiny_lexer_i] = OPER_IS_NOT;
                        tiny_lexer_i++;
                        /* here we can discard the t and check to make
                         sure there's a space before we proceed
                         to the next lexer phase*/
                        fgetc(fptr_read);
                        if (fgetc(fptr_read) != ' '){
                            strcpy("You must put at least one space character bwetween the statement and the next token", err_message);
                            goto err;
                        }
                    }

                    /* else, if this is a space, then we have the correct 'is ' statement, and we
                     MUST continue after*/
                    else if (' ' == res_as_char){
                        tiny_lexer[tiny_lexer_i] = OPER_IS;
                        tiny_lexer_i++;
                    } else{
                        strcpy("Unrecognized declaration: use is or isnt when assigning", err_message);
                        goto err;
                    }
                }
                printf("Done!\n");
                if (tiny_lexer[tiny_lexer_i-1] == OPER_IS) printf("Assignment operator == 'is'");
                if (tiny_lexer[tiny_lexer_i-1] == OPER_IS_NOT) printf("Assignment operator == 'is not'");


                lexer_mode = LEXER_MODE_FUNC;

                break;
            case LEXER_MODE_FUNC:
                if (DEBUG == 1) printf("LEXER_MODE_FUNC -> found potential token %d %c \n", res_as_char, res_as_char);
                /*this one is a little different. there's a lot of functions, so we're gonna
                 use a char buffer to see what value it could be
                 the func could be:
                    and(a,b) -> does an AND on a,b
                    or(a,b) -> does an OR on a, b
                    nand (a,b) -> does an NAND on a, b
                    nor (a,b) -> does an NOR on a,b
                    xor (a,b) -> does an XOR on a, b
                    xnor (a,b) -> does an XNOR on a,b
                    resultpart -> assigns the next bit n to the resultpart that returns (from a 16-bit result)
                    (a-z) -> assigns a-z as the result of the function
                    true, false -> assigns the value to the literal;
                 */
                func_buffer[0] = res_as_char;
                for (i = 1; i < 9; i++){
                    res_as_char = (unsigned char) fgetc(fptr_read);
                    if (res_as_char == '(' || res_as_char == ';'){
                       func_buffer[i] = '\0';
                       break;
                    }
                    func_buffer[i] = res_as_char;
                }

                printf("\n%s\n", func_buffer);
                i = tiny_lexer_i;
                /*no switch cause of strcmp :( */
                if (strcmp("resultpar", func_buffer) == 0){
                    tiny_lexer[tiny_lexer_i] = RETURN_RES;
                    tiny_lexer_i++;
                }
                if (strcmp("and", func_buffer) == 0){
                    tiny_lexer[tiny_lexer_i] = OPER_AND;
                    tiny_lexer_i++;
                }
                if (strcmp("or", func_buffer) == 0){
                    tiny_lexer[tiny_lexer_i] = OPER_OR;
                    tiny_lexer_i++;
                }
                if (strcmp("nor", func_buffer) == 0){
                    tiny_lexer[tiny_lexer_i] = OPER_NOR;
                    tiny_lexer_i++;
                }
                if (strcmp("nand", func_buffer) == 0){
                    tiny_lexer[tiny_lexer_i] = OPER_NAND;
                    tiny_lexer_i++;
                }
                if (strcmp("xor", func_buffer) == 0){
                    tiny_lexer[tiny_lexer_i] = OPER_XOR;
                    tiny_lexer_i++;
                }
                if (strcmp("xnor", func_buffer) == 0){
                    tiny_lexer[tiny_lexer_i] = OPER_XNOR;
                    tiny_lexer_i++;
                }
                if (strcmp("true", func_buffer) == 0){
                    tiny_lexer[tiny_lexer_i] = OPER_XNOR;
                    tiny_lexer_i++;
                    /*dirty solution time! so essentially, we stop
                     * reading the buffer at the character ';', but
                     * we consume it so next read it's not a valid end
                     * character. to vanquish this problem, we will
                     * now manually check this even tho we're in a
                     * different phase, and then proceed to
                     * the next. dirty, and squeamish, but functions
                     */
                     if (res_as_char != ';'){printf("ERR: you must end all statements with a ';' \n");goto err;}
                     tiny_lexer[tiny_lexer_i] = END_SM; tiny_lexer_i++;
                     lexer_mode = LEXER_MODE_START;
                     continue;

                }
                if (strcmp("false", func_buffer) == 0){
                    tiny_lexer[tiny_lexer_i] = OPER_XNOR;
                    tiny_lexer_i++;
                    if (res_as_char != ';'){printf("ERR: you must end all statements with a ';' \n");goto err;}
                    tiny_lexer[tiny_lexer_i] = END_SM; tiny_lexer_i++;
                    lexer_mode = LEXER_MODE_START;
                    continue;
                }
                /* if none of these occured, then let's double check it's not a one char
                 * letter namespace
                 */
                if (i == tiny_lexer_i){
                    if (func_buffer[0] != 0 && func_buffer[1] == 0 && char_is_in_low_alphabet((unsigned char) func_buffer[0])){
                       tiny_lexer[tiny_lexer_i]  = func_buffer[0]-96;
                       tiny_lexer_i++;
                       /*skip the function declaration, continue */
                       if (res_as_char != ';'){printf("ERR: you must end all statements with a ';' \n");goto err;}
                       tiny_lexer[tiny_lexer_i] = END_SM; tiny_lexer_i++;
                       lexer_mode = LEXER_MODE_START;
                       continue;
                    }
                }
                /*we set i=tiny_lexer_i
                 * since the tiny_lexer_i is set, that means that
                 * it should be incremented. if they're the same, the namespace wasn't found
                 */
                if (i==tiny_lexer_i){
                    printf("ERR: symbol %s cannot be found. please put a valid functor name\n", func_buffer);
                    goto err;
                }
                lexer_mode = LEXER_MODE_PARAM_A;
                if (DEBUG==1) printf("\nDEBUG: assigned value %s with trueID (verify!!!) %d\n", func_buffer, tiny_lexer[tiny_lexer_i]);
                break;
            case LEXER_MODE_PARAM_A:
                if (DEBUG == 1) printf("LEXER_MODE_PARAM_A -> found potential token %d %c \n", res_as_char, res_as_char);

                /*here we're veryifying a namespace to compare to. essentially, discard the
                 * parantheses and get the next char, and verify it's alphabetical. if it's not,
                 * then we return an error. if it is, then we can go to the next phase. i wish
                 * C supported multiple switch values because the B value will be the exact same
                 */
                if (char_is_in_low_alphabet(res_as_char)){
                    /*we found a namespace, let's delete the next char,
                     * bump in that, and let's go in
                     */
                    tiny_lexer[tiny_lexer_i] = res_as_char-96;
                    tiny_lexer_i++;
                    fgetc(fptr_read);
                    /*here we discard ',' */
                }
                else{
                    strcpy("ERR: unrecognized non-alphabetical namespace for function", err_message);
                    goto err;
                }
                lexer_mode = LEXER_MODE_PARAM_B;
                break;
        case LEXER_MODE_PARAM_B:
                if (DEBUG == 1) printf("LEXER_MODE_PARAM_B -> found potential token %d %c \n", res_as_char, res_as_char);

                /*here we're veryifying a namespace to compare to. essentially, look at the char and discard the parentheses
                 */

                if (char_is_in_low_alphabet(res_as_char)){
                    /*we found a namespace, let's delete the next char,
                     * bump in that, and let's go in
                     */
                    tiny_lexer[tiny_lexer_i] = res_as_char-96;
                    tiny_lexer_i++;
                    fgetc(fptr_read);
                    /* here we discard ')' */
                }
                else{
                    strcpy("ERR: unrecognized non-alphabetical namespace for function", err_message);
                    goto err;
                }
                lexer_mode = LEXER_MODE_FINISH;
                break;
        case LEXER_MODE_FINISH:
            if (res_as_char == ';'){
                /*we have successfully finished a statement */
                tiny_lexer[tiny_lexer_i] = END_SM;
                tiny_lexer_i++;
                line_number++;
                lexer_mode = LEXER_MODE_START;
            }
            else{
                printf("ERR: you must end all statements with a ';' \n");
                goto err;
            }
        }
    }

    fclose(fptr_read);
    if (DEBUG) display_lexer_out(tiny_lexer);
    /* okay, now that we have our linker, let's generate our assembly
        * first, let's ask possix nicely if we can write a file
        */
    fptr_write = fopen("out.asm", "w");
    if (fptr_write == NULL){
        printf("ERR: cannot write output! do you have access here?");
    }
    if (access("out.asm", W_OK)){
        printf("ERR: cannot write output! OS doesn't permit access");
        return 1;
    }
    /*now that we have a valid fptr, let's write to it
     * first, the heading in x86 matters a lot
     */
    fwrite(".global start\n", sizeof(unsigned char) * 14, 1, fptr_write);
    fwrite("start:\n", sizeof(unsigned char) * 7, 1, fptr_write);
    /*the first part is to init this memory. memory  */


    return 0;



    err:
        printf("ERR: could not compile program\n");
        printf("trace --> LINE NUMBER: %d TOKEN %d\n\n", line_number, total_token_count);
        printf("%s\n", err_message);
        printf("LEXER VALUES:\n ");
        display_lexer_out(tiny_lexer);
        return -1;

}
