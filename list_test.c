#include "address.h"
#include "debug.c"
#include "list.c"

struct node {
	struct list_node ln;
	char key[128];
};

#define cont(x) container_of((x), struct node, ln)

struct node nodes[1024];

void print_list(struct list *list)
{
	struct list_node *node;

	node = list->first;
	//printis(0, 0, "first: %s\n", cont(node)->key);
	do {
		printis(0, 0, "%s, ", cont(node)->key);
		node = node->next;
	} while (node != list->first);

	printis(0, 0, "\n");
}

int main(void)
{
	int i = -1;
	struct list *list;
	struct list_node *node;

	list = list_new();
	while (scanf("%s", nodes[++i].key) == 1) {
		list_node_circle(&nodes[i].ln);
		list_push(&nodes[i].ln, list);
		print_list(list);
	}

	while ((node = list_pop(list)) != NULL)
		printis(1, 1, "pop: %s\n", cont(node)->key);

	return 0;
}
