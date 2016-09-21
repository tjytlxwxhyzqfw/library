
#include <assert.h>
#include <stdlib.h>
#include "../../queue.h"

struct queue *queue_alloc(int cap)
{
	struct queue *q;

	assert(q = malloc(sizeof(struct queue)));
	q->cap = ++cap;
	q->first = q->last = 0;
	assert(q->cell = malloc(sizeof(void *) * q->cap));

	return q;
}

void queue_append(void *x, struct queue *q)
{
	int buble = (q->last + 1) % q->cap;

	/* 邻满 */
	if (buble == q->first)
		return;
	q->cell[q->last] = x;
	q->last = buble;
}

void *queue_pop(struct queue *q)
{
	void *ret = NULL;

	if (q->first != q->last) {
		ret = q->cell[q->first];
		q->first = (q->first + 1) % q->cap;
	}

	return ret;
}

