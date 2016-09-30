#ifndef _TJYTLXWXHYZQFW_COMMON_H
#define _TJYTLXWXHYZQFW_COMMON_H

#include <climits>

/* TODO: remember: 1UL is 32-bit */
#define ULL1 ((unsigned long long )1)
#define LL(x) ((long long)(x))

/* Swap */
#define SWP(a, b) if ((a) != (b)) do {(a) ^= (b); (b) ^= (a); (a) ^= (b);} while(0)
/* Set x to X_MAX if x is negetive (which is caused by overflow) */
#define IOF(p) ((p) = ((p) < 0 ? INT_MAX : (p)))
#define LLOF(p) ((p) = ((p) < 0 ? LLONG_MAX :(p)))

/* Is x in range [start, end) */
#define ir(x, start, end) ((x) >= (start) && (x) < (end))
/* Is x in range [start, end], 'E' stand for 'equal' */
#define ire(x, start, end) ((x) >= (start) && (x) <= (end))
#define odd(x) ((x) & 1UL)
#define even(x) (!odd((x)))
#define sgn(n) (odd((n)) ? (-1) : (1))
/* Absolute value */
#define abv(x) ((x) < 0 ? (x) * (-1) : (x))

/* loops */
#define forn(i, n) for ((i) = 0; (i) < (n); (i) += 1)
#define fore(i, n) for ((i) = 0; (i) <= (n); (i) += 1)
#define forr(i, s, e) for((i) = (s); (i) < (e); (i) += 1)
#define forre(i, s, e) for((i) = (s); (i) <= (e); (i) += 1)

/* tools*/
#define tailer(i, j) ((i) == (j) ? "\n" : " ")
#define printa(a, i, n) forn((i), (n)) cout << (a)[(i)] << tailer((i), (n-1))

/* containers */
#define vecsize(t) vector<t>::size_type
#define veciter(t) vector<t>::iterator

#endif