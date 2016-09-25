/*
 * u is null
 * subroot of the rotated subtree
 */
#include <assert.h>
#include <stdlib.h>

#define RB_RED 0
#define RB_BLACK 1

struct rb_node {
	char color;
	struct rb_node *parent, *left, *right;
};

struct rb_tree {
	struct rb_node *root;
};

struct rb_tree *rb_tree_new(void);
void rb_node_init(struct rb_node *);
void rb_insertion_balance(struct rb_node *, struct rb_tree *);
void rb_general_find(const void *key, const struct rb_tree *tree,
	int (*)(const void *, const void *),
	struct rb_node **result, int *delta);

static void rb_rotate_right(struct rb_node *, struct rb_tree *);
static void rb_rotate_left(struct rb_node *, struct rb_tree *);

struct rb_tree *rb_tree_new(void)
{
	struct rb_tree *tree;

	assert(tree = malloc(sizeof(struct rb_tree)));
	tree->root = NULL;

	return tree;
}

void rb_node_init(struct rb_node *node)
{
	node->color = RB_RED;
	node->parent = node->left = node->right = NULL;
}

void rb_insertion_balance(struct rb_node *x, struct rb_tree *tree)
{
	struct rb_node *g, *p, *u, *t;

	while ((p = x->parent) && p->color == RB_RED) {
		g = p->parent;
		if (p == g->left) {
			u = g->right;
			/* be careful */
			if (u && u->color == RB_RED) {
				g->color = RB_RED;
				p->color = RB_BLACK;
				u->color = RB_BLACK;
				x = g;
				continue;
			}
			if (x == p->right) {
				rb_rotate_left(p, tree);
				t = x;
				x = p;
				p = t;
			}
			rb_rotate_right(g, tree);
			g->color = RB_RED;
			p->color = RB_BLACK;
		} else {
			u = g->left;
			if (u && u->color == RB_RED) {
				g->color = RB_RED;
				p->color = RB_BLACK;
				u->color = RB_BLACK;
				x = g;
				continue;
			}
			if (x == p->left) {
				rb_rotate_right(p, tree);
				t = x;
				x = p;
				p = t;
			}
			rb_rotate_left(g, tree);
			g->color = RB_RED;
			p->color = RB_BLACK;
		}
	}

	tree->root->color = RB_BLACK;
}

static void rb_rotate_right(struct rb_node *x, struct rb_tree *tree)
{
	struct rb_node *y, *p;

	if (!x || !x->left)
		return;

	y = x->left;
	x->left = y->right;
	y->right = x;
	if (x->left)
		x->left->parent = x;

	p = x->parent;
	x->parent = y;
	y->parent = p;
	/* be careful */
	if (!p)
		tree->root = y;
	else if (p->left == x)
		p->left = y;
	else
		p->right = y;
}

static void rb_rotate_left(struct rb_node *x, struct rb_tree *tree)
{
	struct rb_node *y, *p;

	if (!x || !x->right)
		return;

	y = x->right;
	x->right = y->left; 
	y->left = x;
	if (x->right)
		x->right->parent = x;

	p = x->parent;
	x->parent = y;
	y->parent = p;
	if (!p)
		tree->root = y;
	else if (p->right == x)
		p->right = y;
	else
		p->left = y;
}

/*
 * Find *key in *tree.
 *
 * The two parameters of compare() are *key and a <struct rb_node>,
 * *key could be everything, so it is decided by the user that
 * how to compare *key with <struct rb_node>.
 *
 * - **result being null means key is not in tree, deltap is nonesense.
 * - **result not being null:
 * 	- key is found if *deltap is zero
 *	- key is not found and should be a child of **result
 *		- left child if *deltap < 0
 *		- right child if *deltap > 0
 */ 
void rb_general_find(const void *key, const struct rb_tree *tree, 
	int (*compare)(const void *, const void *),
	struct rb_node **result, int *deltap)
{
	int delta = 0;

	struct rb_node *p, *x;

	p = NULL;
	x = tree->root;
	while (x) {
		p = x;
		delta = compare(key, x);
		if (delta < 0)
			x = x->left;
		else if (delta > 0)
			x = x->right;
		else
			break;
	}

	*deltap = delta;
	*result = p;
}

