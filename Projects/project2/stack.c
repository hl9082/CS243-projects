/**
 * @file stack.c
 * @brief implement all functions in stack.h.
 * @author Huy Le (hl9082)
 */
#include "stack.h" ///for functions to implement

#include<stdlib.h> /// standard library

#include<stdio.h> /// for perror and fprintf

#include<string.h> ///for memcpy and strlen

#include "stack_node.h" ///for stack_node

#include "parser.h" ///for alternative operator and ? operator



/// make a new stack
/// @return  a new stack structure with a NULL top pointer
stack_t *make_stack(void){
    stack_t * new_stack=(stack_t*)malloc(sizeof(stack_t));
    if(!new_stack){
        perror("Improper allocation!\n");
        return NULL;
    }
    new_stack->top=NULL;
    return new_stack;
}

/// Add an element to the top of the stack (stack is changed).
/// This routine should dynamically allocate a new node.
/// @param stack Points to the stack 
/// @param data The token (C String)
void push(stack_t *stack, void *data){
    if(stack==NULL || data==NULL){
        perror("Nothing to be pushed.\n");
    }
    stack_node_t * new_node=(stack_node_t*)malloc(sizeof(*new_node));
    if(new_node==NULL){
        perror("Improper allocation.\n");
        return;
    }
    new_node->data=data;
    new_node->next=stack->top;
    stack->top=new_node;
}

/// Return the top element from the stack (stack is unchanged)
/// @param stack points to the stack
/// @return the top element on the stack (a pointer to something)
/// @exception If the stack is empty, the program should 
///     exit with EXIT_FAILURE
void *top(stack_t * stack){
    if(stack==NULL || stack->top==NULL){
        perror("Stack is empty.\n");
        exit(EXIT_FAILURE);
    }
    return stack->top->data;
}

/// Removes the top element from the stack (stack is changed).
/// This routine should free the memory associated with the top node.
/// @param stack points to the stack
/// @exception If the stack is empty, the program should 
///     exit with EXIT_FAILURE
void pop(stack_t *stack){
    if(stack==NULL || stack->top==NULL){
        perror("Stack is empty.\n");
        exit(EXIT_FAILURE);
    }
    stack_node_t * cur=stack->top;
    stack->top=cur->next;
    free(cur);
    cur=NULL;//prevent dangling pointer
}

/// Tells whether the stack is empty or not (stack is unchanged)
/// @param stack Points to the stack
/// @return 0 if not empty, any other value otherwise
int empty_stack(stack_t * stack){
    return(stack==NULL || stack->top==NULL);
}

/// Frees all of the stack nodes, including the stack structure
/// @param stk  Points to the stack to free
void free_stack(stack_t * stack){
    stack_node_t * tmp=stack->top;
        while(tmp){
            stack_node_t*cur=tmp->next;
            free(cur);
            cur=tmp;
            tmp=tmp->next;
        }
        free(stack);    
        stack=NULL;//prevent dangling pointer.
    
}