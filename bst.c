#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <math.h>
#include <string.h>

#include "bst.h"

//create node
bst_node_t *bst_create(char * val) {
    bst_node_t *new = malloc(sizeof(bst_node_t));
    new->val = strdup(val);
    new->left = NULL;
    new->right = NULL;

    return new;
}

//print a binary search tree
void bst_print(bst_node_t * node) {
    if (node == NULL) {
      return;
    }
    //recur to smallest
    bst_print(node->left);
 
    //print value
    printf("%s\n", node->val);
 
    //recur down right side
    bst_print(node->right);
}
 
//delete/deallocate the binary search tree
void bst_delete(bst_node_t * node) {
    if (node == NULL) {
        return;
    }
    bst_delete(node->left);
    bst_delete(node->right);
    free(node->val);
    free(node);
}

//add a string value to the binary search tree
bst_node_t * bst_insert(bst_node_t * node, char * v) {
  if (node == NULL) {
    node = bst_create(v);
  }
  else if (node->val == NULL) { //first case
    node->val = strdup(v);
  }
  else if (strcmp(node->val, v) > 0) { //go left if under
    node->left = bst_insert(node->left, v);
  }
  else {  //go right if greater
    node->right = bst_insert(node->right, v);
  }
  return node;
}