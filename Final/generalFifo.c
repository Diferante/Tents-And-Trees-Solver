#include "generalFifo.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct node {
  void *Items;
  struct node *next;
  unsigned int n; // Table size
};

typedef struct {
  int x, y;
} Point;

struct _fifo {
  struct node *head;
  struct node *tail;
  unsigned int back;  // free index for queue
  unsigned int front; // occupied index for dequeue
  unsigned int itemSize;
  unsigned int initialSize;
  unsigned int tables;
};

Fifo *initFifo(unsigned int initial_size, unsigned int item_size) {
  Fifo *NewFifo;
  NewFifo = (Fifo *)malloc(sizeof(Fifo));
  if (NewFifo == NULL)
    exit(0);

  NewFifo->head = NULL;
  NewFifo->tail = NULL;
  NewFifo->initialSize = initial_size;
  NewFifo->itemSize = item_size;
  NewFifo->back = 0;
  NewFifo->front = 0;
  NewFifo->tables = 0;
  return NewFifo;
};

int isFifoEmpty(Fifo *fifo) {
  return fifo->back == fifo->front && fifo->head == fifo->tail;
}

void freeFifo(Fifo *fifo) {
  struct node *old;

  if (fifo == NULL)
    exit(0);
  while (fifo->head != NULL) {
    old = fifo->head;
    fifo->head = fifo->head->next;
    free(old->Items);
    free(old);
  }
  free(fifo);
}

void queue(Fifo *fifo, void *item) {
  struct node *tail, *new_node;
  unsigned int back, n, itemSize;

  if (fifo == NULL)
    exit(0);

  tail = fifo->tail;
  n = 0;
  if (tail != NULL)
    n = tail->n;
  back = fifo->back;
  itemSize = fifo->itemSize;

  if (back == n) { // If tail == NULL, back and n equal 0.
    // Table full, alocate more.
    fifo->tables++;
    n = fifo->tables * fifo->initialSize;
    // Check for overflow
    if (fifo->tables > n)
      exit(0);
    new_node = (struct node *)malloc(sizeof(struct node));
    if (new_node == NULL)
      exit(0);

    new_node->n = n;
    new_node->next = NULL;
    new_node->Items = malloc(n * itemSize);
    if (new_node->Items == NULL)
      exit(0);

    if (tail != NULL)
      tail->next = new_node;
    else
      fifo->head = new_node;
    tail = new_node;
    back = 0;
    fifo->tail = tail;
  }

  memcpy(tail->Items + back * itemSize, item, itemSize);
  back++;

  fifo->back = back;
}

void dequeue(Fifo *fifo, void *dest) {
  struct node *old;

  if (fifo == NULL || isFifoEmpty(fifo))
    exit(0);

  memcpy(dest, fifo->head->Items + (fifo->front) * (fifo->itemSize),
         fifo->itemSize);
  fifo->front++;
  if (fifo->front == fifo->head->n) {
    if (fifo->head->next == NULL) {
      fifo->tail = NULL;
      fifo->back = 0;
    }
    old = fifo->head;
    fifo->head = fifo->head->next;
    free(old->Items);
    free(old);
    fifo->front = 0;
    fifo->tables--;
  }
}
