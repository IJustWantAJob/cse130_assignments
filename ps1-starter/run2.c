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

    /* TODO: Create 2 child processes. Each child process must execute one of the two commands. Use execlp and execvp to do this.
	     The parent must reap the dead child processes before exiting. 
	     Print the exit status of the children using the following printf statement.
	     printf("exited=%d exitstatus=%d\n", WIFEXITED(status), WEXITSTATUS(status)); */

    int status;

    return 0;
}
