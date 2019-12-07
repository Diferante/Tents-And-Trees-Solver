#ifndef GENERALFIFO_H_INCLUDED
#define GENERALFIFO_H_INCLUDED

typedef struct _fifo Fifo;

Fifo* initFifo(unsigned int initial_size, unsigned int item_size);
void freeFifo(Fifo*);
int isFifoEmpty(Fifo*);
void queue( Fifo*, void* item);
void dequeue(Fifo*, void* dest);


#endif // GENERALFIFO_H_INCLUDED
