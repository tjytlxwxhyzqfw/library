/*
 * numth
 */

#include <stdio.h>
#include <stdlib.h>

#include "numth.c"

int test_modexp(void)
{
	long long a, b, n;
	while (scanf("%lld%lld%lld", &a, &b, &n) == 3)
		printf("%lld\n", numth_modexp(a, b, n));

	return 0;
}

int main(void)
{
	long long a, b;

	while (scanf("%lld%lld", &a, &b) == 2)
		printf("%lld\n", numth_gcd(a, b));

	return 0;
}
