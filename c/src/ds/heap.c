#include <assert.h>
#include <stdlib.h>

#include "../../include/heap.h"

struct heap *heap_alloc(int cap, int (*cmp)(const void *, const void*)
	#ifdef DIJKSTRA
	, void (*set)(void *, int)
	#endif
)
{
        struct heap *h;

        assert(h = malloc(sizeof(struct heap)));
	/* 由于cell[0]不存放元素,故实际分配的元素是(cap + 1)个 */
        h->cap = ++cap;
	/* linux会替你掩饰你犯下的**忘掉初始化**的错误, OJ不会 */
	h->last = 0;
        assert(h->cell = malloc(sizeof(void*) * cap));
	/* comparison function */
	h->cmp = cmp;

	#ifdef DIJKSTRA
	h->sethi = set;
	#endif

        return h;
}

void heap_destroy(struct heap *h)
{
        if (h)
                free(h->cell);
        free(h);
}

void heap_pdown(int x, struct heap *h)
{
        int c, d;
        void *fatty;

	/* x始终是个空格 */
	fatty = h->cell[x];
        while ((c = 2 * x) <= h->last) {
                d = c + 1;
		/* 两个儿子争取占用空格的机会 */
                if (d <= h->last && h->cmp(h->cell[d], h->cell[c]) < 0)
                        c = d;
		/* 小儿子和胖子争取占用空格的机会 */
                if (h->cmp(fatty, h->cell[c]) < 0) 
                        break;

		/* 小儿子赢了 */
                h->cell[x] = h->cell[c];
		#ifdef DIJKSTRA
		h->sethi(h->cell[x], x);
		#endif

                x = c;
        }
	/* 胖子赢了或者胖子没有竞争对手了(空格没儿子了) */
	h->cell[x] = fatty;
	#ifdef DIJKSTRA
	h->sethi(fatty, x);
	#endif
}

void heap_pup(int x, struct heap *h)
{
        int p;
        void *skinny;

	skinny = h->cell[x];
        while ((p = x / 2) >= 1) {
                if (h->cmp(skinny, h->cell[p]) > 0)
                        break;

                h->cell[x] = h->cell[p];
		#ifdef DIJKSTRA
		h->sethi(h->cell[x], x);
		#endif

                x = p;
        }
	h->cell[x] = skinny;
	#ifdef DIJKSTRA
	h->sethi(skinny, x);
	#endif
}

int heap_insert(void *e, struct heap *h)
{
        if (h->last >= h->cap)
                return -1;

        h->cell[++h->last] = e;
	#ifdef DIJKSTRA
	h->sethi(h->cell[h->last], h->last);
	#endif
        heap_pup(h->last, h);

        return 0;
}

void *heap_del(struct heap *h)
{
        void *ret;

        if (h->last <= 0)
                return NULL;

        ret = h->cell[1];
        h->cell[1] = h->cell[h->last--];
	#ifdef DIJKSTRA
	h->sethi(h->cell[1], 1);
	#endif
        heap_pdown(1, h);
        return ret;
}
