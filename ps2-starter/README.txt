2. Code description

1. “Why This Works" explanation:

## Part 1

We break the URL string into:
    protocol (optional)
    hostname,
    port (optional)
    document path

We do so by using pointer arithmetic and character scans.

First we check and strip 'http://' if it's present, then scan until ':' or '/' to get the hostname and copy it into the host[] buffer. If a colon occurs, we parse the digits after with atoi() to set the port, else it's default 80. Then we copy the remainer (after /) into doc[]. If there's no '/' then we add one. 


## Part 2

We do HTTP GET over a TCP socket. We use gethostbyname to resolve host to an IP address, then use AF_INET to populate sockaddr_in, htons() for the network order port, and first returned address. After creating the socket we call connect() to open a TCP session to the webserver. Then we write a request "GET /path\n" using write() and call readResponse() (a helper function) that calls read() repeatedly. Then we printf() the raw page.

## Part 3

We use html2text as a HTML parser. After using part 2 fetching the raw HTML, we open a write-mode pipe to html2text using popen().Then fwrite() the HTML buffer into pipe's stdin, then pclose() the process. html2text strips allt ags, scripts, and markup, making it all plain tet.

 
