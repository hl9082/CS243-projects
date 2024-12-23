/**
 * @file tree_node.c
 * @brief to implement function of tree_node.h
 * @author Huy Le (hl9082)
 */

#define _DEFAULT_SOURCE ///for strdup function.

#include "tree_node.h" ///functions of the tree_node file

#include<stdlib.h> ///<library for error handling.

#include "parser.h" /// for cleanup_tree() function

#include<string.h>///<string operations
#include<stdio.h> ///for perror.

// Construct an interior node dynamically on the heap.
// @param op  the operation (add, subtract, etc.)
// @param token  the token that derives this node
// @param left  pointer to the left child of this node
// @param right pointer to the right child of this node
// @return the new TreeNode, or NULL if error
tree_node_t *make_interior(op_type_t op, char *token,
                       tree_node_t *left, tree_node_t *right){

if(token==NULL||!left||!right){
    perror("Nothing to make a tree.\n");
    return NULL;
}

tree_node_t * tree_node=(tree_node_t*)malloc(sizeof(*tree_node));
if(!tree_node ){
    perror("Improper allocation.\n");
    return NULL;
}

tree_node->type=INTERIOR;

tree_node->token=(char*)malloc(strlen(token)+1);



if(tree_node->token==NULL){
    perror("Improper allocation.\n");
    free(tree_node);
    tree_node=NULL;
    return NULL;
}

strcpy(tree_node->token,token);


interior_node_t * interior_node=(interior_node_t*)malloc(sizeof(interior_node_t));
if(!interior_node){
    perror("Improper allocation.\n");
    if(tree_node->token){
        free(tree_node->token);
        
        tree_node->token=NULL;
    }
    if(tree_node){
        free(tree_node);
        tree_node=NULL;
    }
    return NULL;
}

interior_node->left=left;

interior_node->right=right;

interior_node->op=op;

tree_node->node=(void*)interior_node;

return tree_node;

}

// Construct a leaf node dynamically on the heap.
// @param expType  the operation token type (INTEGER or SYMBOL)
// @param token  the token that derives this node
// @return the new TreeNode, or NULL if error
tree_node_t *make_leaf(exp_type_t exp_type, char *token){

    if(token==NULL){
        perror("No token to be parsed.\n");
        return NULL;
    }

    tree_node_t * tree_node=(tree_node_t*)malloc(sizeof(tree_node_t));

    if(tree_node==NULL){
        perror("Improper allocation.\n");
        return NULL;
    }
    tree_node->type=LEAF;
    
       
    tree_node->token=(char*)malloc(strlen(token)+1);

    
    

    if(tree_node->token==NULL){
        perror("Invalid token!\n");
        free(tree_node);
        tree_node=NULL;
        return NULL;
    }

    strcpy(tree_node->token,token); 
     

    leaf_node_t * leaf=(leaf_node_t*)malloc(sizeof(leaf_node_t));
    
    if(leaf==NULL){
        perror("Improper allocation.\n");
        if(tree_node->token){
            free(tree_node->token);
            tree_node->token=NULL;
        }
        if(tree_node){
            free(tree_node);
            tree_node=NULL;
        }
        return NULL;
    }
    
    leaf->exp_type=exp_type;
    

    tree_node->node=(void*)leaf;
    
    return tree_node;
}