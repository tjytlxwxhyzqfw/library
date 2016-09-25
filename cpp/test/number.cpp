/**
 * @author wcc
 */
#include <algorithm>
#include <cstdio>

#include "number.h"

int main(void)
{
	long long module = 2147483647;
	long long left, right, d, a, b;
	llong x(12, module), y(98798797, module);

	scanf("%lld%lld", &left, &right);
	printf("left: %lld, right: %lld\n", left, right);

	d = number::gcd(left, right, a, b);
	printf("gcd(%lld, %lld): %lld (x: %lld, y: %lld)\n", left, right, d, a, b);
	printf("perm(%lld, %lld): %lld\n", left, right, number::perm(left, right, module));
	printf("%lld!: %lld, %lld!: %lld\n", left, number::fact(left, module), right, number::fact(right, module));
	printf("comb(%lld, %lld): %lld\n", left, right, number::comb(left, right, module));

	return 0;
}
