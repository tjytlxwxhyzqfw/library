#ifndef _WCC_HEAP_H
#define _WCC_HEAP_H

#define DIJKSTRA

/** 
 * 优先队列结构
 * cap - 优先队列的容量
 * last - 优先队列中最后一个元素的下标
 * cell - 指针数组,指向优先队列中的元素
 * cmp - 比较函数,用于比较队列元素的大小
 * sethi - 设置一个元素在cell中的下标
 */
struct heap {
        int cap, last;
        void **cell; 
	int (*cmp)(const void *, const void *);
	#ifdef DIJKSTRA
	void (*sethi)(void *, const int);
	#endif
};

/**
 * 创建优先队列
 *
 * @param cap - 优先队列的容量,也是cell数组可用下标的最大值
 * @param cmp - 优先队列中元素的比较方法
 * @return - 指向优先队列结构的指针
 */
struct heap *heap_alloc(int cap, int (*cmp)(const void *, const void*)
	#ifdef DIJKSTRA
	, void (*set)(void *, int)
	#endif
);

/**
 * 释放优先队列
 *
 * @param h - 要被摧毁的队列
 *
 * 这个过程不会摧毁队列元素,因为队列本身对
 * 存储于其中的元素一无所知
 */
void heap_destroy(struct heap *h);

/**
 * 下滤
 *
 * @param x - 要被下滤的元素的下标
 * @param h - 指向优先队列的指针
 *
 * <li>如果x的两棵子树是优先队列,那么下滤结束后,以x为根的树是优先队列</li>
 * <li>在最大堆中突然增大的元素或者在最小堆中突然变小的元素需要被下滤</li>
 */
void heap_pdown(int x, struct heap *h);

/**
 * 上滤
 *
 * @param x - 要被上滤的元素的下标
 * @param h - 指向优先队列的指针
 */
void heap_pup(int x, struct heap *h);

/**
 * 插入
 *
 * @param e - 指向被插入元素的指针
 * @param h - 指向优先队列的指针
 * @return 插入结果: 0 - 成功, -1 - 失败
 */
int heap_insert(void *e, struct heap *h);

/**
 * 删除最大/最小元
 *
 * @param h - 指向优先队列的指针
 * @return - 指向被删除的元素的指针; NULL - 队列为空
 */
void *heap_del(struct heap *h);

#endif

