
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "../../debug.c"

struct kmp_param;

void kmp_presuf(const char *, const int, int *);
void kmp_simple(struct kmp_param *, int (*onmatch)(const int matchpoint, struct kmp_param *));

struct kmp_param {
	char *txt, *pat;
	int txtlen, patlen;
	int *pi;
	void *private;
};

/** kmp */

void kmp_presuf(const char *s, const int len, int *pi)
{
	int i, k;

	pi[0] = k = 0;
	for (i = 1; i < len; ++i) {
		while (k > 0 && s[i] != s[k])
			k = pi[k - 1];
		if (s[i] == s[k])
			++k;
		pi[i] = k;
	}
}

void kmp_simple(struct kmp_param *param, int (*onmatch)(const int, struct kmp_param *))
{
	int i, k;

	k = 0;
	for (i = 0; i < param->txtlen; ++i) {
		while (k > 0 && param->txt[i] != param->pat[k])
			k = param->pi[k - 1];
		if (param->txt[i] == param->pat[k])
			++k;
		if (k == param->patlen && onmatch(i + 1 - param->patlen, param))
				break;
	}
}

/** end kmp */

char t[10001], flag[10001], tr[10001];
char p[10000][101];

int onmatch(const int matchpoint, struct kmp_param *p)
{
	int i;
	for (i = matchpoint; i < matchpoint+p->patlen; ++i)
		flag[i] = 1;
	return 0;
}

void kmap_simple_test(void)
{
	int i, j, k, n, ch;
	int pi[10001];
	int nmatches;
	struct kmp_param param;

	param.txt = t;
	param.pi = pi;
	param.private = &nmatches;

	while (scanf("%d", &n) == 1) {
		memset(flag, 0, sizeof(flag));

		//printf("n: %d\n", n);
		for (i = 0; i < n; ++i) scanf("%s", p+i);

		scanf("%*c");
		i = 0;
		while((ch = getchar()) != '\n') t[i++] = ch;
		t[i] = 0;

		strcpy(tr, t);
		printf("tr: %s\n", tr);
		param.txtlen = strlen(t);

		for (i = 0; i < n; ++i) {
			j = strlen(p[i]);
			for (k = 0; k < j; ++k)
				if (isupper(p[i][k])) p[i][k] += 32;
			printf("p[%d]: %s\n", i, p[i]);
		}


		for (i = 0; i < param.txtlen; ++i)
			if (isupper(t[i])) t[i] += 32;

		for (i = 0; i < n; ++i) {
			param.pat = p[i];
			param.patlen = strlen(p[i]);
			kmp_presuf(param.pat, param.patlen, param.pi);
			kmp_simple(&param, onmatch); 
		}

		//for (i = 0; i < param.txtlen; ++i)
			//printf("%d", flag[i]);
		//puts("");

		for (i = 0; i < param.txtlen; ++i)
			putchar(flag[i] ? '*' : tr[i]);
		puts("");
	}
}

int main(void)
{
	kmap_simple_test();
	return 0;
}
