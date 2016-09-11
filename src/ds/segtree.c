/*
 * Segment Tree
 *
 * @author wcc
 */

#include "../../segtree.h"

#define SEG_MIN(x, y) ((x) < (y) ? (x) : (y))
#define SEG_MAX(x, y) ((x) > (y) ? (x) : (y))

#define SEG_LAZY_UPD(r, d, t)\
	do {\
		(r)->flag |= SEG_F_UPD;\
		(t)->remember((r), (d));\
		(t)->alter((r), (d));\
	}while(0)

#define SEG_UPDATE_DOWN(r, t) \
	do {\
		SEG_LAZY_UPD((r)->left, (r)->history, (t));\
		SEG_LAZY_UPD((r)->right, (r)->history, (t));\
		(r)->flag &= (~SEG_F_UPD);\
		(t)->forget((r));\
	}while (0);

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
	node = tree->nodes + tree->current++;

	node->begin = begin;
	node->end = end;
	node->flag = 0UL;
	node->left = node->right = NULL;
	memset(&node->data, 0, sizeof(seg_data_t));
	memset(&node->history, 0, sizeof(seg_data_t));

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

void seg_do_query(int begin, int end, struct seg_node *root, struct seg_tree *tree, seg_data_t *result)
{
	if (end < begin)
		return;

	if (root->begin == begin && root->end == end) {
		tree->reduce(root, result);
		return;
	}

	if (root->flag & SEG_F_UPD)
		SEG_UPDATE_DOWN(root, tree);
		
	seg_do_query(begin, SEG_MIN(root->left->end, end), root->left, tree, result);
	seg_do_query(SEG_MAX(root->right->begin, begin), end, root->right, tree, result);
}

void seg_do_alter(int begin, int end, seg_data_t data, struct seg_node *root, struct seg_tree *tree)
{
	if (end < begin)
		return;

	if (root->begin == begin && root->end == end) {
		SEG_LAZY_UPD(root, data, tree);
		return;
	}

	if (root->flag & SEG_F_UPD)
		SEG_UPDATE_DOWN(root, tree);

	seg_do_alter(begin, SEG_MIN(root->left->end, end), data, root->left, tree);
	seg_do_alter(SEG_MAX(root->right->begin, begin), end, data, root->right, tree);

	tree->update(root);
}

#ifdef DEBUG

void printnode(struct seg_node *n)
{
	print("I(i, i): s: i, s: i, s: i", _I, n->begin, n->end, "data", n->data, "history", n->history, "upd", n->flag);
}

#endif

