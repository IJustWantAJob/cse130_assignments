#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include "guess-mt.h"

typedef struct thread_arg_tag {
    gmn_t* sb;
    /* TODO: Add the required mutexes and condition variables */
} thread_arg_t;

void *thread_c(void* consumer_thread_data);
void *thread_p(void* producer_thread_data);

int main(int argc,char* argv[]) {
    /* TODO
     * Create an instance of thread_arg_t and initialize it. The value to be guessed is taken as input from the command line arguments.
     * Create two threads - one each for the player and the checker
     * Join the threads and reap the resources (destroy the mutexes and condition variables used).
    */
    return 0;
}

void* thread_p(void *producer_thread_data) {
   /* TODO: This is the player thread.
    * Repeat the following until guess is correct
        guess a number between min and max. initially max is set to MAX_VALUE
        send the guess to the checker and wait for a result (thread_c)
        if result is 0 i.e. you guessed the number correctly, end the thread
        if result is -1 or 1, update the search interval and make a new guess
   */
   return NULL;
}

void* thread_c(void * consumer_thread_data) {
    /* TODO: This is the checker thread.
     * Repeat the following until guess is correct 
     *      wait for a guess from the player (thread_p) 
     *      call gmn_check() to compare the guess with the user input value
     *      send the result to thread_p
     */
    return NULL;
}
