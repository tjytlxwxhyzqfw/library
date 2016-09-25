#ifndef _WCC_QUEUE_H
#define _WCC_QUEUE_H

struct queue {
	int cap;
	int first, last;
	void **cell;
};

#define queue_full(q) (((q)->last+1)%((q)->cap) == (q)->first)
#define queue_reset(q) ((q)->first = (q)->last = 0)
#define queue_destroy(q) do{if((q)) free((q)->cell); free((q));}while(0)

/**
 * 初始化一个队列
 * q->last指向一个气泡
 * 气泡与头重合的时候,队列是空的
 * 气泡与头相邻的时候,队列是满的
 *
 * @param cap - 队列容量
 * @return - 指向队列的指针
 */
struct queue *queue_alloc(int cap);

/**
 * 插入
 *
 * @param x
 * @param q
 *
 * 队列满的时候,什么都不做
 */
void queue_append(void *x, struct queue *q);

/**
 * 从队列中取出排头
 *
 * @param q
 * @return - 队列为空的时候返回NULL
 */
void *queue_pop(struct queue *q);

#endif

