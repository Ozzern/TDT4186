#include <sem.h>


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
    int* buffer;
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
    BNDBUF *ring_buffer = check_malloc(sizeof(BNDBUF));
    ring_buffer->size = size;
    ring_buffer->head = 0;
    ring_buffer->tail = 0;
    ring_buffer->empty = sem_init(0);
    ring_buffer->busy = sem_init(1);
    ring_buffer->full = sem_init(size);
    int* buffer[size];
    ring_buffer->buffer = buffer;
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
    free(bb->buffer);
    free(bb->empty);
    free(bb->full);
    free(bb->head);
    free(bb->tail);
    free(bb->size);
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

int  bb_get(BNDBUF *bb) {
    P(bb->busy);
    int element = bb->buffer[bb->tail];
    bb->tail = (bb->tail + 1) % bb->size;
    V(bb->full);
    P(bb->empty);
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
    P(bb->busy);
    bb->buffer[bb->head] = fd;
    bb->head = (bb->head + 1) % bb->size;
    V(bb->empty);
    P(bb->full);
    V(bb->busy);
}
