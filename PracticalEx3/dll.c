#include <stdlib.h>
#include <stdio.h>
#include "dll.h"

/* Linked list copied from https://stackoverflow.com/questions/51223203/c-shell-with-background-tasks */
/* Trimmed to fit task */

/* initialize "shortcut links" for empty list */
void
list_init(struct list_head *head)
{
    head->next = head;
}

/* insert new entry after the specified head */
void 
list_add(struct list_head *new, struct list_head *head)
{
    new->prev = head;
    new->next = head->next;
    head->next = new;
    new->next->prev = new;
}

/* deletes entry from list and reinitialize it, returns pointer to entry */
struct list_head* 
list_del(struct list_head *entry)
{
    struct list_head *previous = entry->prev;
    struct list_head *following = entry->next;
    previous->next = following;
    following->prev = previous;
    entry->prev = NULL;
    entry->next = NULL;
    return entry;
}

/* tests whether a list is empty */
int 
list_empty(struct list_head *head)
{
    return (head == head->next);
}