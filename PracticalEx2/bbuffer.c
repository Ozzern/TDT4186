#include <stdio.h>
#include <stdlib.h>

#include "sem.h"


/*
 * Bounded Buffer implementation to manage int values that supports multiple 
 * readers and writers.
 *
 * The bbuffer module uses the sem module API to synchronize concurrent access 
 * of readers and writers to the bounded buffer.
 */

/* Opaque type of a Bounded Buffer.
 * ...you need to figure out the contents of struct BNDBUF yourself
 */

typedef struct BNDBUF{
    int* buffer_ptr;
    unsigned int size;
    unsigned int head;
    unsigned int tail;
    SEM *full;
    SEM *empty;
    SEM *busy;
} BNDBUF;

/* Creates a new Bounded Buffer. 
 *
 * This function creates a new bounded buffer and all the helper data 
 * structures required by the buffer, including semaphores for 
 * synchronization. If an error occurs during the initialization the 
 * implementation shall free all resources already allocated by then.
 *
 * Parameters:
 *
 * size     The number of integers that can be stored in the bounded buffer.
 *
 * Returns:
 *
 * handle for the created bounded buffer, or NULL if an error occured.
 */

BNDBUF *bb_init(unsigned int size) {
    BNDBUF *ring_buffer = malloc(sizeof(BNDBUF));
    ring_buffer->size = size;
    ring_buffer->head = 0;
    ring_buffer->tail = 0;
    ring_buffer->empty = sem_init(0);
    ring_buffer->busy = sem_init(1);
    ring_buffer->full = sem_init(size);
    int *buffer = malloc(sizeof(unsigned int) * size);
    ring_buffer->buffer_ptr = buffer;
    if(ring_buffer)
        return ring_buffer;
    return NULL;
}

/* Destroys a Bounded Buffer. 
 *
 * All resources associated with the bounded buffer are released.
 *
 * Parameters:
 *
 * bb       Handle of the bounded buffer that shall be freed.
 */

void bb_del(BNDBUF *bb) {
    free(bb);
}

/* Retrieve an element from the bounded buffer.
 *
 * This function removes an element from the bounded buffer. If the bounded 
 * buffer is empty, the function blocks until an element is added to the 
 * buffer.
 *
 * Parameters:
 *
 * bb         Handle of the bounded buffer.
 *
 * Returns:
 *
 * the int element
 */

int bb_get(BNDBUF *bb) {
    P(bb->busy);
    P(bb->empty);
    int element = bb->buffer_ptr[bb->tail];
    // printf("GETTING ELEMENT FROM BUFFER: %i\n", element);
    // printf("Old tail at %i\n", bb->tail);
    // bb->tail = bb->tail + 1;
    // if (bb->tail == bb->size)
    //     bb->tail = 0;
    bb->tail = (bb->tail + 1) % bb->size;
    // printf("New tail at %i\n\n", bb->tail);
    V(bb->full);
    V(bb->busy);
    return element;
}

/* Add an element to the bounded buffer. 
 *
 * This function adds an element to the bounded buffer. If the bounded 
 * buffer is full, the function blocks until an element is removed from 
 * the buffer.
 *
 * Parameters:
 *
 * bb     Handle of the bounded buffer.
 * fd     Value that shall be added to the buffer.
 *
 * Returns:
 *
 * the int element
 */

void bb_add(BNDBUF *bb, int fd) {
    P(bb->full);
    // printf("SIZE %i\n", bb->size);
    bb->buffer_ptr[bb->head] = fd;
    // printf("ADDING ELEMENT TO BUFFER: %i\n", bb->buffer_ptr[bb->head]);
    // printf("Old head at %i\n", bb->head);
    bb->head = (bb->head + 1) % bb->size;
    // printf("New head at %i\n\n", bb->head);
    V(bb->empty);
}
