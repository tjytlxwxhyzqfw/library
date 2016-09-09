/*
 * Aho-Corasick Automathine
 *
 * @author wcc
 * @see trie.c
 *
 * require: clist.c, trie.c
 * last modified: 2016-09-09 15:01
 *
 * To turn a trie to an AC-Automachine, we attach a string to each
 * node in trie, that string is the path from root to the node.
 * Thus, if the failed pointer of N1 points to N2 then the string of N2
 * is the longest suffix of the string of N1:
 *
 * N1:R#######
 *           | <--- failed pointer
 *           V
 * N2:  R#####
 * 		('R' means 'root')
 *
 * Once a mismatch occured in a filled trie, we follow failed pointers
 * until another node is met. This procedure looks just like that
 * we throw away (as few as we can) some prefix of the current string
 * (of the current node) to be same with a suffix of the input string
 * since the initial string is not a suffix.
 *
 * Since the node currnetly mismatched does't have a failed pointer during
 * aho_corasick_fill() procedure, we (have to) check and flow the failed pointer
 * of its parent until another occourence of the mismatched node is found (as a
 * child of the node linked by the initial parent's failed pointer).
 *
 * Once we have found a node, the string of the newly-founded node must be the
 * longgest suffix of the string of the mismatched node in the trie(prove it).
 *
 * Here are some useful properties: 
 * 1. Failed pointer of N always points to a node with key == N->key.->key is a letter.
 * 2. If ch in trie mismatches and return back to trie root, then *trie must not have
 * 	an arc to ch, because if there were, ch should have been returned
 * 	to the child of *trie;
 * 3. Failed pointers link up all strings and sub-strings in trie that end up with
 * 	a specific character.
 */

/*
 * Fill a trie with failed pointers and matching counts.
 *
 * @param trie - A trie with two fields added in its node:
 * 	->ends and ->failed. ->ends denotes the number of
 * 	end nodes from current node to the last node linked
 * 	by the failed pointers.
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

/*
 * Simple aho-corasick algorithm
 *
 * @param s - a long string(or text) to be analized
 * @para trie - trie of key words
 * @param onmatch - call-back function on matching
 */
void aho_corasick_simple(const char *s, const struct trie_node *trie,
	int (*onmatch)(const struct trie_node *))
{
	const struct trie_node *node = trie;
	int i, len;
	int chid, failch;

	len = strlen(s);
	for (i = 0; i < len; ++i) {
		chid = TRIE_IDX(s[i]);
		/* we don't call onmatch() during moving along failed pointer */
		while (node->nexts[chid] == NULL && node != trie)
			node = node->failed;
		if (node->nexts[chid]) {
			node = node->nexts[chid];
			if (node->ends && onmatch(node))
				return;
		}
	}
}
