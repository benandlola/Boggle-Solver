#ifndef __LIST_H__
#define __LIST_H__

//node type stored in lists
typedef struct ll_node{
  struct ll_node * next; 
  char * val; 
} ll_node_t;

//list_t struct to store a list
typedef struct{
  ll_node_t * head;
  int size; 
} llist_t;

// Return a newly initialized, empty linked list
llist_t * ll_init();

//delete/deallocate a linked list
void ll_delete(llist_t * ll);

//insert the string (duplicated by strdup!) onto the front of the list 
void ll_push(llist_t * ll, char * s);

#endif