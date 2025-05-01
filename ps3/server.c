#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <dirent.h>

/* prints the error that is encountered and terminate the program */
void checkError(int status,int line) {
   if (status < 0) {
      printf("socket error(%d)-%d: [%s]\n",getpid(),line,strerror(errno));
      exit(-1);
   }
}

/* creates a server-side socket that binds to the given port number and listens for TCP connect requests */
int bindAndListen(int port) {
   int sid = socket(PF_INET,SOCK_STREAM,0);
   struct sockaddr_in addr;
   addr.sin_family = AF_INET;
   addr.sin_port   = htons(port);
   addr.sin_addr.s_addr = INADDR_ANY;
   int status = bind(sid,(struct sockaddr*)&addr,sizeof(addr));
   checkError(status,__LINE__);
   status = listen(sid,10);
   checkError(status,__LINE__);
   return sid;
}

/* reaps dead children */
void cleanupDeadChildren() {
   int status = 0;
   pid_t deadChild; // reap the dead children
   do {
      deadChild = waitpid(0,&status,WNOHANG);
   } while (deadChild > 0);
}

int main(int argc,char* argv[]) {
   if (argc < 3) {
      printf("usage is: server <port1> <port2>\n");
      return 1;
   }

   /* Create two sockets: one to receive SQL commands and another for service */
   int sid = bindAndListen(atoi(argv[1]));
   int srv = bindAndListen(atoi(argv[2]));

   while(1) {

        /* TODO: Listen for input on both sockets.
           *  If a client communicates to sid, run a unique instance of SQLite to respond to SQLite queries from the client. Reap dead children using cleanupDeadChildren.
           *       The executable for SQLite is sqlite3 that takes as argument a database foobar.db.
           *       Note that if you are coding this on your localhosts, you may need to install SQLite using 'sudo apt install sqlite3'.
           *  If a client communicates to srv, compare the received string with '$die!'. If it matches, terminate the server process.
           *
           * Hint: use the select API to listen to both ports sid and srv. Set nfds as the max of two fds + 1.
        */
   }
   
   close(srv);
   close(sid);
   cleanupDeadChildren();
   printf("terminated...\n");
   
   return 0;
}
