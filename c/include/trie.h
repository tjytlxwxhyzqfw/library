#ifndef _WCC_TRIE_C
#define _WCC_TRIE_C

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define TRIE_ASZ 2 
#define TRIE_ABS 48 
#define TRIE_IDX(x) ((x) - TRIE_ABS)

/**
 * .key - a character
 * .end - number of occurrences of the word
 * 	insert ++ .end
 *	remove -- .end
 * .parent
 * .nexts
 * .failed - used by aho-corasick
 * .ends - used by aho-corasick
 */
struct trie_node {
	/* consider key as value of node's handle */
	char key;
	int end;
	struct trie_node *parent;
	struct trie_node *nexts[TRIE_ASZ];

	struct trie_node *failed;
	int ends;
};

struct trie_node *trie_node_new(char key);

/**
 * trie->end = 1 if s='\0'
 */
void trie_insert(char *s, struct trie_node *trie);
struct trie_node *trie_find(char *s, struct trie_node *trie);

/**
 * Minus .end by one
 *
 * @param s - Word to be removed
 * @param trie
 *
 * Use trie_find() instead of this function for more flexibility
 */
void trie_remove(char *s, struct trie_node *trie);

#endif
