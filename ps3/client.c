#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>
#include <assert.h>

/* readString reads input from the stdin and returns the \n\0 terminated string  
 * If EOF is read, it returns NULL */
char* readString() {
   int   l = 2; // start with enough space to hold the emtpy string (with \n).
   char* s = malloc(l * sizeof(char));
   int i = 0;
   char ch;
   while ((ch = getchar()) != '\n' && ch != EOF) {
      if (i == l-2) {
         s = realloc(s,l*2*sizeof(char));
         l *= 2;
      }
      s[i++] = ch;
   }
   if (ch == EOF) {
      free(s);
      return NULL;
   } else {
      s[i] = ch;
      s[i+1] = 0;
      return s;
   }
}

/* prints the error that is encountered and terminate the program */
void checkError(int status,int line) {
   if (status < 0) {
      printf("socket error(%d)-%d: [%s]\n",getpid(),line,strerror(errno));
      exit(-1);
   }
}

int main(int argc,char* argv[])  {
   if (argc < 3) {
      printf("usage is: client <hostname> <port>\n");
      return 1;
   }

   /* Connect to the server hosted on <hostname> <port>. This should be the same as port1 of server */
   char* hostname = argv[1];
   int sid = socket(PF_INET,SOCK_STREAM,0);
   struct sockaddr_in srv;
   struct hostent *server = gethostbyname(hostname);
   srv.sin_family = AF_INET;
   srv.sin_port   = htons(atoi(argv[2])); // same as sid in server
   memcpy(&srv.sin_addr.s_addr,server->h_addr,server->h_length);
   int status = connect(sid,(struct sockaddr*)&srv,sizeof(srv));
   checkError(status,__LINE__);
   
   /* TODO: Read SQLite commands from stdin using the function readString. 
               * If the function returns a valid command, send it to the server. 
               * If EOF is returned, prevent the process from terminating and continue to read the responses from the server.
            Read response from the server and print it out to stdout.
            Hint: use the select API to read input from both the file descriptors above.
   */

   return 0;
}
