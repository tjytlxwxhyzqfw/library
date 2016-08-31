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

void list_node_circle(struct list_node *node)
{
	node->next = node->prev =node;
}

/*
 *    __ node___
 *   /  _____   \
 *  |  /      \ |
 * prev        next
 *     \______/ 
 */
void list_link(struct list_node *node, struct list_node *prev)
{
	node->next = prev->next;
	node->prev = prev;

	node->next->prev = node;
	node->prev->next = node;
}


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

#ifndef LIST_NO_HEAD

struct list {
	struct list_node *first;
};

struct list *list_new(void)
{
	struct list *list;

	list = malloc(sizeof(struct list));
	list->first = NULL;

	return list;
}

void list_head_destroy(struct list *head)
{
	if (head)
		free(head);
}

void list_append(struct list_node *node, struct list *list)
{
	if (list->first == NULL) {
		list->first = node;
		list_node_circle(node);
		return;
	}
	list_link(node, list->first->prev);
}

void list_push(struct list_node *node, struct list *list)
{
	list_append(node, list);
	list->first = node;
}

struct list_node *list_pop(struct list *list)
{
	struct list_node *target;

	target = list->first;
	if (target == NULL)
		return NULL;
	list->first = (target->next == target ? NULL : target->next);
	return list_unlink(target);
}

#endif

#endif

