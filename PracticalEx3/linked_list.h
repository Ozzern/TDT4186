struct node{
    int data;
    int key;
    struct node *next;
};

void insertFirst(int key, int data);

int isEmpty();

int delete_from_list(int key);