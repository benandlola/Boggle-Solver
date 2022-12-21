#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "llist.h"
#include "hashmap.h"
#include "boggle.h"

//constant values on all the dice in global
const char * boggle_dice[] = {
        "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
        "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCENST",
        "CEIILT", "CEILPT", "CEIPST", "DDHNOT", "DHHLOR",
        "DHLNOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
        "FIPRSY", "GORRVW", "IPRRRY", "NOOTUW", "OOOTTU",
};

//initialize a boggle instance 
boggle_t * bg_init(int seed, char * dict_fn) {
  //set a random seed
  srandom(seed);

  //create the instance
  boggle_t * bg = calloc(sizeof(boggle_t), 1);

  //setup the board by rolling dice
  int used[BOGGLE_NUM_DICE] = {0};
  for(int i=0;i<BOGGLE_NUM_DICE;i++) {
    while(1){
      //choose a dice to roll
      int d = random()%BOGGLE_NUM_DICE;
      if(used[d]) continue; //keep choosing if already rolled

      //choose a random side
      int s = random()%BOGGLE_DICE_SIDES;
      bg->board[i/BOGGLE_DIMENSION][i%BOGGLE_DIMENSION] = boggle_dice[d][s];
      used[d] = 1; //mark that dice as rolled
      break;
    }
  }

  //load the words into the dictionary (hashmap)
  bg->dict = hm_init(); //initialize hashmap for dictionary
  FILE *dictionary = fopen(dict_fn, "r");
  char word [1024]; 
  while (fscanf(dictionary, "%1023s", word) != EOF) { //creating the dictionary
    for (char *w = word; (*w = toupper(*w)) != 0; w++); 
    hm_add(bg->dict, word);
  }
  fclose(dictionary);
  return bg;
}

//delete/free the boggle instance 
void bg_delete(boggle_t * bg) {
  hm_delete(bg->dict); 
  free(bg);
}

//print the board using
void bg_print(boggle_t * bg){
  printf(".-----------.\n");
  for (int r = 0; r < 5; r++) {
    printf("| ");
    for (int c = 0; c < 5; c++) {
      printf("%c",bg->board[r][c]);      
      if (bg->board[r][c]=='Q')
	printf("u");
      else
	printf(" ");
    }
    printf("|\n");
  }
  printf("'-----------'\n");
}

//count the points up
// 3 and 4 length words are worth 1 point
// 5 length worth 2
// 6 length worth 3
// 7 length worth 5
// 8 length worth 11
// words only go from 3 - 8 letters in length
int bg_count_points(boggle_t * bg, llist_t * words) {
  int pts=0;
  for(ll_node_t * n = words->head; n; n = n->next) {
    if(hm_check(bg->dict,n->val)){
      switch(strlen(n->val)){
      case 0:
      case 1:
      case 2: break;
      case 3:
      case 4:
	pts+=1;
	break;
      case 5:
	pts+=2;
	break;
      case 6:
	pts+=3;
	break;
      case 7:
	pts+=5;
	break;
      default:
	pts+=11;
      }
    }
  }
  return pts; 
}

//helper functions for the recursio
void _bg_all_words(boggle_t * bg, //boggle instance
		   hashmap_t * hm, //hashmap to track found words
		   int r, int c, //current row (r) / column (c)
		   char * last_word, //what the last word was
		   int last_visited[BOGGLE_DIMENSION][BOGGLE_DIMENSION] //what has been previously visited; don't want to double back
		   );

//entry function for finding all the words
llist_t * bg_all_words(boggle_t *bg){

  //currently visited nowhere (all 0's), and the current word is the empty string
  int visited[BOGGLE_DIMENSION][BOGGLE_DIMENSION] = {0};
  char cur_word[9] = {0}; //empty string 

  //initialize a hashmap to track status
  hashmap_t * hm = hm_init();

  //start a recurssion at every space on the board
  for(int i=0;i<BOGGLE_DIMENSION;i++) {
    for(int j=0;j<BOGGLE_DIMENSION;j++) {
      _bg_all_words(bg, hm, i, j, cur_word, visited);
    }
  }

  //return all the words you found by traversing the hashmap
  llist_t *all_words = ll_init();
  for (int i = 0; i < hm->num_buckets; i++) { //traverse hashmap and push to allwords
    ll_node_t *cur = hm->buckets[i]->head;
    while (cur != NULL) {
      if (hm_check(bg->dict, cur->val)) { //check to see if the dictionary contains the value
        ll_push(all_words, cur->val); //add found words to list
      }
      cur = cur->next;
    }
  }
    
  hm_delete(hm); //delete the hashmap

  //return the list of words
  return all_words;
}

void _bg_all_words(boggle_t *bg, hashmap_t * hm, int r, int c,
		  char * last_word, int last_visited[BOGGLE_DIMENSION][BOGGLE_DIMENSION]) {

  char cur_word[9] = {0};
  int visited[BOGGLE_DIMENSION][BOGGLE_DIMENSION] = {0};

  //edge cases
  if (strlen(last_word) > 7) {
    return;
  }

  if (r >= BOGGLE_DIMENSION || c >= BOGGLE_DIMENSION || r < 0 || c < 0) { 
    return;
  }

  if (last_visited[r][c] == 1) { 
    return;
  }

  strcpy(cur_word, last_word);  
  strncat(cur_word, &bg->board[r][c], 1);

  //if letter is Q add u to it (and if valid size)
  if (bg->board[r][c] == 'Q' && strlen(cur_word) < 7) { 
    cur_word[strlen(cur_word)] = 'U';
  }

  //check if value exists in hashmap, if not, add it
  if (strlen(cur_word) > 2) { 
    if(hm_check(hm, cur_word) == false) {
      hm_add(hm, cur_word);
    }
  }

  //copy over last visited
  for (int i = 0; i < BOGGLE_DIMENSION; i++) {
    for (int j = 0; j < BOGGLE_DIMENSION; j++) {
      visited[i][j] = last_visited[i][j];
    }
  }
  //mark as visited
  visited[r][c] = 1; 
  
  //Perform 8 recursive calls on all surrounding dic
  _bg_all_words(bg,hm,r-1,c,cur_word,visited);
  _bg_all_words(bg,hm,r-1,c-1,cur_word,visited);
  _bg_all_words(bg,hm,r-1,c+1,cur_word,visited);
  _bg_all_words(bg,hm,r+1,c,cur_word,visited);
  _bg_all_words(bg,hm,r+1,c-1,cur_word,visited);
  _bg_all_words(bg,hm,r+1,c+1,cur_word,visited);
  _bg_all_words(bg,hm,r,c-1,cur_word,visited);
  _bg_all_words(bg,hm,r,c+1,cur_word,visited);
}