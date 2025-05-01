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

/* prints the error that is encountered and terminate the program */
void checkError(int status,int line) {
   if (status < 0) {
      printf("socket error(%d)-%d: [%s]\n",getpid(),line,strerror(errno));
      exit(-1);
   }
}

int main(int argc,char* argv[]) {
   if (argc < 3) {
      printf("usage is: service <hostname> <port>\n");
      return 1;
   }

   /* Connect to the server hosted on <hostname> <port>. The port should be the same as port2 on server */
   char* hostname = argv[1];
   int sid = socket(PF_INET,SOCK_STREAM,0);
   struct sockaddr_in srv;
   struct hostent *server = gethostbyname(hostname);
   srv.sin_family = AF_INET;
   srv.sin_port   = htons(atoi(argv[2])); // same as srv in server
   memcpy(&srv.sin_addr.s_addr,server->h_addr,server->h_length);
   int status = connect(sid,(struct sockaddr*)&srv,sizeof(srv));
   checkError(status,__LINE__);

   /* TODO: Connect to the server and send the string '$die!' to terminate the server. */

   const char *msg = "$die!";
   int len = strlen(msg), sent = 0;
   while (sent < len) {
      int w = write(sid, msg + sent, len - sent);
      checkError(w, __LINE__);
      sent += w;
   }

   close(sid);
   return 0;
}
