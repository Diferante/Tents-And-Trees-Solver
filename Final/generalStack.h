/******************************************************************************
 * 2019-2020 
 * Authors - Haohua Dong e Diogo Antunes
 *
 * DESCRIPTION
 *  Header file for a general implementation of a single type stack.
 *	The item's size must be specified at runtime for Initialization and
 *  an item comparison function must be supplied for Search.
 *
 *  Function list:
 *    A) Initialization & Termination
 *        initStack
 *        freeStack
 *
 *    B) Properties
 *        isStackEmpty
 *
 *    D) Search
 *        itemExists
 *
 *    E) Insertion & Removal
 *       push
 *		 pop
 *
 *	Dependencies:
 *    stdlib.h
 *	  string.h
 *
 *****************************************************************************/

#ifndef GENERALSTACK_H_INCLUDED
#define GENERALSTACK_H_INCLUDED

typedef struct _stack Stack;

/* Description: Allocates a Stack object and initializes it with the
 * specified size.
 * Arguments: The initial size of the stack in items, and the
 * size of each item in bytes.
 * Return: Pointer to the created Stack.
 * */
Stack *initStack(unsigned int initial_size, unsigned int item_size);

/* Description: Frees a Stack object and its contents.
 * */
void freeStack(Stack *);

/* Description: Returns 1 if the Stack is empty, 0 otherwise.
 * */
int isStackEmpty(Stack *);

/* Description: Copies an item to the top of the Stack.
 * Arguments: Pointer to the Stack and pointer to the item to be copied.
 * */
void push(Stack *, void *item);

/* Description: Copies an item from the top of the Stack and deletes it from the
 * Stack.
 * Arguments: Pointer to the Stack and pointer with the destination address
 * */
void pop(Stack *, void *dest);

/* Description: Returns 1 if the item already exists in the Stack, 0 otherwise.
 * Arguments:
 *  Stack *     - Pointer to Stack
 *  item        - Pointer to item to search for.
 *  max_depth   - Maximum number of items to check. -1 for no limit.
 *  equal       - Function used to compare the items, must be 0 for different
 *                items.
 * */
int itemExists(Stack *, void *item, int max_depth, int equal(void *, void *));

#endif // GENERALSTACK_H_INCLUDED
