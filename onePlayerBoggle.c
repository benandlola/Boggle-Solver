#include <stdio.h>
#include <stdlib.h>

#include "llist.h"
#include "hashmap.h"
#include "boggle.h"

//One player boggle solver using random seed
int main(int argc, char * argv[]){
  FILE * dict_f;
  boggle_t * bg;
  int seed = 0;
  int points = 0;
  bst_node_t * bst;
  llist_t * ll;
  
  if(argc < 2){
    printf("ERROR: require dictionary file\n");
    exit(1);
  }

  if (argc == 3){
    seed = atoi(argv[2]);
  }
  
  //initialize the boggle instance
  bg = bg_init(seed,argv[1]);

  //print the board
  bg_print(bg);

  //find all the words
  bst = bg_all_words(bg);
  ll = bg_all_words2(bg);
  //count up the points.
  points = bg_count_points(bg, ll);

  //print out the words
  bst_print(bst);
  bst_delete(bst); //free the tree
  ll_delete(ll); //free the list
  
  printf("\n");
  printf("Total Points: %d\n",points);

  //delete the boggle instance
  bg_delete(bg);
}
