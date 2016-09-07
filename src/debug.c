/*
 * 用于调试
 */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

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
