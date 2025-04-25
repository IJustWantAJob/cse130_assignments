#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/select.h>
#include <assert.h>
#include "analyze.h"

void checkError(int status,int line) {
   if (status < 0) {
      printf("socket error(%d)-%d: [%s]\n",getpid(),line,strerror(errno));
      exit(-1);
   }
}


// helper function from wgethtml.c
//

char* readResponse(int sid) {
   size_t sz = 8;
      char* buf = malloc(sz);
      int done = 0, received = 0;
      while (!done) {
         int n = read(sid, buf + received, sz - received);
	 if (n > 0) {
            received += n;
	 } else if (n == 0) {
            done = 1;
	 } 
	 if (received == sz) {
            buf = realloc(buf, sz * 2);
	    sz *= 2;
	 }
      }
      buf[received] = '\0';
      return buf;
}

int main(int argc,char* argv[]) {
   if (argc < 2) {
      printf("Usage is: wgettext <url>\n");
      return 1;
   }

   
   //parse
   char host[512];
   int  port = 80;
   char url[512];
   analyzeURL(argv[1], host, &port, url);
   printf("[%s] [%d] [%s]\n", host, port, url);

   // hostname
   struct hostent* he = gethostbyname(host);
   if (!he) {
      fprintf(stderr, "Unknown host: %s\n", host);
      return 1;
   }

   // Build address and connect
   struct sockaddr_in addr = {0};
   addr.sin_family = AF_INET;
   addr.sin_port   = htons(port);
   memcpy(&addr.sin_addr, he->h_addr_list[0], he->h_length);

   int sid = socket(AF_INET, SOCK_STREAM, 0);
   checkError(sid, __LINE__);
   checkError(connect(sid, (struct sockaddr*)&addr, sizeof(addr)), __LINE__);

   // Send GET
   char req[1024];
   snprintf(req, sizeof(req), "GET %s\n", url);
   checkError(write(sid, req, strlen(req)), __LINE__);

   // Read full HTML response
   char* html = readResponse(sid);
   close(sid);

   // Pip thru html2text
   FILE* pipe = popen("html2text", "w");
   if (!pipe) {
      perror("popen");
      free(html);
      return 1;
   }
   fwrite(html, 1, strlen(html), pipe);
   pclose(pipe);

   free(html);
   return 0;
}

