/**
 * Algorithms for Number Theory
 *
 * @author wcc
 */

#ifndef _WCC_NUMTH_H
#define _WCC_NUMTH_H

#include <assert.h>

#define NUMTH_ENS (-1) /* error: no solution */

#define NUMTH_MODPLS(x, y, p) ((((x) % (p)) + ((y) % (p))) % p)
#define NUMTH_MODMUL(x, y, p) ((((X) % (P)) * ((y) % (p))) % p)

#define NUMTH_CORRECT(x, mod) ((x) += ((x) < 0 ? (mod) : 0))

/**
 * Modular exponentiation 
 *
 * @return result of a^b mod n.
 */
long long numth_modexp(long long a, long long b, long long n);

/** 
 * Greatest common divisor, gcd(|a|, |b|)
 *
 * @param a
 * @param b
 * @return A **non-negetive** number denoting gcd(|a|, |b|),
 * 	0 if (and only if) a = b = 0.
 * @see ia524
 */
long long numth_gcd(long long a, long long b);

/**
 * Extended gcd algothrim
 *
 * Compute d = gcd(|a|, |b|), as well as x, y that satisfies d = |a|x + |b|y.
 * d = x = y = 0 if (and only if) a = b = 0. Note that d must be non-negetive.
 */
void numth_gcd_e(long long a, long long b, long long *d, long long *x, long long *y);

/** 
 * Modular linear equation
 * 
 * Given |a|x = |b| (mod |n|), there are gcd(|a|, |n|) solutions (or none),
 * this function returns just one of them. Use numth_for_each_solution() to
 * traverse all solutions.
 *
 * @param n - **positive** number
 * @param gcd -  gcd(a, n)
 * @param x -  st. |a|*x = |b| (mod n). </br>
 * 	If a < 0, b > 0 then -x is a solution of ax=b(mod n)
 * 	
 * @see ia524, numth_for_each_solution().
 */
int numth_mle(long long a, long long b, long long n, long long *x, long long *gcd);

/**
 * Traverse all solutions of |a|x=|b| (mod |n|) (*)
 *
 * 1. Ensure that the equation dose have solutions!
 * 2. This ineffictive as there is a 'n/d'.
 *
 * @param i - necessary iterator
 * @param x - one solution given by numth_mle()
 * @param n - |n| in equation (*)
 * @param d - gcd(|a|, |n|)
 *
 * @see ia534, numth_mle()
 */
#define numth_for_each_solution(i, x, n, d) \
	for ((i) = 0; (i) < (d); (i) += 1, (x) = ((x) + (n) / (d)) % (n))

/** 
 * Divisors of positive integer k
 *
 * Divisors are sorted by asending order.
 *
 * This function doesn't check boundary, please make sure that
 * divs[] is big enouth.
 * 
 * @param k -  **positive** integer
 * @param divs - array to hold divisors of k
 * @param ndivs - numbers of divisors of k
 *
 * bits		divisors	mutipile
 * 1		4		
 * 2		12		3
 * 3		32		2.67
 * 4		64		2
 * 5		128		2
 * 6		240		1.875
 * 7		448		1.86
 * 8		768		1.714
 * 9		< 1316
 * 10		< 2256
 * 11		< 3867
 * 12		< 6629
 */
int numth_divisors(long long k, long long divs[], long long *ndivs);

#endif
