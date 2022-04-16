#ifndef DLL_H
#define DLL_H

/* Linked list copied from https://stackoverflow.com/questions/51223203/c-shell-with-background-tasks */
/* Trimmed to fit task */

struct list_head {
    struct list_head *next, *prev;
};

/* initialize "shortcut links" for empty list */
extern void
list_init(struct list_head *head);

/* insert new entry after the specified head */
extern void 
list_add(struct list_head *new, struct list_head *head);

/* deletes entry from list and reinitialize it, returns pointer to entry */
extern struct list_head* 
list_del(struct list_head *entry);

/* tests whether a list is empty */
extern int 
list_empty(struct list_head *head);

#endif