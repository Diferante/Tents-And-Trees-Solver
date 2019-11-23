#include <stdlib.h>
#include <limits.h>

struct node {
    char *c;
    struct node* next;
};

struct node* head;
unsigned int i;
unsigned int n;

void initStack(unsigned int initial_size) {
    n = initial_size;
    head = (struct node*) malloc(sizeof(struct node));
    if(head == NULL) exit(0);
    head->c = (char*) malloc(n);
    if(head->c == NULL) exit(0);

    head->next = NULL;
    i = 0;
};

void freeStack() {
    struct node* old;
    while(head != NULL) {
        old = head;
        head = head->next;
        free(old->c);
        free(old);
    }
}

void push( char c) {
    struct node* new_node;

    if(i == n) {
        n = 2*n;
        // Check for overflow, since i == old n :
        if(i>=n) exit(0);
        new_node = (struct node*) malloc(sizeof(struct node));
        if(new_node == NULL) exit(0);

        new_node->c = (char*) malloc(n);
        if(new_node->c == NULL) exit(0);

        new_node->next = head;
        head = new_node;
        i = 0;
    }
    head->c[i] = c;
    i++;
}

char pop() {
    struct node* old;

    if(i == 0) {
        if(head->next == NULL) exit(0);

        n = n/2;
        old = head;
        head = head->next;
        free(old->c);
        free(old);
        i = n;
    }

    i--;
    return head->c[i];
}
