#include "internal.h"

#include <stdlib.h>

struct queue_queue *queue_create (void)
{
	struct queue_queue *queue = malloc(sizeof(struct queue_queue));

	queue->head.next = &queue->head;
	queue->head.prev = &queue->head;

	return queue;
}

void queue_destroy (struct queue_queue *queue)
{
	free(queue);
}

void queue_enqueue (struct queue_queue *queue, void *data)
{
	struct node *node = malloc(sizeof(struct node));
	node->data = data;

	node->next = &queue->head;
	node->prev = queue->head.prev;
	node->next->prev = node;
	node->prev->next = node;
}

void *queue_dequeue (struct queue_queue *queue)
{
	if (queue_isEmpty(queue)) return NULL;

	struct node *node = queue->head.next;
	node->next->prev = &queue->head;
	queue->head.next = node->next;

	void *data = node->data;
	free(node);

	return data;
}

bool queue_isEmpty (struct queue_queue *queue)
{
	return queue->head.next == &queue->head;
}

