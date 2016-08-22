#ifndef _TJYTLXWXHYZQFW_LIST_H
#define _TJYTLXWXHYZQFW_LIST_H

#include <assert.h>
#include <stdlib.h>

#include "address.h"

struct knot {
	struct knot *next, *prev;
};

struct knot *knot_new() {
	struct knot *list;

	assert(list = malloc(sizeof(struct knot)));
	list->next = list->prev = NULL;
	
	return list;
}

void knot_tie(struct knot *new, struct knot *prev)
{
	struct knot *next = prev->next;

	prev->next = new;
	new->next = next;

	if (next)
		next->prev = new;
	new->prev = prev;
}

/*
 * old name: list_unlink
 */
void knot_untie(struct knot *del, struct knot *list)
{
	struct knot *prev = del->prev, *next = del->next;

	if (prev)
		prev->next = next;
	if (next)
		next->prev = prev;
}

#endif

