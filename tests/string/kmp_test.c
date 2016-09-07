/*
 * Knuth-Morris-Prrat
 */

#include <string.h>

#include "../debug.c"
#include "kmp.c"

void kmap_presuf_test(void)
{
	char buf[65535];
	int pi[65535];

	int i, j, k, len;
	char ch;

	while (scanf("%s", buf) == 1) {
		printis(0, 0, "buf: %s\n", buf);
		len = strlen(buf);
		kmp_presuf(buf, len, pi);
		for (i = 0; i < len; ++i) {
			ch = buf[i + 1];
			buf[i + 1] = 0;

			k = pi[i];
			printis(1, 0, "k: %d\n", k);
			printis(1, 0, "%s\n", buf);

			printis(1, 0, "");
			for (j = 0; j < i + 1 - k; ++j)
				putchar(' ');
			printis(0, 0, "%s\n", buf);

			buf[i + 1] = ch;
		}
	}
}

int onmatch(const int matchpoint, struct kmp_param *p)
{
	printis(1, 0, "onmatch(): [%3d, %3d)\n", matchpoint, matchpoint + p->patlen);
	*((int *)(p->private)) += 1;
	return 0;
}

void kmap_simple_test(void)
{
	char t[65535], p[65535];
	int pi[65535];
	int nmatches;
	struct kmp_param param;

	param.txt = t;
	param.pat = p;
	param.pi = pi;
	param.private = &nmatches;
	
	while (scanf("%s", t) == 1) {
		param.txtlen = strlen(t);
		while (scanf("%s", p) == 1) {
			if (!strcmp(p, "end"))
				break;
			*(int *)param.private = 0;
			param.patlen = strlen(p);
			kmp_presuf(param.pat, param.patlen, param.pi);
			kmp_simple(&param, onmatch); 
			printis(0, 0, "nmatches: %d\n", *(int *)param.private);
		}
	}
}

int main(void)
{
	kmap_simple_test();
	return 0;
}
