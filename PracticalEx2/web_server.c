#define PORT 6789
#define MAXREQ (4096 * 1024)

#include <sys/socket.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>

char buffer[MAXREQ], body[MAXREQ], msg[MAXREQ];
void error(const char *msg)
{
    perror(msg);
    exit(1);
}
int main()
{
    int sockfd, newsockfd;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);
    if (bind(sockfd, (struct sockaddr *)&serv_addr,
             sizeof(serv_addr)) < 0)
    {
        error("ERROR on binding");
    }
    listen(sockfd, 5);
    while (1)
    {
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr,
                           &clilen);
        if (newsockfd < 0)
            error("ERROR on accept");
        bzero(buffer, sizeof(buffer));
        n = read(newsockfd, buffer, sizeof(buffer) - 1);
        if (n < 0)
            error("ERROR reading from socket");
        snprintf(body, sizeof(body),
                 "<html>\n<body>\n"
                 "<h1>Hello web browser</h1>\nYour request was\n"
                 "<pre>%s</pre>\n"
                 "</body>\n</html>\n",
                 buffer);
        snprintf(msg, sizeof(msg),
                 "HTTP/1.1 200 OK\n"
                 "Content-Type: text/html\n"
                 "Content-Length: %ld\n\n%s",
                 strlen(body), body);
        n = write(newsockfd, msg, strlen(msg));
        if (n < 0)
            error("ERROR writing to socket");
        close(newsockfd);
    }
}