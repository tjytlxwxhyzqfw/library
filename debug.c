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
	
	
void printi(int ind, const char *format, ...)
{
	va_list args;

	indent(ind);
	
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
}

void prints(int sec, const char *format, ...)
{
	va_list args;

	va_start(args, format);
	vprintf(format, args);
	va_end(args);

	sleep(sec);
}

