#include <stdio.h>

char getChoice() {
    char choice;
    printf("Please enter \"s\" (schedule), \"l\" (list), \"c\" (cancel), \"x\" (exit): ");
    scanf("%c", &choice);
    return choice;
}

void schedule() {
    printf("Schedule\n");
}

void list() {
    printf("List\n");
}

void cancel() {
    printf("Cancel\n");
}

void exit_program() {
    printf("Exit\n");
}

int main() {
    char choice;
    printf("Welcome to the alarm clock!");
    choice = getChoice();
    switch (choice)
    {
    case 's':
        schedule();
        break;
    case 'l':
        list();
        break;
    case 'c':
        cancel();
        break;
    case 'x':
        exit_program();
        break;
    
    default:
        break;
    }
}