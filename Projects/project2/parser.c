/**
 * @file parser.c
 * @brief where we write functions to parse the tree.
 * @author Huy Le (hl9082)
 */

#define _DEFAULT_SOURCE ///for isalpha function and isalnum functions.

#include "parser.h" ///for functions to implement

#include <stdlib.h>///standard library

#include<ctype.h> ///for type handling

#include <stdio.h> /// for error handling

#include<string.h>///for string operations.

#include<stdbool.h> ///for boolean operations.

static char * ALT_OP_STR=":"; //for ternary operator

static enum parse_error_e cur_parse_error=PARSE_NONE; ///current parse error (PARSE_NONE if no problems)

static enum eval_error_e cur_eval_error=EVAL_NONE; ///current evaluation error (EVAL_NONE if no problems)

/**
 * Returns a corresponding mathematical/boolean operator based on the given token.
 * @param token the string token.
 * @return a math/boolean operator.
 */
static op_type_t to_operator(char *token) {
    if (strcmp(token, ADD_OP_STR) == 0){
        return ADD_OP;
    }
    else if (strcmp(token, SUB_OP_STR) == 0){
        return SUB_OP;
    }
    else if (strcmp(token, MUL_OP_STR) == 0){
        return MUL_OP;
    }
    else if (strcmp(token, DIV_OP_STR) == 0){
        return DIV_OP;
    }
    else if (strcmp(token, MOD_OP_STR) == 0){
        return MOD_OP;
    }
    else if (strcmp(token,Q_OP_STR)==0){
        return Q_OP;
    }
    else if(strcmp(token,ASSIGN_OP_STR)==0){
        return ASSIGN_OP;
    }
   
    return NO_OP; // Default case
}

/**
 * Check if a token is an integer.
 * @param token the string token.
 * @return true if it's a positive integer and false otherwise.
 */
static bool integer_check(char * token){
    if(token==NULL){
        return false;
    }
    char * end;///pointer to the end of the token
    const char minus='-';
    if(token[0]==minus){
        //an integer must be positive.
        return false;
    }
    strtol(token,&end,10);
    return(*end=='\0');//check if a string can be converted to an integer.
}

/**
 * Check if a token is a symbol.
 * @param token the string token.
 * @return true if the token is a symbol, and false if otherwise.
 */
static bool symbol_check(char * token){
    if(token==NULL || !isalpha(token[0])){
        return false;
    }
    
    for(size_t i=0;i<strlen(token);i++){
        if(!isalnum(token[i])){
            return false; ///it must be non-numeric.
        }
    }
    return true;
    
    
}

/**
 * Check if a token is an operator.
 * @param token the string token.
 * @return true if the token is an operator other than ternary operator, and false if otherwise.
 */
static bool operator_check(char*token){
    return(!strcmp(token,ADD_OP_STR)||!strcmp(token,SUB_OP_STR)||!strcmp(token,DIV_OP_STR)
    ||!strcmp(token,MUL_OP_STR)||!strcmp(token,MOD_OP_STR)||!strcmp(token,Q_OP_STR)||
    !strcmp(token,ASSIGN_OP_STR));
}

/// The main read-eval-print function that reads the expression,
/// parses it, and evaluates the result, printing the infix expression
/// and the resulting value to standard output.
/// process, using the rest of the routines defined here.
/// @param exp The expression as a string
void rep(char *exp){

    if(!exp){
        return;
    }
  
    if(strlen(exp)>0 && exp[strlen(exp)-1]=='\n'){
    // Remove the newline character if it exists at the end of the line
        exp[strlen(exp)-1] = '\0';
    }

    tree_node_t *tree = make_parse_tree(exp);

    if(cur_parse_error!=PARSE_NONE){
        cleanup_tree(tree);
        switch(cur_parse_error){
            case TOO_FEW_TOKENS:
                perror("Invalid expression, not enough tokens\n");
                break;
            case TOO_MANY_TOKENS:
                perror("Invalid expression, too many tokens\n");
                break;
            case INVALID_ASSIGNMENT:
                perror("Invalid expression, invalid assignment\n");
                break;
            case ILLEGAL_TOKEN:
                perror("Invalid expression, illegal token\n");
                break;
            case PARSE_NONE:
                break;
        }
        cur_parse_error=PARSE_NONE;
        cur_eval_error=EVAL_NONE;
        return;
    }

    int result = eval_tree(tree);

    if(cur_eval_error!=EVAL_NONE){
        cleanup_tree(tree);
        switch(cur_eval_error){
            case DIVISION_BY_ZERO:
                fprintf(stderr,"Division by zero\n");
               
                break;
            case INVALID_MODULUS:
                fprintf(stderr,"Division by zero\n");
                break;
            case UNDEFINED_SYMBOL:
                perror("Undefined symbol\n");

                cur_eval_error=EVAL_NONE;
                break;
            case UNKNOWN_EXP_TYPE:
                perror("Unknown expression type\n");
                cur_eval_error=EVAL_NONE;
                break;
            case MISSING_LVALUE:
                perror("Missing l-value\n");
                break;
            case INVALID_LVALUE:
                perror("Invalid l-value\n");
                cur_eval_error=EVAL_NONE;
                break;
            case SYMTAB_FULL:
                perror("Symbol table is full\n");
                cur_eval_error=EVAL_NONE;
                break;
            case UNKNOWN_OPERATION:
                perror("Invalid expression, unknown operation\n");
                cur_eval_error=EVAL_NONE;
                break;
            case EVAL_NONE:
                break;
        }
        cur_parse_error=PARSE_NONE;
        cur_eval_error=EVAL_NONE;
        return;
    }
    
    print_infix(tree);
   
    printf(" = %d\n", result);
    cleanup_tree(tree);
     
    cur_parse_error=PARSE_NONE;

    cur_eval_error=EVAL_NONE;
    

}

/**
 * Build interior tree for ternary operation.
 * @param op operation.
 * @param token token string.
 * @param stack the stack.
 * @return the interior tree for ternary operation.
 */
static tree_node_t* ternary_tree(op_type_t op, char * token, stack_t * stack){
   tree_node_t * false_res=parse(stack);
   tree_node_t * true_res=parse(stack);
   tree_node_t * condition=parse(stack);
   //base case : if one of the node is null, we return null.
   if(!false_res||!true_res||!condition||strcmp(token,Q_OP_STR)!=0){
        return NULL;
   }
   tree_node_t* ternary_right=make_interior(ALT_OP,ALT_OP_STR,true_res,false_res);
   if(ternary_right==NULL){
        cleanup_tree(false_res);
        
        cleanup_tree(true_res);

        cleanup_tree(condition);

        return NULL;
   }
   tree_node_t * res=make_interior(op,token,condition,ternary_right);
   if(res==NULL){

        cleanup_tree(false_res);
        
        cleanup_tree(true_res);

        cleanup_tree(condition);

        cleanup_tree(ternary_right);

        return NULL;
   }
   return res;
}

/// Recursively build the parse tree from items on the stack
/// @param stack  the list of tokens to parse
/// @return the root of the parse tree, or NULL on failure
/// @exception will occur if the parse fails
tree_node_t *parse(stack_t *stack){

    if(cur_parse_error!=PARSE_NONE){
        perror("Parsing failed.\n");
        return NULL;
    }

   
    if(empty_stack(stack)){
        cur_parse_error=TOO_FEW_TOKENS;
        
        return NULL;
    }

    char * token = (char*)top(stack);

    pop(stack);
    if(token==NULL){
        cur_parse_error=ILLEGAL_TOKEN;
        
        return NULL;
    }

    if(operator_check(token)==true){

        op_type_t op=to_operator(token);

        if(op==NO_OP){
            
            free(token);
            token=NULL;//avoid dangling pointer
            return NULL;
        }

        else if(op==Q_OP){
            return ternary_tree(op,token,stack);
        }
        
        
        tree_node_t * right=parse(stack);

        tree_node_t * left=parse(stack);
    
        return make_interior(op,token,left,right);

    }

    else{

        exp_type_t expression_type;

        if(integer_check(token)){
            expression_type=INTEGER;
        }

        else if(symbol_check(token)){
            expression_type=SYMBOL;
        }

        else{
            cur_parse_error=ILLEGAL_TOKEN;
            
            free(token);
            token=NULL;
            return NULL;
        }
        
        return make_leaf(expression_type,token);

    }

    

    

}

/// Constructs the expression tree from the expression.  It
/// must use the stack to order the tokens.  It must also
/// deallocate the memory associated with the stack in all cases.
/// If a symbol is encountered, it should be stored in the node
/// without checking if it is in the symbol table - evaluation will
/// resolve that issue.
/// @param expr the postfix expression as a C string
/// @return the root of the expression tree
/// @exception There are 2 error conditions that you must deal
///     with.  In each case, the memory associated with the
///     tree must be cleaned up before returning.  Neither 
///     stops execution of the program. 
///
///     1. If there are too few tokens, set the parser error
///     to TOO_FEW_TOKENS and display the message to standard error:
///
///     Invalid expression, not enough tokens
///
///     2. If there are many tokens (stack is not empty after building),
///     set the parser error to TOO_MANY_TOKENS and display the message
///     to standard error:
///
///     Invalid expression, too many tokens
tree_node_t *make_parse_tree(char *expr){
    cur_parse_error=PARSE_NONE;
    if(expr==NULL){
        cur_parse_error=ILLEGAL_TOKEN;
        
        return NULL;
    }

    cur_parse_error=PARSE_NONE; //reset to default value

    stack_t * stack=make_stack();
    char * token=strtok(expr," ");
   
    if(token==NULL){
        cur_parse_error=TOO_FEW_TOKENS;
        free_stack(stack);
        return NULL;
    }

    while(token!=NULL){
        push(stack,(void*)token);
        token=strtok(NULL, " ");
    }
    tree_node_t * result=parse(stack);

    if(!result){
        cur_parse_error=ILLEGAL_TOKEN;
        free_stack(stack);
        
        return NULL;
    }
    
    if(!empty_stack(stack)){
        ///we have more than allowed tokens.
        cur_parse_error=TOO_MANY_TOKENS;
        
        free_stack(stack);
        cleanup_tree(result);
        return NULL;
    }

    

    if(cur_parse_error!=PARSE_NONE){
        cleanup_tree(result);
        
        free_stack(stack);
        cur_parse_error=PARSE_NONE;
        return NULL;
    }

    free_stack(stack);
    cur_parse_error=PARSE_NONE;

    
    return result;
}

/**
 * This function evaluates the ternary operation.
 * @param left_tree the left subtree.
 * @param right_tree the right subtree - the alternating operation.
 * @return the evaluation of the left node of the right subtree if the condition is true. Else,
 * return the evaluation of the right node of the right subtree.
 */
static int ternary_tree_eval(tree_node_t * left_tree, tree_node_t * right_tree){
int condition=eval_tree(left_tree);
interior_node_t * alt=(interior_node_t*)right_tree->node;
return(condition!=0)?eval_tree(alt->left):eval_tree(alt->right);
}

/// Evaluates the tree and returns the result.
/// @param node The node in the tree: either an INTERIOR or LEAF node
/// @precondition:  This routine should not be called if there
///     is a parser error.
/// @return the evaluated int.  Note:  A symbol evaluates
///     to the value bound to it.
int eval_tree(tree_node_t * node){
    if(cur_eval_error!=EVAL_NONE || node->node==NULL || node->token==NULL){
        return 0;
    }
   
    if(node->type==LEAF){
        
        leaf_node_t * leaf=(leaf_node_t*)node->node;
        char * token=(char*)node->token;
        if(leaf->exp_type==INTEGER){
            
            return strtol(token,NULL,10);
        }
        else if(leaf->exp_type==SYMBOL){
            symbol_t * symbol=lookup_table(token);
            if(symbol==NULL){
                cur_eval_error=UNDEFINED_SYMBOL;
                
                
                return 0;
            }
            
            
            return symbol->val;
        }
        else{
            cur_eval_error=UNKNOWN_EXP_TYPE;
            
            
            
            return 0;
        }
      
    }
    interior_node_t * interior=(interior_node_t*)node->node;
    tree_node_t * left=interior->left;
    tree_node_t * right=interior->right;
    int left_val,right_val;
    
    
    switch(interior->op){
        case ADD_OP:
            left_val=eval_tree(left);
            right_val=eval_tree(right);
            return left_val+right_val;
            break;
        case MUL_OP:
            left_val=eval_tree(left);
            right_val=eval_tree(right);
            return left_val*right_val;
            break;
        case SUB_OP:
            left_val=eval_tree(left);
            right_val=eval_tree(right);
            return left_val-right_val;
            break;
        case DIV_OP:
            left_val=eval_tree(left);
            right_val=eval_tree(right);
            if(right_val==0){

                cur_eval_error=DIVISION_BY_ZERO;

                

                return cur_eval_error;
                
            }
            return left_val/right_val;
            break;
        case MOD_OP:
            left_val=eval_tree(left);
            right_val=eval_tree(right);
            if(right_val==0){

                cur_eval_error=INVALID_MODULUS;

                
                

                return 0;
            }
            return left_val % right_val;
            break;
        case ASSIGN_OP:
            if(!left){
                cur_eval_error=MISSING_LVALUE;
                return 0;
            }
            if(left->type!=LEAF){

                cur_eval_error=INVALID_LVALUE;

                

                return 0;
                
            }
            

            
            leaf_node_t * left_node=(leaf_node_t*)left->node;

            if(left_node->exp_type==SYMBOL){
                

                symbol_t * entry=lookup_table(left->token);

                if(entry==NULL){
                    int right_val=eval_tree(right);
                    entry=create_symbol(left->token,right_val);

                    if(entry==NULL){
                        //if it's still null, we throw an error.
                        cur_eval_error=SYMTAB_FULL;
                        
                        return 0;
                    }
                  return right_val;
                }
                else{
                    int right_val=eval_tree(right);
                    entry->val=right_val;
                    
                    return entry->val;
                }

            }
            
            break;
        case Q_OP:
            return ternary_tree_eval(left,right);
            break;
        default:
            cur_eval_error=UNKNOWN_OPERATION;//unknown operation.
            
            return 0;
            break;    
    }
    return 0;
}

/// Displays the infix expression for the tree, using
/// parentheses to indicate the precedence, e.g.:
///
/// postfix expression: 10 20 + 30 *
/// infix string: ((10+20)*30) 
///
/// @param node  the tree_node of the tree to print
/// @precondition:  This routine should not be called if there
///     is a parser error.
void print_infix(tree_node_t * node){
    if(!node){
        return;
    }
    if(node->type!=LEAF){
        interior_node_t*interior=(interior_node_t*)node->node;
        printf("(");
        print_infix(interior->left);
        
        printf("%s",node->token);
        print_infix(interior->right);
        printf(")");
    }
    else{
        if(node->token){
           
           printf("%s",node->token);
        }
    }
}

/// Cleans up all dynamic memory associated with the expression tree.
/// @param node The current node in the tree
void cleanup_tree(tree_node_t * node){
    if(!node){
        return;
    }
    if(node->type==LEAF){
        if(node->node){
            free(node->node);
            node->node=NULL;
        }
        if(node->token){
            free(node->token);
            node->token=NULL;
        }
        free(node);
        node=NULL;//dangling pointer prevented.
    }
    else{
        interior_node_t * interior_node=(interior_node_t*)node->node;
        
        if(interior_node->left){
            cleanup_tree(interior_node->left);
        }
        if(interior_node->right){
            cleanup_tree(interior_node->right);
        }
        
            
        
        if(node->token){
            free(node->token);
            node->token=NULL;
        }
        
        if(node->node){
            free(node->node);
            node->node=NULL;
        }
        free(node);
        node=NULL;
    }
}