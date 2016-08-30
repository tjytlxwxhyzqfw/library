/*
 * Rabin-Karp
 *
 * ia560
 *
 * 0xfffffd * 128 < 0x7fffffff
 * 16777213
 *
 * radix <= 128
 *
 * 注意,以下表述是**错误**的:
 * 	p中必须存储纯数,而非ascii字符
 * 	比如,如果字母表是{a,b,c,d,e},p="abc",则p中必须存储"0,1,2"
 * 因为基数取128,所以可以作用于所有ascii字符;
 */

#include <string.h>

#define RK_MAGIC 0xfffffdUL
#define RK_RADIX 128

#define rabin_karp_next(ti, left, right, power) \
	((RK_RADIX * ((ti) - (left) * (power)) + right) % RK_MAGIC)

int rabin_karp_power(int m)
{
	int power = 1;

	while (--m) {
		power = (power * RK_RADIX) % RK_MAGIC;
	}

	return power;
}
		

int rabin_karp_value(const char *p, const int m)
{
	int i, result;

	result = p[0];
	for (i = 1; i < m; ++i)
		result = ((result * RK_RADIX) % RK_MAGIC + p[i] % RK_MAGIC) % RK_MAGIC;

	return result;
}

void rabin_karp_query_init(const char *p, const char *t, 
	int *lenp, int *pval, int *tval, int *power)
{
	*lenp = strlen(p);
	*pval = rabin_karp_value(p, *lenp);
	*tval = rabin_karp_value(t, *lenp);
	*power = rabin_karp_power(*lenp);
}

void rabin_karp_simple(const char *t, const char *p, const int lent,
	void (*onmatch)(const char *, const int, const int, void *),
	void *rkres)
{
	int ilast, i;
	int lenp, pval, tval, power;

	rabin_karp_query_init(p, t, &lenp, &pval, &tval, &power);

	ilast = lent - lenp;
	for (i = 0; i <= ilast; ++i) {
		if (tval == pval)
			onmatch(t, i, i + lenp, rkres);
		tval = rabin_karp_next(tval, t[i], t[i + lenp], power);
	}
}

#include "debug.c"

struct rkres {
	int nmatches;
};

void onmatch(const char *t, const int start, const int end, void *rkres)
{
	int i = 0;
	printis(1, 0, "onmatch(): [%3d, %3d): ", start, end);
	for (i = start; i < end; ++i)
		putchar(t[i]);
	printis(0, 0, "\n");

	++((struct rkres *)rkres)->nmatches;
}

int main(void)
{
	char t[65535], p[65535];
	int lent;
	struct rkres rkres;

	while (scanf("%s", t) == 1) {
		lent = strlen(t);
		while (scanf("%s", p) == 1) {
			if (!strcmp(p, "end"))
				break;
			printis(0, 0, "t: %s\np: %s\n", t, p);

			rkres.nmatches = 0;
			rabin_karp_simple(t, p, lent, onmatch, &rkres);
			printis(0, 0, "nmatches: %d\n", rkres.nmatches);
		}
	}

	return 0;
}
