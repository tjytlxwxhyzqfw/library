/*
 * numth
 */

#ifndef _TJYTLXWXHYZQFW_NUMTH_C
#define _TJYTLXWXHYZQFW_NUMTH_C

#define NUMTH_CORRECT(x, mod) ((x) += ((x) < 0 ? (mod) : 0))

#define NUMTH_ENS (-1) /* error: no solution */

#include <assert.h>

/* a^b % n */
long long numth_modexp(long long a, long long b, long long n);

/* gcd(|a|, |b|) , gcd(0, 0) = 0 */
long long numth_gcd(long long a, long long b);

/* *d = |a| * x + |b| * y , 0 = 0*0 + 0*0 */
void numth_gcd_e(long long a, long long b, long long *d, long long *x,
	long long *y);

/*
 * Modular linear equation solver
 * @ia534
 *
 * |a|*x = |b| (mod |n|), there are gcd(|a|, |n|) solutions,
 * this function returns just one of them.
 *
 * Parameters:
 * 	n:**positive** number
 * 	delta: = n / gcd(a, n)
 * 	solution: st. |a|*x = |b| (mod n), if a < 0, b > 0 
 * 		then -x is a solution of ax=b(mod n)
 * 	
 */
int numth_mle(long long a, long long b, long long n, long long *x,
	long long *delta);

/*
 * @ia534
 *
 * This ineffictive as there is a 'n/delta'.
 *
 * Important:
 * 	1. i & x must be variables.
 * 	2. ensure that the equation dose have solutions!
 */
#define numth_for_each_solution(i, x, n, delta) \
	for ((i) = 0; (i) < (n) / (delta); \
		(i) += 1, (x) = ((x) + (delta)) % (n))
	

static long long numth_do_gcd(long long greater, long long lesser);
static void numth_do_gcd_e(long long greater, long long lesser,
	long long *d, long long *x, long long *y);

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

void numth_gcd_e(long long a, long long b, long long *d,
	long long *x, long long *y)
{
	if (a == 0 && b == 0) {
		*d = *x = *y = 0;
		return;
	}

	if (a < 0)
		a *= -1;
	if (b < 0)
		b *= -1;

	if (a < b)
		numth_do_gcd_e(b, a, d, y, x);
	else
		numth_do_gcd_e(a, b, d, x, y);
}

static void numth_do_gcd_e(long long a, long long b,
	long long *d, long long *x, long long *y)
{
	long long tmp;

	if (b == 0) {
		*d = a;
		*x = 1;
		*y = 0;
		return;
	}
	numth_do_gcd_e(b, a % b, d, x, y);
	tmp = *x;
	*x = *y;
	//TODO: overflow?
	*y = (tmp - (a / b) * (*y));
}

int numth_mle(long long a, long long b, long long n, long long *solution,
	long long *delta)
{
	long long d, x, y;

	assert(n != 0);

	if (n < 0)
		n *= (-1);
	if (a < 0)
		a *= (-1);
	if (b < 0)
		b *= (-1);

	numth_gcd_e(a, n, &d, &x, &y);
	assert(d != 0); //d=0 if and only if a=b=0
	if (b % d)
		return NUMTH_ENS;
	*solution = ((b / d) * (x % n)) % n;
	NUMTH_CORRECT(*solution, n);
	*delta = n / d;
	return 0;
}

#endif
