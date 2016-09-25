#ifndef _WCC_LARGENUM_C
#define _WCC_LARGENUM_C

#include <math.h>
#include <string.h>

/* stirling's approximation:
 *	      ___ /n\ n
 *	n! = /2Pn|---| * T
 *		  \e/
 * T = 1 + 1/(12n) + 1/(288n^2) + o(n^-3)
 */
#define largenum_log10fact(n) ((double).5 * log10(2 * acos(-1) * (n)) \
	+ (n) * log10((n) / exp(1)) \
	+ log10((double)1 + (double)1 / (12 * (n)) + (double)1 / ((double)288 * (n) * (n))))

void largenum_mul(char *x, char *y, char *res)
{
	int carry;

	int nx, ny, base;
	int result1, result2, high1, low1, high2, low2;
	int i, j;

	nx = strlen(x);
	ny = strlen(y);

	memset(res, '0', nx + ny);

	/*
	 * Take one number yi from y,
	 * mutiply x with yi, we get a sub-product,
	 * sum this sub-product with res[].
	 */
	carry = 0;
	for (i = 0; i < nx; ++i) {
		for (j = 0; j < ny; ++j) {
			/*please remember this */
			base = i + j;

			/* result1 is for multiply */
			result1 = (x[i] - '0') * (y[j] - '0');
			high1 = result1 / 10;
			low1 = result1 % 10;

			/* result2 is for summary */
			result2 = res[base] + low1 + carry - 48;
			high2 = result2 / 10;
			low2 = result2 % 10;

			carry = high1 + high2;
			res[base] = low2 + 48;
		}
		/* important */
		while (carry) {
			++base;
			result2 = res[base] + carry - 48;
			res[base] = result2 % 10 + 48;
			carry = result2 / 10;
		}
	}
	res[++base] = 0;
}

#endif

