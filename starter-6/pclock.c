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
	pthread_mutex_t mutex;
	pthread_cond_t cond_guess;
	pthread_cond_t cond_result;


    /* TODO: Add the required mutexes and condition variables */
} thread_arg_t;

void *thread_c(void* consumer_thread_data) {
	thread_arg_t *arg = (thread_arg_t*) consumer_thread_data;
	gmn_t *sb = arg->sb;
	int res;

	while (1) {
		pthread_mutex_lock(&arg->mutex);
		while (sb->status != 1) {
			pthread_cond_wait(&arg->cond_guess, &arg->mutex);
		}

		res = gmn_check(sb);
		sb->status = 2;
		pthread_cond_signal(&arg->cond_result);
		pthread_mutex_unlock(&arg->mutex);

		if (res == 0)
			break;
	}
	return NULL;

}


void *thread_p(void* producer_thread_data) {
	thread_arg_t *arg = (thread_arg_t*) producer_thread_data;
	gmn_t *sb = arg->sb;
	int low = 1, high = MAX_VALUE;
	int guess, res;

	while (1) {
		guess = (low + high) / 2;
		pthread_mutex_lock(&arg->mutex);
		sb->guess = guess;
		sb->status = 1;
	
		printf("My guess is %d.\n\n", guess);
		fflush(stdout);

		pthread_cond_signal(&arg->cond_guess);
		while (sb->status != 2) {
			pthread_cond_wait(&arg->cond_result, &arg->mutex);
		}

		res = sb->result;
		printf("%s", sb->message);
		if (res != 0)
			printf("\n");
		if (res == 0) {
			pthread_mutex_unlock(&arg->mutex);
			break;
		}

		sb->status = 0;
		pthread_mutex_unlock(&arg->mutex);
		if (res == 1)
			low = guess + 1;
		else
			high = guess - 1;
	}
	return NULL;
}

int main(int argc,char* argv[]) {
    /* TODO
     * Create an instance of thread_arg_t and initialize it. The value to be guessed is taken as input from the command line arguments.
     * Create two threads - one each for the player and the checker
     * Join the threads and reap the resources (destroy the mutexes and condition variables used).
    */
 //   return 0;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <number to guess>\n", argv[0]);
		return EXIT_FAILURE;
	}

	int secret = atoi(argv[1]);
	if (secret < 1 || secret > MAX_VALUE) {
		fprintf(stderr, "Usage: %s <number to guess>\n", argv[0]);
		return EXIT_FAILURE;
	}


	gmn_t *sb = gmn_init(secret);
	thread_arg_t arg;
	arg.sb = sb;
	pthread_mutex_init(&arg.mutex, NULL);
	pthread_cond_init(&arg.cond_guess, NULL);
	pthread_cond_init(&arg.cond_result, NULL);

	pthread_t tid_c, tid_p;
	if (pthread_create(&tid_c, NULL, thread_c, &arg) != 0 || pthread_create(&tid_p, NULL, thread_p, &arg) !=0) {
		perror("pthread_create");
		return EXIT_FAILURE;	
	}
	pthread_join(tid_p, NULL);
	pthread_join(tid_c, NULL);
	pthread_mutex_destroy(&arg.mutex);
	pthread_cond_destroy(&arg.cond_guess);
	pthread_cond_destroy(&arg.cond_result);

	free(sb);
	return EXIT_SUCCESS;
}

