
#include "../debug.c"
#include "rabin_karp.c"

int onmatch(const int matchpoint, struct rabin_karp_param *param)
{
	printis(1, 0, "onmatch(): [%3d, %3d)\n", matchpoint, matchpoint + param->patlen);
	++*(int *)param->private;
	return 0;
}

int main(void)
{
	char t[65535], p[65535];
	struct rabin_karp_param param;
	int nmatches;

	param.txt = t;
	param.pat = p;
	param.private = &nmatches;

	while (scanf("%s", param.txt) == 1) {
		param.txtlen = strlen(param.txt);
		while (scanf("%s", param.pat) == 1) {
			if (!strcmp(param.pat, "end"))
				break;
			printis(0, 0, "t: %s\np: %s\n", param.txt, param.pat);

			param.patlen = strlen(param.pat);
			param.patval = rabin_karp_value(param.pat, param.patlen);
			param.txtval = rabin_karp_value(param.txt, param.patlen);
			param.power = rabin_karp_power(param.patlen);

			*(int *)param.private = 0;
			rabin_karp_simple(&param, onmatch);
			printis(0, 0, "nmatches: %d\n", *(int *)param.private);
		}
	}

	return 0;
}
