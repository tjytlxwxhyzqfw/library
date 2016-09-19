/*
 * Segment Tree
 *
 * @author wcc
 */

#include "../../segtree.h"

/* interface */

struct seg_tree *seg_tree_alloc(int nnodes)
{
	struct seg_tree *tree;

	tree = malloc(sizeof(struct seg_tree));
	assert(tree);

	tree->root = NULL;
	tree->current = 0;
	tree->nnodes = nnodes;
	tree->nodes = malloc(sizeof(struct seg_node) * nnodes);

	return tree;
}

void seg_tree_free(struct seg_tree *tree)
{
	if (!tree)
		return;
	if (tree->nodes)
		free(tree->nodes);
	free(tree);	
}

void seg_build(int begin, int end, struct seg_tree *tree)
{
	tree->current = 0;
	tree->root = seg_node_alloc(begin ,end, tree);
	seg_do_build(tree->root, tree);
}

void seg_traverse(void (*f)(struct seg_node *), struct seg_node *node)
{
	#ifdef DEBUG
	++_I;
	#endif

	f(node);
	if (node->left) {
		assert(node->right);
		seg_traverse(f, node->left);
		seg_traverse(f, node->right);
	}

	#ifdef DEBUG
	--_I;
	#endif
}

/* worker */

struct seg_node *seg_node_alloc(int begin, int end, struct seg_tree *tree)
{
	struct seg_node *node;

	assert(tree->current < tree->nnodes);
	node = tree->nodes + tree->current;

	node->index = tree->current;
	node->begin = begin;
	node->end = end;
	node->flag = 0UL;
	node->left = node->right = NULL;

	++tree->current;
	return node;
}

void seg_do_build(struct seg_node *root, struct seg_tree *tree)
{
	if (root->begin >= root->end)
		return;
	root->left = seg_node_alloc(root->begin ,(root->begin + root->end) / 2, tree);
	root->right = seg_node_alloc(root->left->end + 1, root->end, tree);
	seg_do_build(root->left, tree);
	seg_do_build(root->right, tree);
}

