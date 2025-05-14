#include "treeMT.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

Tree* makeEmptyTree() {
    /* TODO: Create a new tree, setting the root to NULL. Initialize any necessary mutexes. */
}

void insertIntoTree(Tree* t, int val) {
    /* TODO: 
     * This function should construct a new Node, traverse the BST to find the correct insertion location, and insert it. 
     * If there is already a node with that value, nothing needs to be inserted.
     * Use mutexes to prevent threads from interfering with one another. 
     * Read the description in the pdf for more instructions.  
    */
}

void printInOrder(Tree* t) {
    /* TODO: Print values in sorted order. The function need not be thread-safe. */
}

void destroyTree(Tree* t) {
    /* TODO: Release all resources. */
}


