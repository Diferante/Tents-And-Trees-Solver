#ifndef CHARSTACK_H_INCLUDED
#define CHARSTACK_H_INCLUDED

typedef struct _stack Stack;

Stack* initStack(unsigned int initial_size, unsigned int item_size);
void freeStack(Stack*);
int isEmpty(Stack*);
void push( Stack*, void* item);
void pop(Stack*, void* dest);
int ItemExists(Stack *stack, void *item, int equal(void *, void *));


#endif // CHARSTACK_H_INCLUDED
