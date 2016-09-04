/*
 * numth
 */

#ifndef _TJYTLXWXHYZQFW_NUMTH_C
#define _TJYTLXWXHYZQFW_NUMTH_C

#include <assert.h>

/* a^b % n */
long long numth_modexp(long long a, long long b, long long n);
long long numth_gcd(long long a, long long b);

static long long numth_do_gcd(long long greater, long long lesser);

long long numth_modexp(long long a, long long b, long long n)
{
	static long long bit63 = (unsigned long long)0x8000000000000000;
	long long square = 1;
	int i;

	assert(b >= 0 && n > 0);

	//FIXME: '==' > '&'
	for(i = 63; i >= 0 && (b & bit63) == 0; --i)
		b <<= 1;
	for(; i >= 0; --i) {
		square = (square * square) % n;
		if (b & bit63)
			square = (square * a) % n;
		b <<= 1;
	}

	return square;
}

long long numth_gcd(long long a, long long b)
{
	assert(a != 0 || b != 0);
	if (a < b)
		return numth_do_gcd(b, a);
	return numth_do_gcd(a, b);
}

/* 大数除小数, 小数除余数 */
static long long numth_do_gcd(long long a, long long b)
{
	long long r;

	if (!b)
		return a;

	for (r = a % b; r != 0; r = a % b) {
		a = b;
		b = r;
	}

	return b;
}

#endif
