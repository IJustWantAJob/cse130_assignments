#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define READEND 0
#define WRITEEND 1

int main(int argc,char* argv[]) {  
   if (argc < 3) {
      printf("Usage: pipes a b, where a and b are integers. \n");
      exit(1);
   }
   
   /* TODO: Implement the command: "ls /dev | xargs | cut -d ' ' -f<a>-<b>" 
      	    where <a> and <b> are integers that specify the range and are taken as input from the command line. */


   // ASCII to Integer
   int a = atoi(argv[1]);
   int b = atoi(argv[2]);

   int pipe1[2];
   int pipe2[2];  

   if (pipe(pipe1) == -1) { perror("pipe1"); exit(1); }
   if (pipe(pipe2) == -1) {
     perror("pipe2");
     exit(1);
   }



   pid_t pid1 = fork();
   if (pid1 == 0) {
     // child 1 ls / dev
     dup2(pipe1[WRITEEND], STDOUT_FILENO); // stdout to pipe1 write
     close(pipe1[READEND]);
     close(pipe1[WRITEEND]);
     close(pipe2[READEND]);
     close(pipe2[WRITEEND]);
    
     execlp("ls", "ls", "/dev", NULL);
     perror("execlp ls failed");
     exit(1);
   }
   

   pid_t pid2 = fork();
   if (pid2 == 0) {
     // child 2 xargs
     dup2(pipe1[READEND], STDIN_FILENO);  // raed pipe1
     dup2(pipe2[WRITEEND], STDOUT_FILENO); 

     close(pipe1[READEND]);
     close(pipe1[WRITEEND]);
     close(pipe2[READEND]);
     close(pipe2[WRITEEND]);

     execlp("xargs", "xargs", NULL);
     perror("execlp xargs failed");
     exit(1);

   }

    pid_t pid3 = fork();
    if (pid3 == 0) {
      // child 3 cut -d ' ' -f<a><b>
      dup2(pipe2[READEND], STDIN_FILENO);
      close(pipe1[READEND]); 
      close(pipe1[WRITEEND]);
      close(pipe2[READEND]); 
      close(pipe2[WRITEEND]);

    // convert a and b to string fields
      char field_range[20];
      snprintf(field_range, sizeof(field_range), "-f%d-%d", a, b);

      execlp("cut", "cut", "-d", " ", field_range, NULL);
      perror("execlp cut failed");
      exit(1);
   }

   // close all pipe ends and wait
   close(pipe1[READEND]);
   close(pipe1[WRITEEND]);
   close(pipe2[READEND]);
   close(pipe2[WRITEEND]);

   
   int status;
   waitpid(pid1, &status, 0);
   waitpid(pid2, &status, 0);
   waitpid(pid3, &status, 0);



   return 0;
}


