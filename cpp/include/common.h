/** @file
 *
 * @author wcc
 * @date 2016-6
 * @version 1.0
 *
 * @brief Frequently Used Macros and Functions
 */

#ifndef __INCLUDE_COMMON_H
#define __INCLUDE_COMMON_H

#include <algorithm>
#include <climits>
#include <cstdarg>

/* 1UL is 32-bit */
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

/* containers */
#define foriter(_i, _c) for((_i) = (_c).begin(); (_i) != (_c).end(); ++(_i))

/* debug */
#define blstr(b) (b ? "true" : "false")

/* printing tools */

#define tailer(i, j) ((i) == (j) ? "\n" : " ")

/** @brief Print with indent */
void printi(int ind, const char *format, ...) {
	va_list args;

	for (int i = 0; i < ind; ++i)
		putchar('\t');
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
}

/** @brief Print dazzlingly */
void printdzl(const char *format, ...) {
	va_list args;
	printf("--------------------------------> ");
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
}

/** @brief Print tags, titles or cutlines */
void printt(const char *s, int type=1) {
	const char *raw = "%s";
	const char *title = "%s\n";
	const char *cutline = "--- %s --->\n";
	const char *longcut = "------ %s ------>\n";
	const char *llcut = "------------ %s ------------>\n";
	const char *types[] = {raw, title, cutline, longcut, llcut};
	assert(ir(type, 0, 5));
	printf(types[type], s);
}

/** @breif Print sequences
 */
template <class Iterator> void printa(const Iterator bgn, const Iterator end, const char *tag="", const int tagtype=0) {
	static const char *fmts[] = {"%c ", "%3d ", "%5lld "};
	const char *fmt = fmts[sizeof(*bgn)/4];
	printt(tag, tagtype);
	for (Iterator it = bgn; it != end; ++it)
		printf(fmt, *it);
	printf("\n");
}

/** @breif Print sequences who's elements own a 'print(void)' method
 */
template <class Iterator> void printb(const Iterator bgn, const Iterator end, const char *tag="", const int tagtype=0) {
	printt(tag, tagtype);
	for (Iterator it = bgn; it != end; ++it)
		it->print();
}

#endif
