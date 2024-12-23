/**
 * @file symtab.c
 * @brief implementation of functions from symtab.h .
 * @author Huy Le (hl9082)
 * 
 */

#define _DEFAULT_SOURCE ///for isalpha function, and strdup function.
#include<string.h> /// string operations
#include<stdio.h> /// for error handling
#include<stdlib.h> /// standard library
#include "symtab.h" /// for functions to implement
#include <errno.h> /// for number of error
#include "stack.h" /// for stack-based functions
#include<ctype.h>///for isspace and isalpha function.

static symbol_t * table_top=NULL; //symbol table global variable, pointing to the first entry of the table.





/// Destroys the symbol table
void free_table(void){
    while(table_top!=NULL){//stop when we reaches the end.
        symbol_t * tmp = table_top->next;
        if(table_top->var_name!=NULL){
            free(table_top->var_name);
            table_top->var_name=NULL;//prevent dangling pointer.
        }
        free(table_top);
        table_top=tmp;
    }
}

/// Constructs the table by reading the file.  The format is
/// one symbol per line in the format:
///
///     variable-type variable-name     variable-value
///     ...
///
/// For this problem we guarantee the variable-names are all
/// unique, and the format of the file is error free.
/// You are allowed to create it statically or
/// dynamically and store it locally.
/// @param filename The name of the file containing the symbols
/// @exception If the file can't be opened, an error message should
/// be displayed to standard error and the program should exit
/// with EXIT_FAILURE.  If no file is specified, an empty table
/// should be built (there is no message or error/exit in this case).
///
/// Error loading symbol table
///
void build_table(char *filename){
    
    if(filename==NULL){
        return;
    }
    FILE * fptr=fopen(filename,"r");
    if(fptr==NULL){
        perror("Error loading symbol table\n"); // Use perror() for more informative error messages
        
        exit(EXIT_FAILURE);
    }

    char line[BUFLEN+1];//current line


    char *buffer; //current symbol name

    int tmp; /// current symbol value

    char * val;///string representation of the symbol's value. 

    const char hashtag='#';//if we meet this, we ignore the line

       

    while(fgets(line, BUFLEN, fptr) != NULL){
        
       
        
       
        if(line[0]==hashtag){
            continue;
        }

        buffer=strtok(line," \t\n");
        
        
        if(isalpha(buffer[0])==0){
                fprintf(stderr,"Invalid symbol name: %s\n",buffer);
                continue;
        }
        val=strtok(NULL," \t\n");
        
        if(sscanf(val,"%d",&tmp)==1){
            
            

            //Otherwise, we just create a new entry in the table.
            symbol_t * new_entry=(symbol_t*)malloc(sizeof(symbol_t));

            if(new_entry==NULL){
                perror("Invalid allocation.\n");
                free_table();
                fclose(fptr);
                exit(EXIT_FAILURE);
            }

            new_entry->var_name=(char*)malloc(strlen(buffer)+1);
            if(new_entry->var_name==NULL){
                perror("Invalid token\n");
                free(new_entry);
                new_entry=NULL;
                fclose(fptr);
                exit(EXIT_FAILURE);
            }
            
            strcpy(new_entry->var_name,buffer);
            new_entry->val=tmp;
            new_entry->next=table_top;
            table_top=new_entry;

        }
        
        
        
        
    }
    
    fclose(fptr);
}

/// Displays the contents of the symbol table in the following format:
///
/// SYMBOL TABLE:
///     Name: variable-name, Value: variable-value
///     ...
///
/// Each symbol should be printed one per line, tab-indented.
void dump_table(void){
    
    
    if(table_top!=NULL){
    
        printf("SYMBOL TABLE:\n");
        symbol_t * tmp=table_top;
        while(tmp!=NULL){
            printf("\tName: %s, Value: %d\n",tmp->var_name,tmp->val);
            tmp=tmp->next;
        }
    }
}

/// Returns the symtab_t object in the symbol table associated
///     with the variable name
/// @param variable The name of the variable (a C string)
/// @return The symbol_t object containing the binding,
///     or NULL if not found
symbol_t *lookup_table(char *variable){
    if(table_top==NULL || variable==NULL){
        return NULL;
    }
    symbol_t * tmp=table_top;
  
    while(tmp!=NULL){
        if(strcmp(tmp->var_name,variable)==0){
            return tmp;
        }
        tmp=tmp->next;
    }
    return NULL;
}

/// Adds a binding to the symbol table
/// @param name  The name of the variable (a C string)
/// @param val  The value associated with the variable
/// @return the new symbol_t object added to the table,
///     or NULL if no space is available
/// No check is done to see if the symbol is already in the table
symbol_t *create_symbol(char *name, int val){
    if(name==NULL){
        perror("Nothing to create.\n");
        return NULL;
    }
    symbol_t * new_symb=malloc(sizeof(*new_symb));
    if(!new_symb){
        perror("Symbol tables can't be created!\n");
        return NULL;
    }
    new_symb->var_name  = (char*)malloc(strlen(name)+1);
   
    if(!new_symb->var_name){
        perror("Improper allocation for symbol's name.\n");
        free(new_symb);
        new_symb=NULL;
        return NULL;        
    }
    strcpy(new_symb->var_name,name);
    new_symb->val=val;
    new_symb->next=table_top;
    table_top=new_symb;
    return new_symb;
}

