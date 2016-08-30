/*
 * 用于调试
 */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void clock_stat(int cmd, char *tag, clock_t *total)
{
        static clock_t clocks;
        if (cmd == 0) {
                clocks = clock();
        } else {
                clocks = clock() - clocks;
		if (total)
			*total += clocks;
        }

        if (tag)
                printf("%s: delta: %lld, total: %lld\n", tag, clocks, total);
}


clock_t pup_total = 0, insert_total = 0, del_total = 0;


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

