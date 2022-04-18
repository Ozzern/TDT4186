#include <unistd.h>
#include <sys/types.h>

struct node{
    pid_t pid;
    char* command;
    struct node *next;
};

void push(struct node ** head, pid_t pid, char* command);

void delete_by_pid(struct node ** head, pid_t pid);

void print_list(struct node* node);