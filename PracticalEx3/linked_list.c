#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

struct node {
    pid_t pid;
    char * command;
    struct node *next;
};

struct list {
    struct node *head;
    struct node **tail;
};

void enqueue(struct list *list, struct node *item){
    item->next = NULL;
    *list->tail = item;
    list->tail = &item->next;
}

void delete_by_pid(pid_t pid, struct list *list){
    struct node *item = *list->tail;
    while (1) {
        printf("PID: %ls", &item->pid);
        if(item->next == NULL){
            break;
        }
        item = item->next;
    }
}

int main() {
    struct list *linked_list;
    struct node *value_1;
    linked_list->head = malloc(sizeof(struct node));
    linked_list->tail = malloc(sizeof(struct node));
    char* cmd = "TEST";
    value_1->command = malloc(strlen(cmd));
    value_1->command = cmd;
    value_1->pid = malloc(sizeof(pid_t));
    value_1->pid = 123;
    value_1->next = malloc(sizeof(struct node));
    enqueue(linked_list, value_1);
    delete_by_pid(123, linked_list);
}
