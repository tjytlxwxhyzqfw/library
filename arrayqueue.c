#ifndef _TJYTLXWXHYZQFW_ARRAYQUEUE_C
#define _TJYTLXWXHYZQFW_ARRAYQUEUE_C

#include <assert.h>
#include <stdlib.h>

struct queue {
	int cap;
	int first, last;
	void **cell;
};

/**
 * 初始化一个队列
 * q->last指向一个气泡
 * 气泡与头重合的时候,队列是空的
 * 气泡与头相邻的时候,队列是满的
 *
 * @param cap - 队列容量
 * @return - 指向队列的指针
 */
struct queue *queue_new(int cap)
{
	struct queue *q;

	assert(q = malloc(sizeof(struct queue)));
	q->cap = ++cap;
	q->first = q->last = 0;
	assert(q->cell = malloc(sizeof(void *) * q->cap));

	return q;
}

/**
 * 销毁一个队列
 * 
 * @param q - 要被销毁的队列
 *
 * 注意:销毁堆列前,请先销毁队列中存储的元素
 */
void queue_destroy(struct queue *q)
{
	if (q)
		free(q->cell);
	free(q);
}

/**
 * 插入
 *
 * @param x
 * @param q
 *
 * 队列满的时候,什么都不做
 */
void queue_append(void *x, struct queue *q)
{
	int buble = (q->last + 1) % q->cap;

	/* 邻满 */
	if (buble == q->first)
		return;
	q->cell[q->last] = x;
	q->last = buble;
}

/**
 * 从队列中取出排头
 *
 * @param q
 * @return - 队列为空的时候返回NULL
 */
void *queue_pop(struct queue *q)
{
	void *ret = NULL;

	if (q->first != q->last) {
		ret = q->cell[q->first];
		q->first = (q->first + 1) % q->cap;
	}

	return ret;
}

#endif

