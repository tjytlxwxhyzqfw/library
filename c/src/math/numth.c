/**
 * Algorithms for Number Theory
 *
 * @author wcc
 */

#include <string.h>
#include "../../include/numth.h"

static long long numth_do_gcd(long long a, long long b);
static void numth_do_gcd_e(long long grt, long long les, long long *d, long long *x, long long *y);
#define matident(x) ((x)[0][0] = (x)[1][1] = 1, (x)[0][1] = (x)[1][0] = 0)
#define matprint(m) printf("%6lld %6lld\n%6lld %6lld\n", (m)[0][0], (m)[0][1], (m)[1][0], (m)[1][1]);
void matmul(long long x[2][2], long long y[2][2], long long z[2][2], long long mod);
void matpow(long long mat[2][2], long long n, long long mod);

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
	if (a == 0 && b == 0)
		return 0;
	if (a < 0)
		a *= -1;
	if (b < 0)
		b *= -1;
	/* We DON'T need a > b */
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

void numth_gcd_e(long long a, long long b, long long *d, long long *x, long long *y)
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

static void numth_do_gcd_e(long long a, long long b, long long *d, long long *x, long long *y)
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

int numth_mle(long long a, long long b, long long n, long long *solution, long long *gcd)
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
	*gcd = d;

	return 0;
}

int numth_divisors(long long k, long long divs[], long long *ndivs)
{
	long long i, n;

	assert(k > 0);

	n = 0;
	for (i = 1; i * i < k; ++i)
		if (k % i == 0)
			divs[n++] = i;

	if (i * i == k) {
		divs[n++] = i;
		i = n - 2;
	} else {
		i = n - 1;
	}

	for (; i >= 0; --i)
		divs[n++] = k / divs[i];

	*ndivs = n;

	return 0;
}

long long numth_fibonacci(long long n, long long mod)
{
	long long mat[2][2];

	if (n == 0) return 1;

	mat[0][0] = mat[0][1] = mat[1][0] = 1;
	mat[1][1] = 0;

	matpow(mat, n, mod);

	return mat[0][0];
}

void matmul(long long x[2][2], long long y[2][2], long long z[2][2], long long mod)
{
	z[0][0] = NUMTH_MODPLS(NUMTH_MODMUL(x[0][0], y[0][0], mod), NUMTH_MODMUL(x[0][1], y[1][0], mod), mod);	
	z[0][1] = NUMTH_MODPLS(NUMTH_MODMUL(x[0][0], y[0][1], mod), NUMTH_MODMUL(x[0][1], y[1][1], mod), mod);
	z[1][0] = NUMTH_MODPLS(NUMTH_MODMUL(x[1][0], y[0][0], mod), NUMTH_MODMUL(x[1][1], y[1][0], mod), mod);
	z[1][1] = NUMTH_MODPLS(NUMTH_MODMUL(x[1][0], y[0][1], mod), NUMTH_MODMUL(x[1][1], y[1][1], mod), mod);
}

void matpow(long long mat[2][2], long long n, long long mod)
{
	int i;
	long long square[2][2], ans[2][2], res[2][2];

	matident(ans);
	memcpy(square, mat, sizeof(square));

	for (i = 0; i < 64; ++i) {
		if (n & 1UL) {
			matmul(ans, square, res, mod);
			memcpy(ans, res, sizeof(res));
		}
		matmul(square, square, res, mod);
		memcpy(square, res, sizeof(res));
		n >>= 1;
	}

	memcpy(mat, ans, sizeof(ans));
}
