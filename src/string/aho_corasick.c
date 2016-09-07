/*
 * Aho-Corasick Automathine
 *
 * need:
 * 	clist.c
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
 * 1. Failed pointer of N always points to a node with key == N->key.
 * 2. If ch in trie mismatches and return back to trie root, then *trie must not have
 * 	an arc to ch, because if there were, ch should have been returned
 * 	to the child of *trie;
 * 3. Failed pointers link up all strings and sub-strings in trie that end up with
 * 	a specific character.
 */
void aho_corasick_fill(struct trie_node *trie)
{
	struct trie_node *node = trie;

	struct trie_node *parent, *another;
	struct clist *queue;
	int i;

	queue = clist_alloc();

	trie->parent = trie; /*important*/
	clist_append(trie, queue);

	while ((node = clist_pop(queue)) != NULL) {
		/*
		 * We compute failed pointer for every node,
		 * although some of them is never used.
		 *
		 * TODO: simplify
		 */
		for (parent = node->parent; parent != trie; parent = another) {
			another = parent->failed;
			if (another->nexts[TRIE_IDX(node->key)])
				break;
		}
		node->failed = (parent != trie ? another->nexts[TRIE_IDX(node->key)] : trie);

		/*
		 * A pretty independent part for matching counts
		 */
		if (node->end)
			++node->ends;
		if (node->failed != trie)
			node->ends += node->failed->ends;

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
			if (node->ends && onmatch(node))
				return;
		}
	}
}
