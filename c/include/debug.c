/*
 * 用于调试
 */

#ifndef __WCC_DEBUG_C
#define __WCC_DEBUG_C

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define DEBUG

int __debugger_private;

#ifndef _I
#define _I __debugger_private
#else
#define _I "macro '_I' has been defined!"
#endif

#define printni(is, ss, n) do {for (_I = 0; _I < (n); ++_I) printf("%s: %4d, ", is[_I], ss[_I]); puts("");} while (0)

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

void print(char *format, ...)
{
	int pause = 0;

	va_list args;
	char ch;

	/* this is wrong:
 	 * printf("%d%s", va_arg(), va_arg());
 	 * Maybe I assume too mush about computing order
 	 */

	va_start(args, format);
	while (*format) {
		ch = *format++;
		switch (ch) {
			case 's':
				printf("%s", va_arg(args, char *));
				break;
			case 'i':
				printf("%4d", va_arg(args, int));
				break;
			case 'I':
				indent(va_arg(args, int));
				break;
			case 'S':
				pause = va_arg(args, int);
				break;
			default:
				putchar(ch);
		}
	}
	puts("");
	va_end(args);

	sleep(pause);
}

#endif
