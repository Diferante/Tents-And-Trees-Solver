#ifndef CHARSTACK_H_INCLUDED
#define CHARSTACK_H_INCLUDED

void initStack(unsigned int initial_size, unsigned int item_size);
void freeStack();
void push( void* item);
void pop(void* dest);


#endif // CHARSTACK_H_INCLUDED
