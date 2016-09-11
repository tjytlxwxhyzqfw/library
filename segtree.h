/*
 * Segment Tree
 *
 * @author wcc
 *
 * A little bit slow, why ?
 *
 * Each 'struct seg_node' occupies about 48 bytes (sizeof(seg_data) == 8),
 * therefore **46,875KB** are needed for **1,000,000** tree nodes.
 *
 * Modifiy data (e.g. by defining it with a structure) to adapt multiple alternations.
 * But be carefule of their relations.
 *
 * Interface
 *
 *	seg_tree_alloc()
 *	seg_tree_free() 	
 *	seg_build() - build a segment tree on segment [begin, end]
 * 	seg_query() - query information on a specified segment
 * 	seg_alter() - alter information on a specified segment
 * 	seg_traverse() - traverse
 * 	<*> seg_print() - print a segment tree
 *
 * 	Note: function with prefix <*> denotes that '#define DEBUG' is required.
 *
 * Worker
 *
 *	seg_node_alloc()
 * 	seg_do_build()
 * 	seg_do_alter()
 *	seg_do_query()
 *
 * Usage
 *
 * 	There are 5 customized functions:
 *
 * 		seg_remember_t - accumulate all modifications during lazy-updating
 * 		seg_forget_t - forget modifications after updating-down
 * 		seg_update_t - update a parent from its two children
 * 		seg_alter_t - change information of every point in a specified segment
 * 		seg_redude_t - reduce all sub segments and get a value for a single specified segment
 *
 * 	Note that seg_remember_t are highly relative to seg_alter_t, and seg_forget_t, seg_update_t and
 * 	seg_reduce_t are quite independent by comparison.
 *
 * Example
 * 	segtree_test.c show a simple segtree with region-add and region-sumary
 * 	1166.c uses point-update and region-query
 * 	1556.c uses region-update and point-query
 *	@see segtree_test.c
 *	@see 1166.c
 *	@see 1556.c
 *
 */

#ifndef __WCC_SEGTREE_H
#define __WCC_SEGTREE_H

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "debug.c"

#ifndef seg_data_t
#define seg_data_t int
#endif

struct seg_node;
struct seg_tree;

typedef void (*seg_forget_t)(struct seg_node *);
typedef void (*seg_remember_t)(struct seg_node *, seg_data_t);
typedef void (*seg_update_t)(struct seg_node *);
typedef void (*seg_alter_t)(struct seg_node *, seg_data_t);
typedef void (*seg_reduce_t)(struct seg_node*, seg_data_t *);

/**
 * left, right: children
 * begin, end: [begin, end]
 * flag
 * data - please make it **small**
 * history -
 */
struct seg_node {
	struct seg_node *left, *right;
	int begin, end;
	unsigned int flag;
	seg_data_t data;
	seg_data_t history;
};

struct seg_tree {
	struct seg_node *root;
	struct seg_node *nodes;
	int nnodes, current;

	seg_update_t update;
	seg_alter_t alter;
	seg_reduce_t reduce;
	seg_forget_t forget;
	seg_remember_t remember;
};

#define SEG_F_UPD (0x1UL)
#define SEG_COVER(b, e, n) ((b) >= (n)->begin && (e) <= (n)->end)

/* Memory relative */
struct seg_tree *seg_tree_alloc(int nnodes);
struct seg_node *seg_node_alloc(int begin, int end, struct seg_tree *tree);
void seg_tree_free(struct seg_tree *tree);

/* Recursive worker functions */
void seg_do_build(struct seg_node *root, struct seg_tree *tree);
void seg_do_alter(int begin, int end, seg_data_t data, struct seg_node *node, struct seg_tree *tree);
void seg_do_query(int begin, int end, struct seg_node *root, struct seg_tree *tree, seg_data_t *result);

/* Main Interface */
void seg_build(int begin, int end, struct seg_tree *tree);
#define seg_alter(b, e, d, t) seg_do_alter((b), (e), (d), (t)->root, (t))
#define seg_query(b, e, t, r) do {assert(SEG_COVER((b), (e), (t)->root));seg_do_query((b), (e), (t)->root, (t), (r));}while (0)
void seg_traverse(void (*f)(struct seg_node *), struct seg_node *node);

#ifdef DEBUG

/* debug tools */
void printnode(struct seg_node *n);
#define seg_print(t) do {_I = 0; seg_traverse(printnode, (t)->root);} while(0)

#endif

#endif

