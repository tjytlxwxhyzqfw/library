/*
 * support:
 * 	append/push
 * 	pop
 * 	clist_for_each
 *
 * doesn't support:
 * 	find
 * 	insert
 * 	remove
 */

#include <assert.h>
#include <stdlib.h>

struct clist_node {
	struct clist_node *prev, *next;
	void *data;
};

struct clist_node *clist_node_alloc(void *data)
{
	struct clist_node *node;

	node = malloc(sizeof(struct clist_node));
	assert(node);
	node->next = node->prev = node;
	node->data = data;

	return node;
}

struct clist {
	struct clist_node *first;
	struct clist_node *iterator;
	int size;
};

struct clist *clist_alloc(void)
{
	struct clist *clist;

	clist = malloc(sizeof(struct clist));
	assert(clist);
	clist->first = NULL;
	clist->iterator = NULL;
	clist->size = 0;

	return clist;
}

void clist_append(void *data, struct clist *clist)
{
	struct clist_node *node = clist_node_alloc(data);

	++clist->size;
	
	if (clist->first == NULL) {
		clist->first = node;
		/*
		 * To simply clist_for_each(),
		 * ->next of the last <clist_node> points to NULL.
		 */
		node->next = NULL;
		return;
	}

	node->next = NULL;
	node->prev = clist->first->prev;
	node->prev->next = node;
	clist->first->prev = node;
}

void clist_push(void *data, struct clist *clist)
{
	struct clist_node *node;

	++clist->size;

	node = clist_node_alloc(data);
	if (clist->first == NULL) {
		clist->first = node;
		node->next = NULL;
		return;
	}

	node->next = clist->first;
	node->prev = clist->first->prev;

	node->next->prev = node;
	if (node->prev->next)
		node->prev->next = node;

	clist->first = node;
}

void *clist_pop(struct clist *clist)
{
	struct clist_node *target;
	void *data;

	if (clist->first == NULL)
		return NULL;

	--clist->size;

	target = clist->first;
	clist->first = target->next;

	/*
	 * *target is leaving,
	 * all he should do is:
	 * 	1. tell his ->prev who is his ->next
	 * 	2. tell his ->next who is his ->prev
	 * since target->next always points to NULL,
	 * target can only do #1 thing;
	 */
	if (target->next)
		target->next->prev = target->prev;

	data = target->data;
	free(target);

	return data;
}

#define clist_empty(li) ((li)->first == NULL ? 1 : 0)
#define clist_for_each(li) for((li)->iterator = (li)->first; (li)->iterator; (li)->iterator = (li)->iterator->next)
#define clist_data(li, t) ((t)((li)->iterator->data))
