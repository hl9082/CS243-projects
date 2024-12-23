/**
 * @file interp.c
 * @brief where we implement everything.
 * @author Huy Le (hl9082)
 */

#include "interp.h" ///<for the interpretation functions.

#include "symtab.h" /// < for symbol table

#include <stdlib.h> /// < standard library

#include "parser.h" /// for the parsing stuff

#include<stdio.h> /// for printing and error handling

#include<stdbool.h> /// for boolean operations

#include<string.h> ///for string operations.



/**
 * Perform parsing operations for the symbol tables.
 * @param argv number of arguments.
 * @param args list of arguments.
 * @return EXIT_SUCCESS if no erros. Else, return EXIT_FAILURE.
 */
int main(int argv, char * args[]){

    if(argv>=3){
        perror("Usage: interp [sym-table]\n");
        return EXIT_FAILURE;
    }

    
   if(argv==2){
        build_table(args[1]);
   }

    dump_table();

    printf("Enter postfix expressions (CTRL-D to exit):\n> ");

    const char hashtag='#';

    char buffer[BUFLEN];

    memset(buffer,0,BUFLEN);

    while (fgets(buffer, BUFLEN, stdin) != NULL) {
    

        if(buffer[0]=='\n' || buffer[0]==hashtag){
            
            continue;
        }
        for(int i=1;i<MAX_LINE;i++){
            if(buffer[i]==hashtag){
                buffer[i]='\0';
                break;
            }
        }
    

        rep(buffer);

        printf("> ");  // Display prompt for the next input
    }

printf("\n");
dump_table();  // Print symbol table for debugging
free_table();  // Free memory associated with symbol table
return EXIT_SUCCESS;
}