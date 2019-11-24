#include <stdlib.h>
#include <limits.h>
#include <string.h>

struct node {
    void *Items;
    struct node* next;
};

struct node* head;
unsigned int i;
unsigned int n; // Current table size
unsigned int itemSize;

void initStack(unsigned int initial_size, unsigned int item_size) {
    n = initial_size;
    itemSize = item_size;
    head = (struct node*) malloc(sizeof(struct node));
    if(head == NULL) exit(0);
    head->Items = malloc(n*itemSize);
    if(head->Items == NULL) exit(0);

    head->next = NULL;
    i = 0;
};

void freeStack() {
    struct node* old;
    while(head != NULL) {
        old = head;
        head = head->next;
        free(old->Items);
        free(old);
    }
}

void push( void* item) {
    struct node* new_node;

    if(i == n) {
        n = 2*n;
        // Check for overflow, since i == old n :
        if(i>=n) exit(0);
        new_node = (struct node*) malloc(sizeof(struct node));
        if(new_node == NULL) exit(0);

        new_node->Items = malloc(n*itemSize);
        if(new_node->Items == NULL) exit(0);

        new_node->next = head;
        head = new_node;
        i = 0;
    }

    memcpy(head->Items + i*itemSize, item, itemSize);
    i++;
}

void pop(void* dest) {
    struct node* old;

    if(i == 0) {
        if(head->next == NULL) exit(0);

        n = n/2;
        old = head;
        head = head->next;
        free(old->Items);
        free(old);
        i = n;
    }

    i--;
    memcpy(dest, head->Items + i*itemSize, itemSize);
}
