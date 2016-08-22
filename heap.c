#ifndef _TJYTLXWXHYZQFW_HEAP_H
#define _TJYTLXWXHYZQFW_HEAP_H

#include <assert.h>
#include <stdlib.h>

/** 优先队列结构 */
struct heap {
	/**
	 * cap - 优先队列的容量
	 * last - 优先队列中最后一个元素的下标
	 */
        int cap, last;

	/* 指针数组,指向优先队列中的元素 */
        void **cell; 

	/* 比较函数,用于比较队列元素的大小 */
	int (*cmp)(const void *, const void *);
};

/**
 * 创建优先队列
 *
 * @param cap - 优先队列的容量,也是cell数组可用下标的最大值
 * @param cmp - 优先队列中元素的比较方法
 * @return - 指向优先队列结构的指针
 */
struct heap *heap_new(int cap, int (*cmp)(const void *, const void*))
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

        return h;
}

/**
 * 释放优先队列
 *
 * @param h - 要被摧毁的队列
 *
 * 这个过程不会摧毁队列元素,因为队列本身对
 * 存储于其中的元素一无所知
 */
void heap_destroy(struct heap *h)
{
        if (h)
                free(h->cell);
        free(h);
}

/**
 * 下滤
 *
 * @param x - 要被下滤的元素的下标
 * @param h - 指向优先队列的指针
 *
 * <li>如果x的两棵子树是优先队列,那么下滤结束后,以x为根的树是优先队列</li>
 * <li>在最大堆中突然增大的元素或者在最小堆中突然变小的元素需要被下滤</li>
 */
void heap_pdown(int x, struct heap *h)
{
        int c, d;
        void *tmp;

        while ((c = 2 * x) <= h->last) {
                d = c + 1;
                if (d <= h->last && h->cmp(h->cell[d], h->cell[c]) < 0)
                        c = d;
                if (h->cmp(h->cell[x], h->cell[c]) < 0)
                        break;

                tmp = h->cell[c];
                h->cell[c] = h->cell[x];
                h->cell[x] = tmp;

                x = c;
        }
}

/**
 * 上滤
 *
 * @param x - 要被上滤的元素的下标
 * @param h - 指向优先队列的指针
 */
void heap_pup(int x, struct heap *h)
{
        int p;
        void *tmp;

        while ((p = x / 2) >= 1) {
                if (h->cmp(h->cell[p], h->cell[x]) < 0)
                        break;

                tmp = h->cell[x];
                h->cell[x] = h->cell[p];
                h->cell[p] = tmp;

                x = p;
        }
}

/**
 * 插入
 *
 * @param e - 指向被插入元素的指针
 * @param h - 指向优先队列的指针
 * @return 插入结果: 0 - 成功, -1 - 失败
 */
int heap_insert(void *e, struct heap *h)
{
        if (h->last >= h->cap)
                return -1;

        h->cell[++h->last] = e;
        heap_pup(h->last, h);

        return 0;
}

/**
 * 删除最大/最小元
 *
 * @param h - 指向优先队列的指针
 * @return - 指向被删除的元素的指针; NULL - 队列为空
 */
void *heap_del(struct heap *h)
{
        void *ret;

        if (h->last <= 0)
                return NULL;

        ret = h->cell[1];
        h->cell[1] = h->cell[h->last--];
        heap_pdown(1, h);
        return ret;
}

#endif

