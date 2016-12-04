#ifndef _WMATRIX_H
#define _WMATRIX_H

#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define wabs(x) ((x) < 0 ? (-1 * (x)) : (x))
#define sgn(x) ((x) < 0 ? (-1) : 1)

#define DEBUG_CHECK_QR
#define DEBUG_QR_DSD_OUT

typedef struct matrix matrix_t;

matrix_t *new_matrix( int, int );
matrix_t *new_matrixp( char *matrix_file_path );
matrix_t *new_matrixm( const matrix_t* const x );
void freemat( matrix_t *mat );

void ex( int, int, matrix_t* );
void mn( int, long double, matrix_t* );
void ma( int, int, long double, matrix_t* );
/* 列主元素法化行阶梯 */
int rechelon( matrix_t *mat );
long double det( const matrix_t* const matrix );

/* 就地转置 */
int transpose( matrix_t **mat );
int mul( matrix_t *res, const matrix_t* const x, const matrix_t* const y );

/* 列主元素高斯消去法 */
int solve( matrix_t *sol, const matrix_t* const abar );
/* 幂法&反幂法 */
long double powermethod(const matrix_t* const A, const matrix_t* const u0,
				long double errtol, int kmax, int inverse,
				long double *lambda, struct matrix *p);
/* Jacobi方法 */
int jacobi(matrix_t *raw_A, long kmax, long double err, long double *lambda, 
				matrix_t *U);
/* QR分解 */
int qr(const matrix_t *raw_A, long double err, matrix_t *raw_Q, matrix_t *raw_R);
/* 拟上三角化*/
int hessenberg(matrix_t *raw_A, long double err, matrix_t *H);

void printmat( const matrix_t* const m );

/* new 2015-12-10 */
/* matrix */
void reverse(struct matrix *res, const struct matrix *A);
void comlr(struct matrix *abar, const struct matrix *a, const struct matrix *b);
long double norm_vec(const struct matrix *v);

/* fitting */
/*
 * 曲线拟合时，填充基于多项式的系数矩阵
 *
 *     x^0 x^1 x^2 ... x^n
 * x0  .   .   .   .   .
 * x1  .   .   .   .   .
 * x2  .   .   .   .   .
 * .   .   .   .   .   .
 * xm  .   .   .   .   .
 */
void fitting_coefmat_ploy(struct matrix *A, const struct matrix *x, int order);

/* 
 * 曲线拟合
 * 输入： 系数矩阵，函数值向量
 * 输出参数： c， 曲线拟合的系数向量
 */
void fitting(struct matrix *c, const struct matrix *A, const struct matrix *y);
/*
 * 曲面拟合
 * 输入参数： B， 第一个自变量的系数矩阵
 * 输入参数： G， 第二个自变量的系数矩阵
 * 输入参数： U， 函数值矩阵
 * 输出参数： C， 曲面拟合结果的系数矩阵
 */
void fitting2(const struct matrix *B, const struct matrix *G, 
		const struct matrix *U, struct matrix *C);

/* nonlinear equation group */
/*
 * 解非线性方程组， 基于牛顿法
 * 输入参数： F， 函数，与非线性方程组等价的多元向量值函数
 * 输入参数： jacobiF，函数，给出F在指定自变量出的Jacobi矩阵
 * 输入参数： x0, 迭代初值
 * 输入参数： kmax, err
 * 输出参数： result， 非线性方程组的解
 * 使用此函数的用户需基于要带解方程组自行编写F和jacobiF
 */
void newton_nl( void (*F)(const struct matrix*, struct matrix*),
	void (*jacobiF)(const struct matrix*, struct matrix*),
	const struct matrix *x0, long kmax, long double err, 
	struct matrix *result);

struct matrix{
	int nrow, ncol;
	long double **cell;
};

#endif

