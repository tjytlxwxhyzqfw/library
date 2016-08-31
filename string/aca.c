#include <assert.h>
#include <stdlib.h>
#include <string.h>

//TODO: use listqueue
#include "arrayqueue.c"
#include "debug.c"

#define TRIE_ASZ 128
#define TRIE_ABS 0
#define TRIE_IDX(x) ((x) - TRIE_ABS)

struct trie_node {
	/* consider key as value of node's handle */
	char key;
	char end;
	struct trie_node *parent;
	struct trie_node *nexts[TRIE_ASZ];

	struct trie_node *failed;
};

struct trie_node *trie_node_new(char key)
{
	struct trie_node *node;

	node = malloc(sizeof(struct trie_node));
	assert(node);

	node->key = key;
	node->end = 0;
	node->parent = NULL;
	memset(node->nexts, 0, TRIE_ASZ * sizeof(struct trie_node *));

	return node;
}

/*
 * trie->end = 1 if s='\0'
 */
void trie_insert(char *s, struct trie_node *trie)
{
	int i = 0;
	struct trie_node *node = trie;

	while (s[i] && node->nexts[TRIE_IDX(s[i])]) {
		printis(1, 0, "trie_insert: meet: %c\n", node->nexts[TRIE_IDX(s[i])]->key);
		node = node->nexts[TRIE_IDX(s[i])];
		++i;
	}

	while (s[i]) {
		node->nexts[TRIE_IDX(s[i])] = trie_node_new(s[i]);
		node->nexts[TRIE_IDX(s[i])]->parent = node;
		printis(1, 0, "trie_insert: new: %c parent: %c\n", node->nexts[TRIE_IDX(s[i])]->key, node->key);
		node = node->nexts[TRIE_IDX(s[i])];
		++i;
	}

	node->end = 1;
}

struct trie_node *trie_find(char *s, struct trie_node *trie)
{
	int i = 0;
	struct trie_node *node = trie;

	while (s[i] && node->nexts[TRIE_IDX(s[i])]) {
		node = node->nexts[TRIE_IDX(s[i])];
		++i;
	}

	if (!s[i] && node->end)
		return node;
	return NULL;
}

void trie_remove(char *s, struct trie_node *trie)
{
	struct trie_node *node;

	node = trie_find(s, trie);
	if (node)
		node->end = 0;
}


/*
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
 * Here is a useful property: N is failed pointer always points to
 * a node with key == N->key.
 */
void aho_corasick_fill(struct trie_node *trie)
{
	struct trie_node *node = trie;
	struct trie_node *hope;
	struct queue *q;
	int i;

	//TODO: use listqueue
	q = queue_new(1000000);

	/*
 	 * trie->nexts[x] = trie if trie does't have a child x.
 	 *
 	 * This will simplify failed pointer tracing.
 	 */
	for (i = 0; i < TRIE_ASZ; ++i)
		if (node->nexts[i] == NULL)
			node->nexts[i] = node;
	node->failed = node;

	queue_append(node, q);

	while ((node = queue_pop(q)) != NULL) {
		assert(node->parent);
		assert(node->parent->failed);
		/*
		 * We compute failed pointer for every node,
		 * although some of them is never used.
		 *
		 */
		hope = node->parent->failed;
		while (hope != trie && hope->nexts[TRIE_IDX(node->key)] == NULL)
			hope = hope->failed;
		node->failed = hope->nexts[TRIE_IDX(node->key)];
		assert(node->failed == trie || node->failed->key == node->key);

		for (i = 0; i < TRIE_ASZ; ++i)
			if (node->nexts[i])
				queue_append(node->nexts[i], q);
	}

	queue_destroy(q);
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

	trie = trie_node_new(0);

	while (scanf("%d%s", &cmd, buf) == 2) {
		str = string_new(buf);
		switch (cmd) {
			case 1:
				trie_insert(str, trie);
				break;
			case 2:
				if (trie_find(str, trie))
					printis(2, 0, "found: %s\n", str);
				break;
			case 3:
				trie_remove(str, trie);
				break;
			default:
				assert(0);
		}
	}

	return 0;
}
