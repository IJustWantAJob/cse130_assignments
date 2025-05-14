#include "treeMT.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

void shuffle(int* numbers, int n) {
  for(int i = n - 1; i > 0; i--){
    int j = rand() % (i + 1);
    int x = numbers[i];
    numbers[i] = numbers[j];
    numbers[j] = x;
  }
}

ThreadArgs* divideWork(int* numbers, int n, int nt, Tree* t) {
    /* Each thread inserts n / nt numbers in the tree. Any remainders are spread equally */
    ThreadArgs* args = calloc(nt, sizeof(ThreadArgs));
    int rem = n % nt;
    int index = 0;
    for(int i = 0; i < nt; i++){
        args[i].t = t;
        args[i].numbers = numbers + index;
        args[i].n = n/nt;
        if(i < rem)
        args[i].n++;
        index += args[i].n;
    }
    return args;
}

void* startThread(void* arg) {
    /* Each thread inserts values in a BST */
    ThreadArgs* ta = (ThreadArgs*) arg;
    for(int i = 0; i < ta->n; i++){
        insertIntoTree(ta->t, ta->numbers[i]);
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    if(argc != 3){
        printf("Usage: ./treeMTTest n numThreads\n");
        return 1;
    }

    /* Create an empty tree*/
    Tree* tree = makeEmptyTree();

    /* Store the value of n (num of values to be inserted) and nt (num of threads to be used)*/
    int n = atoi(argv[1]);
    int nt = atoi(argv[2]);

    /* Store numbers from 0 to n-1 in an array and shuffle them to prevent any ordering*/
    int numbers[n];
    for(int i = 0; i < n; i++){
        numbers[i] = i;
    }
    shuffle(numbers, n);

    /* Numbers are to be inserted in the tree in the below order */
    printf("Order of the numbers inserted into the tree: \n");
    for(int i = 0; i < n; i++){
      printf("%d ",numbers[i]);
    }
    printf("\n");

    /* Divide the work of inserting n elements among nt threads*/
    ThreadArgs* args = divideWork(numbers, n, nt, tree);

    /* Create nt threads. Each thread calls the startThread function with a different input parameter */
    pthread_t threads[nt];
    for(int i = 0; i < nt; i++){
        assert(pthread_create(&threads[i], NULL, startThread, &args[i]) == 0);
    }

    /* Wait for each thread to complete its job */
    for(int i = 0; i < nt; i++){
        assert(pthread_join(threads[i], NULL) == 0);
    }

    /* Print the tree */
    printf("In order traversal of the tree: \n");
    printInOrder(tree);
    printf("\n");
    
    /* Destroy the tree */
    destroyTree(tree);
    free(args);

    return 0;
}
