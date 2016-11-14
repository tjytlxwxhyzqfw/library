#ifndef __INCLUDE_COMMON_H
#define __INCLUDE_COMMON_H

#include <algorithm>
#include <climits>

/* TODO: remember: 1UL is 32-bit */
#define ULL1 ((unsigned long long )1)
#define LL(x) ((long long)(x))

/* Set x to X_MAX if x is negetive (which is caused by overflow) */
/* You may check and re-write your code if you have to deal with overflow */
// #define IOF(p) ((p) = ((p) < 0 ? INT_MAX : (p)))
// #define LLOF(p) ((p) = ((p) < 0 ? LLONG_MAX :(p)))

/* Is x in range [start, end) */
#define ir(x, start, end) ((x) >= (start) && (x) < (end))
/* Is x in range [start, end], 'E' stand for 'equal' */
#define ire(x, start, end) ((x) >= (start) && (x) <= (end))
#define odd(x) ((x) & 1UL)
#define even(x) (!odd((x)))
#define sgn(n) (odd((n)) ? (-1) : (1))
#define max3(a, b, c) std::max(std::max((a),(b)), (c))
/* Absolute value */
#define abv(x) ((x) < 0 ? (x) * (-1) : (x))

/* loops */
#define forn(_i, _n) for ((_i) = 0; (_i) < (_n); (_i) += 1)
#define fore(_i, _n) for ((_i) = 0; (_i) <= (_n); (_i) += 1)
#define forr(_i, _s, _e) for((_i) = (_s); (_i) < (_e); (_i) += 1)
#define forre(_i, _s, _e) for((_i) = (_s); (_i) <= (_e); (_i) += 1)
#define rfor(_i, _n) for((_i)=(_n)-1; ((_i) >= 0); (_i) -= 1)
#define rfore(_i, _n) for((_i)=(_n); (_i) >= 0; (_i) -= 1)
#define rforre(_i, _b, _e) for ((_i) = (_b); (_i) >= (_e); (_i) -= 1) 

/* tools*/
#define tailer(i, j) ((i) == (j) ? "\n" : " ")

/* containers */
#define vecsize(t) vector<t>::size_type
#define veciter(t) vector<t>::iterator
#define foriter(_i, _c) for((_i) = (_c).begin(); (_i) != (_c).end(); ++(_i))

/* debug */
#define blstr(b) (b ? "true" : "false")

/* read/print */

bool scanfnint(int *a, const int n) {
	for (int i = 0; i < n; ++i)
		if (scanf("%d", &a[i]) != 1)
			return false;
	return true;
}

void printnint(const int *a, const int n, const char *tag="") {
	printf("%s", tag);
	for (int i = 0; i < n; ++i)
		printf("%5d%s", a[i], i == n-1 ? "\n" : " ");
}

#endif
