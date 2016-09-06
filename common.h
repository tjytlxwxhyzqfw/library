#ifndef _TJYTLXWXHYZQFW_COMMON_H
#define _TJYTLXWXHYZQFW_COMMON_H

#include <limits.h>

#define offset(t,m) ((unsigned long)&(((t*)0)->m))
#define container_of(a,t,m) ((t*)((unsigned long)a-offset(t,m)))

//TODO: remember: 1UL is 32-bit
#define ULL1 ((unsigned long long )1)

/* swap */
#define SWP(a, b) do {a ^= b; a ^= a; b ^= a} while(0)

/* Set x to X_MAX if x is negetive (which is caused by overflow) */
#define IOF(p) ((p) = ((p) < 0 ? INT_MAX : (p)))
#define LLOF(p) ((p) = ((p) < 0 ? LLONG_MAX :(p)))

/* is x in range [start, end)*/
#define IR(x, start, end) ((x) >= (start) && (x) < (end))

/* absolute value */
#define ABS(x) ((x) < 0 ? (x) * (-1) : (x))

#endif
