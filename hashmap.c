#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <math.h>
#include <string.h>

#include "llist.h"
#include "hashmap.h"

//a localfunction for hashing an input value
unsigned int _hash(hashmap_t * hm, char * val) {
  //based on Java's String.hashcode()
  double k = 0;
  double len = (double) strlen(val);
  int i=1;
  for(char * c = val; *c ; c++,i++) {
    k += (*c) * pow(31,len-i);
  }

  return (unsigned int) (((unsigned long) k) % hm->num_buckets);
}


//local function: resize the hashmap by doubling the number of buckets and rehashing
void _resize(hashmap_t * hm) {
  
  llist_t **temp = hm->buckets; 
  int old_buckets = hm->num_buckets;
  hm->num_buckets = hm->num_buckets*2; 
  hm->buckets = calloc(hm->num_buckets, sizeof(llist_t*));
  hm->size = 0;
  for (int i = 0; i < hm->num_buckets; i++) { 
    hm->buckets[i] = ll_init(); 
  }
  for (int i = 0; i < old_buckets; i++) { 
    ll_node_t *cur = temp[i]->head; 
    while (cur != NULL) {
      hm_add(hm, cur->val); 
      cur = cur->next; 
    }
    ll_delete(temp[i]); 
  }
  free(temp); 
  
}

//initliaze a hashmap with initial number of buckets
hashmap_t * hm_init() {

  hashmap_t * hm = calloc(1, sizeof(hashmap_t));
  hm->num_buckets = HM_INIT_NUM_BUCKETS; 
  hm->buckets = calloc(HM_INIT_NUM_BUCKETS, sizeof(llist_t*)); 
  hm->size = 0; 
  for (int i = 0; i < hm->num_buckets; i++) {
    hm->buckets[i] = ll_init(); 
  }

  return hm;
}

//delete/deallocate the hashmap
void hm_delete(hashmap_t * hm){
  //free llists and nodes
  for (int i = 0; i < hm->num_buckets; i++) {
    ll_delete(hm->buckets[i]); 
  }
  //delete the hashmap
  free(hm->buckets);
  free(hm);
}

//add a string value to the hashmap
void hm_add(hashmap_t * hm, char * val) {
  
  //check the load on the hashtable, if > max load, resize
  if(((hm->size+1)/(double) hm->num_buckets) > HM_MAX_LOAD) {
    _resize(hm); 
  }

  unsigned int h = _hash(hm,val); 
  ll_push(hm->buckets[h],val); /
  hm->size++;

}

//see if a string value is in the hashmap
bool hm_check(hashmap_t * hm, char * val) {
 
  unsigned int value = _hash(hm, val);
  ll_node_t *cur = hm->buckets[value]->head; 
  while (cur != NULL) {
    if (strcmp(cur->val, val) == 0) { 
      return true;
    }
    cur = cur->next; 
  }
  
  return false;
}