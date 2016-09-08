#ifndef _TJYTLXWXHYZQFW_COMMON_H
#define _TJYTLXWXHYZQFW_COMMON_H

#include <limits.h>

#define offset(t,m) ((unsigned long)&(((t*)0)->m))
#define container_of(a,t,m) ((t*)((unsigned long)a-offset(t,m)))

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
#define max(x,y) ((x) < (y) ? (y) : (x))
#define min(x, y) ((x) < (y) ? (x) : (y))
/* Absolute value */
#define abv(x) ((x) < 0 ? (x) * (-1) : (x))

#define forn(i, n) for ((i) = 0; (i) < (n); (i) += 1)
#define fore(i, n) for ((i) = 0; (i) <= (n); (i) += 1)
#define forr(i, s, e) for((i) = (s); (i) < (e); (i) += 1)
#define forre(i, s, e) for((i) = (s); (i) <= (e); (i) += 1)

#endif
