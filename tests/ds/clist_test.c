#include "address.h"
#include "clist.c"
#include "debug.c"

char strs[128][128];

void clist_print(struct clist *list)
{
	char *s;

	clist_for_each(list)
		printis(0, 0, "%s, ", clist_data(list, char *));
	puts("");
}

int main(void)
{
	int i = -1;
	char *str;
	struct clist *list;

	setbuf(stdout, NULL);

	list = clist_alloc();

	while (scanf("%s", strs[++i]) == 1) {
		clist_push(strs[i], list);
		clist_print(list);
	}

	while ((str = clist_pop(list)) != NULL) {
		printis(0, 0, "pop: %s\n", str);
	}

	return 0;
}
