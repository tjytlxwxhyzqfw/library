/*
 * Aho-Corasick Automathine
 *
 * need:
 * 	queue.c
 * 	trie.c
 */

#include "arrayqueue.c" //TODO: use listqueue
#include "debug.c"
#include "trie.c"

void do_print_path(const struct trie_node *x)
{
	if (x->parent == x || x->parent == NULL)
		return;
	do_print_path(x->parent);
	putchar(x->key);
}

void print_path(const struct trie_node *x, const char next)
{
	do_print_path(x);
	putchar(next);
}

void print_trie(int deepth, const struct trie_node *x)
{
	int i;

	printis(deepth, 0, "");
	print_path(x, 0);
	printf(" -> ");
	print_path(x->failed, '\n');

	for (i = 0; i < TRIE_ASZ; ++i)
		if (x->nexts[i])
			print_trie(deepth + 1, x->nexts[i]);
}

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

		for (i = 0; i < TRIE_ASZ; ++i)
			if (node->nexts[i])
				queue_append(node->nexts[i], q);
	}

	queue_destroy(q);
}

void aho_corasick_simple(const char *s, const struct trie_node *trie)
{
	const struct trie_node *node = trie;
	int i, len;
	int chid, failch;

	len = strlen(s);
	for (i = 0; i < len; ++i) {
		printis(2, 0, "node: ");
		print_path(node, 0);
		printis(0, 0, ", ch: %c\n", s[i]);
		chid = TRIE_IDX(s[i]);

		try_accept:
		//printis(3, 0, "try_accept: ");
		//print_path(node, '\n');
		if (node->nexts[chid]) {
			node = node->nexts[chid];
			if (node->end) {
				printis(3, 0, "match: ");
				print_path(node, '\n');
			}
			continue;
		}

		on_failed:
		//printis(3, 0, "on_failed: ");
		//print_path(node, '\n');
		if (node != trie) {
			node = node->failed;
			if (node->end) {
				printis(3, 0, "match: ");
				print_path(node, '\n');
			}
			goto try_accept;
		}
	}
}

char *string_new(const char *s)
{
	char *str;

	str = malloc(sizeof(s));
	strcpy(str, s);

	return str;
}

int main(void)
{
	struct trie_node *trie;
	char buf[100], *str;
	int cmd;

	freopen("tmp", "r", stdin);
	setbuf(stdout, NULL);

	trie = trie_node_new('r');

	while (scanf("%d%s", &cmd, buf) == 2) {
		str = string_new(buf);
		switch (cmd) {
			case 1:
				printis(1, 0, "insert: %s\n", str);
				trie_insert(str, trie);
				break;
			case 2:
				printis(1, 0, "find: %s\n", str);
				if (trie_find(str, trie))
					printis(2, 0, "found: %s\n", str);
				break;
			case 3:
				printis(1, 0, "remove: %s\n", str);
				trie_remove(str, trie);
				break;
			default:
				assert(0);
		}
	}

	aho_corasick_fill(trie);
	print_trie(0, trie);

	while (scanf("%s", buf) == 1)
		aho_corasick_simple(buf, trie);

	return 0;
}
