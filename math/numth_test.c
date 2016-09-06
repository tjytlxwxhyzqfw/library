/*
 * numth
 */

#include <stdio.h>
#include <stdlib.h>

#include "/home/wcc/OnlineJudge/source/common.h"

#include "numth.c"

int test_modexp(void)
{
	long long a, b, n;
	while (scanf("%lld%lld%lld", &a, &b, &n) == 3)
		printf("%lld\n", numth_modexp(a, b, n));

	return 0;
}

int test_gcd(void)
{
	long long a, b;

	while (scanf("%lld%lld", &a, &b) == 2)
		printf("%lld\n", numth_gcd(a, b));

	return 0;
}

int test_gcd_e(void)
{
	long long a, b, d, x, y;

	while (scanf("%lld%lld", &a, &b) == 2) {
		numth_gcd_e(a, b, &d, &x, &y);
		printf("%lld: %lldx(%lld)+%lldx(%lld)\n", 
			d, a, x, b, y);
	}

	return 0;
}

int test_mle(void)
{
	long long a, b, n, x, delta;
	long long i;

	while (scanf("%lld%lld%lld", &a, &b, &n) == 3) {
		if (numth_mle(a, b, n, &x, &delta) == 0) {
			numth_for_each_solution(i, x, ABS(n), delta)
				printf("%lld\n", x);
		} else {
			printf("no solution\n");
		}
	}

	return 0;
}

int test_divisors(void)
{
	long long k, i;
	long long divs[10000], ndivs;

	while (scanf("%lld", &k) == 1) {
		numth_divisors(k, divs, &ndivs);
		for (i = 0; i < ndivs; ++i)
			printf("%lld, ", divs[i]);
		printf("\n");
	}

	return 0;
}

int main(void)
{
	long long i;
	long long divs[100000], ndivs, ndivs_max;

	ndivs_max = 0;
	for (i = 1; i < 1000000000000; ++i) {
		numth_divisors(i, divs, &ndivs);
		if (ndivs > ndivs_max) {
			ndivs_max = ndivs;
			printf("ndivs: %8lld, i: %8lld\n", ndivs, i);
		}
	}

	return 0;
}


