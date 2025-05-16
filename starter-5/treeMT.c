#include "treeMT.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

// Helper
//
static void printInOrderHelper(TNode* n) {
	if (n == NULL) return;
	printInOrderHelper(n->left);
	printf("%d ", n->val);
	printInOrderHelper(n->right);
}

static void destroyHelper(TNode* n) {
	if (n == NULL) return;
	destroyHelper(n->left);
	destroyHelper(n->right);
	pthread_mutex_destroy(&n->lock);
	free(n);
}


Tree* makeEmptyTree() {
    /* TODO: Create a new tree, setting the root to NULL. Initialize any necessary mutexes. */
	Tree* t = malloc(sizeof *t);
	t->root = NULL;
	pthread_mutex_init(&t->lock, NULL);
	return t;


}

void insertIntoTree(Tree* t, int val) {
    /* TODO: 
     * This function should construct a new Node, traverse the BST to find the correct insertion location, and insert it. 
     * If there is already a node with that value, nothing needs to be inserted.
     * Use mutexes to prevent threads from interfering with one another. 
     * Read the description in the pdf for more instructions.  
    */
	TNode* newNode = malloc(sizeof *newNode);
	newNode->val = val;
	newNode->left = newNode->right = NULL;
	pthread_mutex_init(&newNode->lock, NULL);

	pthread_mutex_lock(&t->lock);
	if (t->root == NULL) {
		t->root = newNode;
		pthread_mutex_unlock(&t->lock);
		return;
	}
	TNode* curr = t->root;
	pthread_mutex_lock(&curr->lock);
	pthread_mutex_unlock(&t->lock);

	while (1) {
		if (val == curr->val) {
			pthread_mutex_unlock(&curr->lock);
			pthread_mutex_destroy(&newNode->lock);
			free(newNode);
			return;
		}

		TNode** childPtr = (val < curr->val) ? &curr->left : &curr->right;
		if (*childPtr == NULL) {
			*childPtr = newNode;
			pthread_mutex_unlock(&curr->lock);
			return;
		}

		TNode* next = *childPtr;
		pthread_mutex_lock(&next->lock);
		pthread_mutex_unlock(&curr->lock);
		curr = next;
	}

}


void printInOrder(Tree* t) {
    /* TODO: Print values in sorted order. The function need not be thread-safe. */
	printInOrderHelper(t->root);
}

void destroyTree(Tree* t) {
    /* TODO: Release all resources. */
	destroyHelper(t->root);
	pthread_mutex_destroy(&t->lock);
	free(t);

}


