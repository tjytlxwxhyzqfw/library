/*
 * 用于调试
 */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define printnint(s, n, i) for((i) = 0; (i) < (n); (i) += 1) printf("%d%s", (s)[(i)], (i) == ((n) - 1) ? "\n" : ", ")
#define printnll(s, n, i) for((i) = 0; (i) < (n); (i) += 1) printf("%lld%s", (s)[(i)], (i) == ((n) - 1) ? "\n" : ", ")
#define print2int(a,b,sa,sb) printf("%s: %4d, %s: %4d\n", (sa), (a), (sb), (b))

static void indent(int ind)
{
	while (ind--)
		putchar('\t');
}

void printis(int ind, int sec, const char *format, ...)
{
	va_list args;

	indent(ind);
	
	va_start(args, format);
	vprintf(format, args);
	va_end(args);

	sleep(sec);
}
