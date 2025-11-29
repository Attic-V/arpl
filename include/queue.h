#ifndef QUEUE_H
#define QUEUE_H

struct queue_queue;

struct queue_queue *queue_create (void);
void queue_destroy (struct queue_queue *queue);
void queue_enqueue (struct queue_queue *queue, void *data);
void *queue_dequeue (struct queue_queue *queue);
bool queue_isEmpty (struct queue_queue *queue);

#endif

