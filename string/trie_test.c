#include <assert.h>
#include <stdlib.h>
#include <string.h>

//TODO: use listqueue
#include "../source/arrayqueue.c"
#include "../source/debug.c"
#include "trie.c"

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
