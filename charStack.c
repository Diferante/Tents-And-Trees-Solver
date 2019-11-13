#include <stdlib.h>
struct node{
    char c;
    struct node* next;
};

struct node* head;

void initStack(){
    head = NULL;
};

void freeStack(){
    struct node* old;
    while(head != NULL){
        old = head;
        head = head->next;
        free(old);
    }
}

void push( char c){
    struct node* new_node;

    new_node = (struct node*) malloc(sizeof(struct node));
    if(new_node == NULL) exit(0);

    new_node->c = c;
    new_node->next = head;
    head = new_node;
}

char pop(){
    char c;
    struct node* old;

    if(head == NULL) exit(0);

    c = head->c;
    old = head;
    head = head->next;
    free(old);
    return c;
}
