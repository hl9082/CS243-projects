///
/// @file    bst.h
/// @author  wrc@cs.rit.edu
///
/// Concordance generator search tree definitions
//

#ifndef TREE_H_
#define TREE_H_

// The definition of a node in the tree

typedef struct bstnode_s {
    char *word;                // the word held in this node
    unsigned int freq;         // how many times it has been seen
    unsigned int reflen;       // capacity of the reference list
    unsigned int *refs;        // list of references
    struct bstnode_s *left;   // node's left child
    struct bstnode_s *right;  // node's right child
} bstnode_t;

///
/// bst_insert() --
///     Allocate a node to hold a word and insert it into the tree.
///
/// @param bp   - a pointer to the pointer to the root node of the
///               tree to which this word will be added
/// @param word - string containing the word to be inserted
/// @param line - the line number where the word occurred in the input
///
void bst_insert( bstnode_t **bp, const char *word, int line );

///
/// bst_traverse() --
///    Recursively traverses the tree and prints the value of each
///    node.
///
/// args -
/// @param root - a pointer to the root node of the tree to traverse
///
void bst_traverse( const bstnode_t *root );

///
/// bst_cleanup() -- 
///    Clean up all memory management associated with the nodes on the heap
///
/// args
/// @param root - the current root of the tree
///
void bst_cleanup( bstnode_t *root );

#endif // BST_H
