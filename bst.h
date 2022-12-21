#ifndef __BST_H__
#define __BST_H__

#include <stdlib.h>
#include <stdbool.h>

typedef struct bst_node{
  struct bst_node * right;
  struct bst_node * left; 
  char * val;
} bst_node_t;

//create new node
bst_node_t *bst_create(char * val);

//print inorder a binary search tree
void bst_print(bst_node_t * node);

//delete/deallocate the binary search tree
void bst_delete(bst_node_t * node);

//add a string value to the binary search tree
bst_node_t * bst_insert(bst_node_t *node, char * v);

#endif