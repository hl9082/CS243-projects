/**
 * @file concordance.c
 * 
 * Descriptions: the file containing your main() function and any non-BST supporting functions you need.
 * 
 * @author Huy Le (hl9082)
 */
///To make functions in bst.h usable
#define _DEFAULT_SOURCE
#include<stdio.h> /// <For printf and other functions related to standard input/output
#include "bst.h" /// < For BST related operations.
#include<stdlib.h> /// < For free() functions.
#include<string.h> /// <For string operations.
#include<ctype.h> /// < For size_t

/**
 * main() --
 * The program will be run with no command-line arguments. If an argument is given, you should display the following message (with a newline at the end) to the standard error output and exit:
    usage:  concordance
 * Else, it reads lines of words and insert them into the binary search tree, then perform
 * operations on them before freeing them.
 * @param argv - number of command arguments.
 * @param args - array of command arguments.
 * @return EXIT_FAILURE whenever the command arguments are invoked or no words are entered. Else, return EXIT_SUCCESS.
 */
int main(int argv , char * args[]){
    if(argv>0 && !args){
        fprintf(stderr,"usage: concordance\n");
        return EXIT_FAILURE;
    }
    char *buff = NULL;  // important: must initially be NULL
    size_t len = 0;    // buffer's length, should initially be 0
    bstnode_t *bp=NULL;
    int line=0;
    while( getline(&buff,&len,stdin) >=0 ) {
        //remove_nonalphabet(buff);
        // do something with each input line
        line++;
        char * word=strtok(buff," \t\n.,;!?\"()=");
        while(word!=NULL){
           
            bst_insert(&bp,word,line);
            
            word=strtok(NULL, " \t\n.,;!?\"()=");
        }
    }
    //error handle
    if(!bp){
        fprintf(stderr,"concordance: no words in the input\n");
        free(buff);
        buff=NULL;///prevent dangling pointers
        return EXIT_FAILURE;
    }
    
    bst_traverse(bp);

    bst_cleanup(bp);

    free(buff);

    buff=NULL; ///prevent dangling pointers

    return EXIT_SUCCESS;
}