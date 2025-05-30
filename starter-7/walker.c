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

    shared_val *sv = arg->sv; 

    while (1) {
        /* TODO:
         * A thread must wait if the user is not at its location and the walk is not over. 
            * If the predicate is false because the walk is over, terminate the thread.
            * If the predicate is false because the user walks to this location, check the current location value at this location
                *  If the value is 0, set over to 1 and send a signal to all threads to terminate
                *  If the value != 0, update the user location, reduce the current location value and signal the appropriate thread to continue the walk. 
         */

        while (sv -> loc != id && !sv->over) { // wait till thread's turn or walk over
            pthread_cond_wait(&sv->cond[id], &sv->mtx);
        }

        // if walk over exit
        if (sv->over) {
            pthread_mutex_unlock(&sv->mtx);
            break;
        }

	// if value 0 at arrival, mark over and wake all
        if (value == 0) {
	    sv->over = 1;
            for (int i = 0; i< n; i++) {
                pthread_cond_signal(&sv->cond[i]);
            }
            pthread_mutex_unlock(&sv->mtx);
            break;
        } else { // compute next location, print, decrement, signal next thread
            int steps = value;      
	    int next = (id + steps) % n;
            printf("At i=%d, user moves forward by %d space to i=%d. New value is %d\n", id, steps, next, value - 1);
            value--;
            sv->loc = next;
            pthread_cond_signal(&sv->cond[next]);
        }
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    if(argc != 2) {
        printf("Usage: %s n\n", argv[0]);
        return 0;
    }

    int n = atoi(argv[1]);          // number of locations (number of threads) 
    char *end;
    long n_long = strtol(argv[1], &end, 10);
    if (*end != '\0' || n_long <= 0) {
        fprintf(stderr, "Usage: %s n (where n must be a positive integer)\n", argv[0]);
        return 1;
    }
    int n = (int)n_long;
    printf("n = %d\n", n);

    /* Initialize the shared_val */
    shared_val *val = malloc(sizeof(shared_val));
    if (!val) {
        perror("malloc shared_val");
        exit(EXIT_FAILURE);
    }

    val->loc = 0;                   // user always starts walking at location 0 
    val->over = 0;                  // over is initialized to 0. It is set to 1 when user arrives at a location where value is 0
    pthread_mutex_init(&val->mtx, NULL);        // initialize the mutex
    val->cond = malloc(sizeof(pthread_cond_t) * n);
    if (!val->cond) {
        perror("malloc cond array");
        exit(EXIT_FAILURE);
    }
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


    for (int i = 0; i < n; i++) {
        args[i].n = n;
        args[i].id = i;
        args[i].value = i + 1;
        args[i].sv = val;
        status = pthread_create(&threads[i], NULL, thread_main, &args[i]);
        assert(status == 0);
    }

    pthread_mutex_lock(&val->mtx);
    pthread_cond_signal(&val->cond[0]);
    pthread_mutex_unlock(&val->mtx);
    for (int i = 0; i< n; i++) {
        status = pthread_join(threads[i], NULL);
        assert(status == 0);
    }


    printf("Ends at %d\n", val->loc);
    pthread_mutex_destroy(&val->mtx);
    for (int i = 0; i < n; i++) {
        pthread_cond_destroy(&val->cond[i]);
    }
    free(val->cond);
    free(val);


    return 0;
}
