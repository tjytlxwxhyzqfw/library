#include "address.h"
#include "debug.c"
#include "list.c"

struct node {
	struct list_node ln;
	char key[128];
};

#define cont(x) container_of((x), struct node, ln)

struct node nodes[1024];

void print_list(struct list_node *head)
{
	struct list_node *node;

	list_for_each(node, head)
		printis(0, 0, "%s, ", cont(node)->key);

	printis(0, 0, "\n");
}

int main(void)
{
	int i = -1;
	struct list_node *head;
	struct list_node *node;

	list_head_init(head);
	while (scanf("%s", nodes[++i].key) == 1) {
		list_push(&nodes[i].ln, head);
		print_list(head);
	}

	while ((node = list_pop(head)) != head)
		printis(1, 1, "pop: %s\n", cont(node)->key);

	return 0;
}
