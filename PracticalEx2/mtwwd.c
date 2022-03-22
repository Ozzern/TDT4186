#define MAXREQ (4096 * 1024)

#include <sys/socket.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "bbuffer.h"

char buffer[MAXREQ], body[MAXREQ], msg[MAXREQ], html_buffer[MAXREQ], path[MAXREQ];

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void set_path(char *raw_request, char *directory)
{
    strcpy(path, raw_request);
    char *token;
    token = strtok(path, " ");
    token = strtok(NULL, " ");
    sprintf(path,"%s%s", directory, token);
}

int read_file(char *filename)
{
    char * buffer = 0;
    long length;
    // printf("FILENAME: %s\n", filename);
    FILE *f = fopen(filename, "rb");

    if (f)
    {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer = malloc(length);
        if (buffer)
        {
            fread(buffer, 1, length, f);
        }
        fclose(f);
        if(buffer){
            strcpy(html_buffer, buffer);
            return 1;
        }

    }
    strcpy(html_buffer, "<html><body><h1>404 Page Not Found</h1></body></html>\n");
    return 0;
}

int init_producer(int port) {
    int sockfd;
    struct sockaddr_in6 serv_addr;
    sockfd = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0)
        error("ERROR opening socket\n");
    int enable = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) == -1)
        error("setsockopt(SO_REUSEADDR) failed\n");
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin6_family = AF_INET6;
    serv_addr.sin6_addr = in6addr_any;
    serv_addr.sin6_port = htons(port);
    if (bind(sockfd, (struct sockaddr *)&serv_addr,
             sizeof(serv_addr)) < 0)
    {
        error("ERROR on binding\n");
    }
    return sockfd;
}

typedef struct WORKER_INFO {
    char * www_path;
    BNDBUF * bb;
} WORKER_INFO;

void *execute_fd(void* vargp) {
    WORKER_INFO *w_i = (WORKER_INFO *) vargp;
    BNDBUF *bb = w_i->bb;
    char * directory = w_i->www_path;
    while(1){
        int newsockfd = bb_get(bb);
        // sleep(1);
        // printf("FILE DESCRIPTOR: %i\n", newsockfd);
        int n;
        bzero(buffer, sizeof(buffer));
        n = read(newsockfd, buffer, sizeof(buffer) - 1);
        if (n < 0)
            error("ERROR reading from socket\n");
        set_path(buffer, directory);
        read_file(path);
        snprintf(body, sizeof(body),
                    "%s",
                    html_buffer);
        snprintf(msg, sizeof(msg),
                    "%s",
                    body);       
        n = write(newsockfd, msg, strlen(msg));
        if (n < 0)
            error("ERROR writing to socket\n");
        close(newsockfd);
    }
}

int main(int argc, char* argv[])
{
    // setvbuf (stdout, NULL, _IONBF, 0);
    if(argc != 5){
        printf("Wrong number of arguments supplied\n");
        return 0;
    }
    char *directory = argv[1];
    int port;
    sscanf(argv[2],"%d",&port);
    int threads;
    sscanf (argv[3],"%d",&threads);
    int bufferslots;
    sscanf (argv[4],"%d",&bufferslots);
    int sockfd = init_producer(port);
    listen(sockfd, 1);
    BNDBUF *bb = bb_init(bufferslots);
    WORKER_INFO w_i = {directory, bb};
    pthread_t t_id[threads];
    for (int i = 0; i < threads; i++) {
        pthread_create(&t_id[i], NULL, execute_fd, (void *)&w_i);
        // printf("THREAD: %ld\n", t_id[i]);
    }
    int n;
    int newsockfd;
    socklen_t clilen;
    struct sockaddr_in cli_addr;
    while (1)
    {
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr,
                           &clilen);
        if (newsockfd < 0)
            error("ERROR on accept\n");
        bb_add(bb, newsockfd);
    }
}