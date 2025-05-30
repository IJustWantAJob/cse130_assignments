#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <assert.h>

typedef struct shared_val_tag {
    int loc;                        // user's current location
    int over;                       // if over = 0, walk is not over. if over=1, walk is over. 
    pthread_mutex_t mtx;            // a mutex
    pthread_cond_t *cond;           // array of condition variables
} shared_val;

typedef struct thread_arg_tag {
    int n;                          // number of threads
    int id;                         // id of each thread from 0 to n-1
    int value;                      // value at each thread. initialized to id+1 and decremented each time a user walks to this location
    shared_val *sv;                 // pointer to the shared_val containing the user state
} thread_arg_t; 

void* thread_main(void* thread_arg) {
    thread_arg_t* arg = thread_arg;
    int n = arg->n;
    int id = arg->id;
    int value = arg->value;

    while (1) {
        /* TODO:
         * A thread must wait if the user is not at its location and the walk is not over. 
            * If the predicate is false because the walk is over, terminate the thread.
            * If the predicate is false because the user walks to this location, check the current location value at this location
                *  If the value is 0, set over to 1 and send a signal to all threads to terminate
                *  If the value != 0, update the user location, reduce the current location value and signal the appropriate thread to continue the walk. 
         */

    }
}

int main(int argc, char* argv[]) {
    if(argc != 2) {
        printf("Usage: %s n\n", argv[0]);
        return 0;
    }

    int n = atoi(argv[1]);          // number of locations (number of threads) 
    printf("n = %d\n", n);

    /* Initialize the shared_val */
    shared_val *val = malloc(sizeof(shared_val));
    val->loc = 0;                   // user always starts walking at location 0 
    val->over = 0;                  // over is initialized to 0. It is set to 1 when user arrives at a location where value is 0
    pthread_mutex_init(&val->mtx, NULL);        // initialize the mutex
    val->cond = malloc(sizeof(pthread_cond_t) * n);
    for (int i = 0; i < n; i++) {
        pthread_cond_init(&val->cond[i], NULL);     // initialize the condition variable for each thread
    }

    pthread_t threads[n];   //use to store thread ids
    thread_arg_t args[n];   //use to initialize arguments passed to each thread
    int status;             //use for error reporting

    /* TODO:
     * Create n threads. Each thread calls the thread_mai() function and takes an instance of thread_arg_t as argument
     * Start the walk by signalling to thread 0.
     * Wait for all the threads to finish.
     * Destroy all resources.
     */

    return 0;
}