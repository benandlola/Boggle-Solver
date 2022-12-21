# Boggle-Solver
Creation of linked list and hashmap structures as a way to learn C

These data structures are then used to find all the words on a boggle board, that is randomly generated
A boggle instance is defined as a 5x5 grid of dice, where each dice displays a different character.

This program finds words that are 3-8 letters long using a depth first search mechanic. 'Q' letters become a 'Qu'.
Points are tallied up based up based on the length of the word and all, which is then printed along with all the words found from a dictionary text file.

A spellchecker program is included as well to test the hashmap dictionary functionality

Create the necessary files with `make` and run the program with `./onePlayerBoggle dictionary.txt int` where int is the seed
