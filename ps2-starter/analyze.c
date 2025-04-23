#include "analyze.h"
#include <string.h>
#include <stdlib.h>

void analyzeURL(char* url, char* host, int* port, char* doc)
{
    char* p = url;

    // skip hhtp:// if there
    if (strncmp(p, "http://", 7) == 0)
        p += 7;

    // extract host
    char* hstart = p;
    while (*p && *p != ':' && *p != '/')
        p++;
    int hlen = p - hstart;
    strncpy(host, hstart, hlen);
    host[hlen] = '\0';

    // default to 80, override if :<num> is there
    *port = 80;
    if (*p == ':') {
        p++;
        *port = atoi(p);
        while (*p && *p != '/')
            p++;
    }

    // Rest is document path, include leading slash
    if (*p == '\0') {
        strcpy(doc, "/");
    } else {
        strcpy(doc, p);
    }
}
