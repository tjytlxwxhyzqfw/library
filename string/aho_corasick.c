/*
 * Aho-Corasick Automathine
 *
 * need:
 * 	queue.c
 * 	trie.c
 */

#include "../arrayqueue.c" //TODO: use listqueue
#include "trie.c"

/*
 * Fill a trie with failed pointers.
 *
 * We attach a string to each node in trie.
 * That string is the path from root to the node;
 *
 * Thus, if the failed pointer of N1 points to N2,
 * then N2 is the longest suffix of N1; 
 *
 * N1: #######
 *           | <--- failed pointer
 *           V
 * N2:   #####
 *
 * Once a mismatch occured, we follow failed pointers
 * until a suffix is met. 
 *
 * Since the node which is currnetly mismatched does't have a failed
 * pointer, we (have to) check its parent and terminate at a suffix of
 * node->parent that is followd by node->key.
 *
 * Once we find a suffix, it must be the longgest suffix in the trie
 * (prove it).
 *
 * Here are some useful properties: 
 * 1. N is failed pointer always points to a node with key == N->key.
 * 2. If ch mismatches and return back to trie, then *trie must not have
 * 	an arc to ch, because if there were, ch should have been returned
 * 	to the child of *trie;
 */
void aho_corasick_fill(struct trie_node *trie)
{
	struct trie_node *node = trie;

	struct trie_node *hope, *parent, *another;
	struct queue *q;
	int i;

	//TODO: use listqueue
	q = queue_new(1000000);
	
	trie->parent = trie; /*important*/
	queue_append(node, q);

	while ((node = queue_pop(q)) != NULL) {
		/*
		 * We compute failed pointer for every node,
		 * although some of them is never used.
		 *
		 */
		for (parent = node->parent; parent != trie; parent = another) {
			another = parent->failed;
			if (another->nexts[TRIE_IDX(node->key)])
				break;
		}

		node->failed = (parent != trie ? another->nexts[TRIE_IDX(node->key)] : trie);

		if (node->end)
			queue_append(node, node->queue);
		if (node->failed != trie)
			for (i = node->failed->queue->first;
				i != node->failed->queue->last;
				i = (i + 1) % node->failed->queue->cap)
				queue_append(node->failed->queue->cell[i], node->queue);

		for (i = 0; i < TRIE_ASZ; ++i)
			if (node->nexts[i])
				queue_append(node->nexts[i], q);
	}

	queue_destroy(q);
}

void aho_corasick_simple(const char *s, const struct trie_node *trie,
	int (*onmatch)(const struct trie_node *))
{
	const struct trie_node *node = trie;
	int i, len;
	int chid, failch;

	len = strlen(s);
	for (i = 0; i < len; ++i) {
		chid = TRIE_IDX(s[i]);
		while (node->nexts[chid] == NULL && node != trie)
			node = node->failed;
		if (node->nexts[chid]) {
			node = node->nexts[chid];
			if (node->queue->first != node->queue->last && onmatch(node))
				return;
		}
	}
}
