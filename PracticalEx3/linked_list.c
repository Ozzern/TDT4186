#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

struct node {
    pid_t pid;
    char * command;
    struct node *next;
};

void push(struct node ** head, pid_t pid, char* command){
    struct node* node = malloc(sizeof(struct node));
    node->pid = pid;
    node->command = command;
    node->next = (*head);
    (*head) = node;
}

void delete_by_pid(struct node ** head, pid_t pid){
    struct node * temp = *head, *prev;

    if (temp != NULL && temp->pid == pid){
        *head = temp->next;
        free(temp);
        return;
    }

    while (temp != NULL && temp->pid != pid){
        prev = temp;
        temp = temp->next;
    }

    if(temp == NULL){
        return;
    }

    prev->next = temp->next;
    free(temp);
}

void print_list(struct node* node){
    if(node == NULL){
        puts("No jobs currently");
        return;
    }
    puts("Current jobs");
    puts("------------");
    while (node != NULL) {
        printf("PID %d: %s\n", node->pid, node->command);
        node = node->next;
    }
    puts("");
}

/*
int main()
{
    struct node* head = NULL;
 
    push(&head, 1, "TEST1");
    push(&head, 2, "TEST2");
    push(&head, 3, "TEST3");
    push(&head, 4, "TEST4");
 
    puts("Created Linked List: ");
    print_list(head);
    delete_by_pid(&head, 1);
    puts("\nLinked List after Deletion of 1: ");
    print_list(head);
    return 0;
}
*/


/*
struct list {
    struct node *head;
    struct node **tail;
};

void enqueue(struct list *list, struct node *item){
    item->next = NULL;
    *list->tail = item;
    list->tail = &item->next;
}

void delete_by_pid(struct list *list){
    // struct node *item;
    // item = *list->tail;
    printf("PID: %d\n", (*list->tail)->pid);
}

int main() {
    struct list *linked_list = malloc(sizeof(struct list));
    linked_list->head = malloc(sizeof(struct node));
    linked_list->tail = malloc(sizeof(struct node));


    struct node *value_1 = malloc(sizeof(struct node));
    char* cmd = "TEST";
    value_1->command = cmd;
    value_1->pid = 1123;

    struct node *value_2 = malloc(sizeof(struct node));
    char* cmd2 = "TEST2";
    value_2->command = cmd2;
    value_2->pid = 123;


    enqueue(linked_list, value_1);
    delete_by_pid(linked_list);
}
*/
