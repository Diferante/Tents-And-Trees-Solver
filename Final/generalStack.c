#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "generalStack.h"

struct node {
    void *Items;
    struct node* next;
};

struct _stack {
    struct node* head;
    unsigned int i; // First empty space
    unsigned int n; // Current table size
    unsigned int itemSize;
};



Stack* initStack(unsigned int initial_size, unsigned int item_size) {
    Stack* newSt;
    newSt = (Stack*) malloc(sizeof(Stack));
    if(newSt == NULL) exit(0);

    newSt->head = (struct node*) malloc(sizeof(struct node));
    if(newSt->head == NULL) exit(0);

    newSt->head->Items = malloc(initial_size*item_size);
    if(newSt->head->Items == NULL) exit(0);

    newSt->head->next = NULL;
    newSt->n = initial_size;
    newSt->itemSize = item_size;
    newSt->i = 0;
    return newSt;
};

int isEmpty(Stack* stack){
    return stack->i == 0 && stack->head->next == NULL;
    }

void freeStack(Stack* stack) {
    struct node* old;

    if(stack == NULL) exit(0);
    while(stack->head != NULL) {
        old = stack->head;
        stack->head = stack->head->next;
        free(old->Items);
        free(old);
    }
    free(stack);
}

void push( Stack* stack, void* item) {
    struct node *head, *new_node;
    unsigned int i, n, itemSize;

    if(stack == NULL) exit(0);

    head = stack->head;
    i = stack->i;
    n = stack->n;
    itemSize = stack->itemSize;

    if(i == n) {
        n = 2*n;
        // Check for overflow, since i == old n :
        if(i >= n) exit(0);
        new_node = (struct node*) malloc(sizeof(struct node));
        if(new_node == NULL) exit(0);

        new_node->Items = malloc(n*itemSize);
        if(new_node->Items == NULL) exit(0);

        new_node->next = head;
        head = new_node;
        i = 0;
        stack->head = head;
        stack->n = n;
    }

    memcpy(head->Items + i*itemSize, item, itemSize);
    i++;

    stack->i = i;
}

void pop(Stack* stack, void* dest) {
    struct node* old;

    if(stack == NULL) exit(0);
    if(stack->i == 0) {
        if(stack->head->next == NULL) exit(0);

        stack->n /= 2;
        old = stack->head;
        stack->head = stack->head->next;
        free(old->Items);
        free(old);
        stack->i = stack->n;
    }

    stack->i--;
    memcpy(dest, stack->head->Items + (stack->i)*(stack->itemSize), stack->itemSize);

}
