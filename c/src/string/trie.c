/**
 * @author wcc
 */
#include "../../include/trie.h"

struct trie_node *trie_node_new(char key)
{
	struct trie_node *node;

	node = malloc(sizeof(struct trie_node));
	assert(node);

	node->key = key;
	node->end = 0;
	node->parent = NULL;
	memset(node->nexts, 0, TRIE_ASZ * sizeof(struct trie_node *));

	#ifdef TRIE_AHO_CORASICK
	node->failed = NULL;
	node->ends = 0;
	#endif

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
		node = node->nexts[TRIE_IDX(s[i])];
		++i;
	}

	while (s[i]) {
		node->nexts[TRIE_IDX(s[i])] = trie_node_new(s[i]);
		node->nexts[TRIE_IDX(s[i])]->parent = node;
		node = node->nexts[TRIE_IDX(s[i])];
		++i;
	}

	++node->end;
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
		--node->end;
}
