#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char ** argv)
{
    /* There should at least be 3 arguments in addition to run2: 2 for the first command, and the rest for the second command */
    if (argc < 4) {
        fprintf(stderr, "Usage: %s cmd1 cmd1_arg cmd2 [cmd2_args ..]\n", argv[0]);
        return 1;
    }

    // Creates 2 child processes (argv[1] and argv[2]) using execlp
    // Second child runs arv[3] arvp4p[ using execvp

   // parent needs to wait

    // create first child process
    pid_t pid1 = fork();
    if (pid1 < 0) {
      perror("fork fail");
      return 1;
// pid1 < 0, fork failed, == 0 this is child process, > 0 is the parent
    }

    if (pid1 == 0) {
      execlp(argv[1], argv[1], argv[2], NULL);
      perror("execlp failed");
      exit(1);
    }

    // second child process (uses execvp() with variable num arguments

    pid_t pid2 = fork();
    if (pid2 < 0) {
      perror("fork failed");
      return 1;
    }

    // inside second child starts at argv[3] ends w NULL (array)
    if (pid2 == 0) {
      char **cmd2_args = &argv[3]; // argv[3] till [argc-1]
      execvp(cmd2_args[0], cmd2_args);
      perror("execvp failed");
      exit(1);
    }



	
    /* TODO: Create 2 child processes. Each child process must execute one of the two commands. Use execlp and execvp to do this.
	     The parent must reap the dead child processes before exiting. 
	     Print the exit status of the children using the following printf statement.
	     printf("exited=%d exitstatus=%d\n", WIFEXITED(status), WEXITSTATUS(status)); */

    int status;

    waitpid(pid1, &status, 0);
    printf("exited=%d exitstatus=%d\n", WIFEXITED(status), WEXITSTATUS(status));

    waitpid(pid2, &status, 0);
    printf("exited=%d exitstatus=%d\n", WIFEXITED(status), WEXITSTATUS(status));

    return 0;
}
