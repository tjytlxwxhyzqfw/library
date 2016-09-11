/**
 * Segment Tree
 *
 * @author wcc
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.c"
#include "segtree.h"

void alter(struct seg_node *node, seg_data_t data);
void update(struct seg_node *node);
void reduce(struct seg_node *node, seg_data_t *res);
void forget(struct seg_node *node);
void remember(struct seg_node *node, seg_data_t data);

int main(void)
{
	struct seg_tree *tree;
	int cmd;
	int i, j;
	seg_data_t x, qres;

	tree = seg_tree_alloc(4 * 11);
	tree->alter = alter;
	tree->update = update;
	tree->reduce = reduce;
	tree->forget = forget;
	tree->remember = remember;

	seg_build(0, 10, tree);
	seg_print(tree);

	while (scanf("%d", &cmd) == 1) {
		switch (cmd) {
			case 1: 
				scanf("%d%d%d", &i, &j, &x);
				seg_alter(i, j, x, tree);
				break;
			case 2:
				scanf("%d%d", &i, &j);
				qres = 0;
				seg_query(i, j, tree, &qres);
				print("s: i", "qres", qres);
				break;
			default:
				assert(0);
		}
		seg_print(tree);
	}

	return 0;
}



void update(struct seg_node *node)
{
	node->data = node->left->data + node->right->data;
}

void alter(struct seg_node *node, seg_data_t data)
{
	node->data += (node->end - node->begin + 1) * data;
}

void reduce(struct seg_node *node, seg_data_t *res)
{
	print("s: (i, i) i ---> i", "reduce", node->begin, node->end, node->data, *res + node->data);
	*res += node->data;
}

void forget(struct seg_node *node)
{
	node->history = 0;
}

void remember(struct seg_node *node, seg_data_t data)
{
	node->history += data;
}
