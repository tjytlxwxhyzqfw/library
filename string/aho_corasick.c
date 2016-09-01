/*
 * Aho-Corasick Automathine
 *
 * need:
 * 	queue.c
 * 	trie.c
 */

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

	struct trie_node *parent, *another;
	struct clist *queue;
	struct clist_node *pos;
	int i;

	queue = clist_alloc();

	trie->parent = trie; /*important*/
	clist_append(trie, queue);

	while ((node = clist_pop(queue)) != NULL) {
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
			clist_append(node, node->ends);

		if (node->failed != trie)
			clist_for_each(pos, node->failed->ends)
				clist_append(pos->data, node->ends);

		for (i = 0; i < TRIE_ASZ; ++i)
			if (node->nexts[i])
				clist_append(node->nexts[i], queue);
	}
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
			if (!clist_empty(node->ends) && onmatch(node))
				return;
		}
	}
}
