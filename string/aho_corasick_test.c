/*
 * Aho-Corasick Automathine
 *
 * need:
 * 	queue.c
 * 	trie.c
 */

#include <assert.h>

#include "../clist.c"
#include "../debug.c"
#include "trie.c"

#include "aho_corasick.c"

void print_trie(int deepth, const struct trie_node *x);
void print_path(const struct trie_node *x, const char *next);
void print_appends(const struct trie_node *x, const char *next);
void do_print_path(const struct trie_node *x);
char *string_new(const char *s);

int onmatch(const struct trie_node *node)
{
	printis(3, 0, "match: ");
	print_path(node, ": ");
	print_appends(node, "\n");
	return 0;
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

	printis(0, 0, "out of while\n");
	aho_corasick_fill(trie);
	printis(0, 0, "aho_corasick_fill(): ok\n");
	print_trie(0, trie);

	while (scanf("%s", buf) == 1)
		aho_corasick_simple(buf, trie, onmatch);

	return 0;
}

void do_print_path(const struct trie_node *x)
{
	if (x->parent == x || x->parent == NULL)
		return;
	do_print_path(x->parent);
	putchar(x->key);
}

void print_path(const struct trie_node *x, const char *next)
{
	do_print_path(x);
	printf("%s", next);
}

void print_appends(const struct trie_node *x, const char *next)
{
	struct clist_node *cnode;
	int i;

	clist_for_each(cnode, x->ends)
		print_path(clist_data(cnode, struct trie_node *), ", ");

	printf("%s", next);
}

void print_trie(int deepth, const struct trie_node *x)
{
	int i;

	printis(deepth, 0, "");
	print_path(x, "");
	printf("->");
	print_path(x->failed, ": ");
	print_appends(x, "\n");

	for (i = 0; i < TRIE_ASZ; ++i)
		if (x->nexts[i])
			print_trie(deepth + 1, x->nexts[i]);
}

char *string_new(const char *s)
{
	char *str;

	str = malloc(sizeof(s));
	strcpy(str, s);

	return str;
}
