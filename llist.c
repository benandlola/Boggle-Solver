#include <stdlib.h>
#include <string.h>

#include "llist.h"

//initialize a linked list
llist_t * ll_init() {
  llist_t *ll = malloc(sizeof(llist_t)); 
  ll->size = 0;
  ll->head = NULL;
  return ll; 
}

//delete a linked list
void ll_delete(llist_t * ll) {
  ll_node_t *cur, *next;
  cur = ll->head;
  while (cur != NULL) { 
    next = cur->next; 
    free(cur->val); 
    free(cur);  
    cur = next; 
  }
  free(ll); //free list itself
}

//push an item onto the front of the list
void ll_push(llist_t * ll, char * s) {
  ll_node_t *new = malloc(sizeof(ll_node_t)); 
  new->val = strdup(s); 
  new->next = ll->head;  
  ll->head = new; 
}