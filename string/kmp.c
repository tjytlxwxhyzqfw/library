/*
 * Knuth-Morris-Prrat
 */

struct kmp_param;

void kmp_presuf(const char *, const int, int *);
void kmp_simple(struct kmp_param *, int (*onmatch)(const int matchpoint, struct kmp_param *));

struct kmp_param {
	char *txt, *pat;
	int txtlen, patlen;
	int *pi;
	void *private;
};

/*
 * 注意k的意义
 *
 *       | <---- 这叫做"对齐线"
 * ######|?
 *    ###|????
 *       |
 *
 * k: p在对齐线左边的长度,所以k也是p在对齐线右边第一个元素的下标
 * pi[k]: p[0:k]的端缀长度(包括p[k]在内哦)
 *
 * s[i] != s[k]意味着对齐线右边第一个元素不相等,
 * 	这样,我把p向右移动,移动到刚好在对齐线右边对齐的位置.
 * 	然后,继续比较对齐线右边第一个元素
 */
void kmp_presuf(const char *s, const int len, int *pi)
{
	int i, k;

	pi[0] = k = 0;
	for (i = 1; i < len; ++i) {
		while (k > 0 && s[i] != s[k])
			k = pi[k - 1];
		if (s[i] == s[k])
			++k;
		pi[i] = k;
	}
}

void kmp_simple(struct kmp_param *param, int (*onmatch)(const int, struct kmp_param *))
{
	int i, k;

	k = 0;
	for (i = 0; i < param->txtlen; ++i) {
		while (k > 0 && param->txt[i] != param->pat[k])
			k = param->pi[k - 1];
		if (param->txt[i] == param->pat[k])
			++k;
		if (k == param->patlen && onmatch(i + 1 - param->patlen, param))
				break;
	}
}
