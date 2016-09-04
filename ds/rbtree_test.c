#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "address.h"
#include "debug.c"
#include "rbtree.c"

struct node {
	char *key;
	struct rb_node rbnode;
};

#define cont(a) container_of(a,struct node,rbnode)

struct node *node_new(char *key)
{
	struct node *node;

	assert(node = malloc(sizeof(struct node)));
	assert(node->key = malloc(sizeof(char) * (strlen(key) + 1)));

	strcpy(node->key, key);
	rb_node_init(&node->rbnode);

	return node;
}

int node_compare(const void *key, const void *rbnode)
{
	return strcmp((char *)key, cont((struct rb_node *)rbnode)->key);
}

struct node *find(const char *key, const struct rb_tree *tree)
{
	struct rb_node *x;
	int delta;

	rb_general_find(key, tree, node_compare, &x, &delta);

	if (x && !delta)
		return cont(x);
	return NULL;
}

void insert(struct node *node, struct rb_tree *tree)
{
	struct rb_node *p;
	int delta;

	rb_general_find(node->key, tree, node_compare, &p, &delta);

	if (!p)
		tree->root = &node->rbnode;
	else if (delta < 0)
		p->left = &node->rbnode;
	else if (delta > 0)
		p->right = &node->rbnode;
	else
		return;

	node->rbnode.parent = p;
	rb_insertion_balance(&node->rbnode, tree);
}

struct rb_tree *tree;

void print(int deepth, struct rb_node *rbn)
{
	if (rbn == NULL) {
		printis(deepth, 0, "(null)\n");
		return;
	}
	printis(deepth, 0, "%s:%s\n", cont(rbn)->key, rbn->color == RB_RED ? "red" : "black");
	print(deepth + 1, rbn->left);
	print(deepth + 1, rbn->right);
}

int main(void)
{
	char inbuf[10];

	setbuf(stdout, NULL);

	tree = rb_tree_new();
	while (1) {
		scanf("%s", inbuf);	
		printis(0, 0, "inbuf: %s\n", inbuf);
		if (find(inbuf, tree)) {
			printis(1, 0, "hit\n");
		} else {
			printis(1, 0, "miss\n");
			insert(node_new(inbuf), tree);
		}
		print(0, tree->root);
	}

	return 0;
}
