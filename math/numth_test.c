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

int main(void)
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
