#ifndef _TJYTLXWXHYZQFW_LIST_C
#define _TJYTLXWXHYZQFW_LIST_C

/*
 * need: address.h
 */

#include <assert.h>
#include <stdlib.h>

struct list_node {
	struct list_node *next, *prev;
};

/*
 *    __ node___
 *   /  ______  \
 *  |  /      \  |
 * prev        next
 *     \______/ 
 */

#define list_link(x, p) \
	do {\
		(x)->next = (p)->next;\
		(x)->prev = (p);\
		(x)->next->prev = (x);\
		(x)->prev->next = (x);\
	}while(0)

/*
 *    ______  ______ 
 *   /      \/      \
 * prev    node    next
 *   \______/\______/
 *
 */
struct list_node *list_unlink(struct list_node *node)
{
	if (node->next == node) {
		assert(node->prev == node);
		return node;
	}

	node->prev->next = node->next;
	node->next->prev = node->prev;

	return node;
}

#define list_head_init(h) ((h)->next = (h)->prev = (h))
#define list_empty(li) ((li)->next == (li) ? 1 : 0)
#define list_append(x, li) list_link((x), (li)->prev)
#define list_push(x, li) list_link((x), (li))
#define list_pop(li) list_unlink((li)->next)

#define list_for_each(p, l) \
	for((p) = (l)->next; (p) != (l); (p) = (p)->next)

#endif

