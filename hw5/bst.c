/**
 * @file bst.c
 * the file containing your implementation of the BST functions described below, 
 * and any other functions needed to support these functions.
 * @author Huy Le (hl9082)
 */

///because we are using C99, the strdup() function declaration is not normally supplied when you include the <string.h> header file. 
///To make it visible, you must define the symbol _DEFAULT_SOURCE before you include any header files in any source file that uses this function.
#define _DEFAULT_SOURCE
#include "bst.h" ///<	The header file describing the binary search tree module, including the bstnode_t type and several function prototypes.
#include<stdlib.h> /// < We include this library for size_t and unsigned integers. 
#include<string.h> /// < we include this library for string operations.
#include<stdio.h> /// <We include this library to get printf function.

//initial capacity for the reference list of binary tree
#define INITIAL_CAPACITY 6 

//initial frequency
#define INITIAL_FREQ 1

//factor to multiply whenever capacity is reached

#define FACTOR 2

///
/// bst_insert() --
///     Allocate a node to hold a word and insert it into the tree.
///
/// @param bp   - a pointer to the pointer to the root node of the
///               tree to which this word will be added
/// @param word - string containing the word to be inserted
/// @param line - the line number where the word occurred in the input
///
void bst_insert( bstnode_t **bp, const char *word, int line ){
  if(word==NULL || line==0){//if the word pointer is null or the line is 0.
      exit(EXIT_FAILURE);
  }
  if(!(*bp)){//if it's a null pointer, then we have to create a tree from scratch.
   *bp=(bstnode_t*)malloc(sizeof(bstnode_t));
   if(!(*bp)){
      exit(EXIT_FAILURE);
   }
   (*bp)->word=strdup(word);
   (*bp)->freq=INITIAL_FREQ;
   (*bp)->reflen=INITIAL_CAPACITY;
   (*bp)->refs=(unsigned int *)malloc((*bp)->reflen*sizeof(unsigned int));
   if(!((*bp)->refs)){
      exit(EXIT_FAILURE);
   }
   
   (*bp)->refs[0]=line;

   (*bp)->left=NULL;

   (*bp)->right=NULL;

  }
  else{
  int cmp=strcasecmp(word,(*bp)->word);

   if(cmp==0){
   (*bp)->freq++;
   if ((*bp)->reflen < (*bp)->freq) {
      (*bp)->reflen *= FACTOR;
      (*bp)->refs = (unsigned int*)realloc((*bp)->refs, (*bp)->reflen * sizeof(unsigned int));
      if(!((*bp)->refs)){
         exit(EXIT_FAILURE);
      }
   }
   int nth_position=(*bp)->freq-1;
   (*bp)->refs[nth_position] = line;
   }

  else if(cmp<0){
      bst_insert(&((*bp)->left),word,line);
  }
  else{
      bst_insert(&((*bp)->right),word,line);
  }
 }
}

///
/// bst_traverse() --
///    Recursively traverses the tree and prints the value of each
///    node.
///
/// args -
/// @param root - a pointer to the root node of the tree to traverse
///
void bst_traverse( const bstnode_t *root ){
   if(root==NULL){
      return;//we can't traverse because the tree is empty already.
   }
   bst_traverse(root->left);
   printf("%s (%u):  ",root->word,root->freq);
   for(unsigned int i=0;i<root->freq;i++){
      
      if(i>=1){
         printf(", %u", root->refs[i]);
      }
      else{
      printf("%u",root->refs[i]);
      }
   }
   printf("\n");
   bst_traverse(root->right);
}

///
/// bst_cleanup() -- 
///    Clean up all memory management associated with the nodes on the heap
///
/// args
/// @param root - the current root of the tree
///
void bst_cleanup( bstnode_t *root ){
   if(root==NULL){
      return;
   }
   bst_cleanup(root->left);
   bst_cleanup(root->right);
   free(root->word);
   free(root->refs);
   free(root);
   root=NULL; //preventing dangling pointer pointing to released or invalid memory
}
