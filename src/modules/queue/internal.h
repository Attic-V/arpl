#ifndef QUEUE_INTERNAL_H
#define QUEUE_INTERNAL_H

#include "queue.h"

struct node {
	void *data;
	struct node *next;
	struct node *prev;
};

struct queue_queue {
	struct node head;
};

#endif

