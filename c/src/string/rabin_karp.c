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
 *
 * 坏的序列:
 * t: aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
 * p: aaaa
 */

#ifndef _TJYTLXWXHYZQFW_RABIN_KARP_C
#define _TJYTLXWXHYZQFW_RABIN_KARP_C

#include <string.h>

#define RK_MAGIC 0xfffffdUL
#define RK_RADIX 128

struct rabin_karp_param {
	char *txt, *pat;
	int txtlen, patlen;
	int txtval, patval;
	int power;
	void *private;
};

/* (radix * (pre - left * power) + right) % magic */
#define rabin_karp_next(pre, left, right, power) \
	((RK_RADIX * (((pre) + RK_MAGIC - ((left) * (power)) % RK_MAGIC) % RK_MAGIC) + (right)) % RK_MAGIC)

int rabin_karp_power(int m)
{
	int power = 1;

	while (--m)
		power = (power * RK_RADIX) % RK_MAGIC;

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

void rabin_karp_simple(struct rabin_karp_param *param,
	int (*onmatch)(const int, struct rabin_karp_param *))
{
	int ilast, i;
	int tval;

	tval = param->txtval;
	ilast = param->txtlen - param->patlen;
	for (i = 0; i <= ilast; ++i) {
		if (tval == param->patval && onmatch(i, param))
			break;
		tval = rabin_karp_next(tval, param->txt[i], param->txt[i + param->patlen], param->power);
	}
}

#endif
