#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "wmatrix.h"

long double F_t, F_u;

long double norm_vec(const struct matrix *v)
{
	struct matrix *t, *res;
	long double n;

	t = new_matrixm(v);
	transpose(&t);
	res = new_matrix(t->nrow, v->ncol);
	mul(res, t, v);
	n = res->cell[0][0];

	freemat(t);
	freemat(res);
	return wabs(n);
}

void F(const struct matrix *x, struct matrix *y)
{
	long double x1, x2, x3, x4;

	assert(y->nrow == 4 && y->ncol == 1);
	assert(x->nrow == 4 && x->ncol == 1);

	x1 = x->cell[0][0];
	x2 = x->cell[1][0];
	x3 = x->cell[2][0];
	x4 = x->cell[3][0];
	
	y->cell[0][0] = -1 * x1 + x3 + x4 + .5 * cos(F_t) + F_u - 2.67;
	y->cell[1][0] = -1 * x2 + x3 + x4 + F_t + .5 * sin(F_u) - 1.07;
	y->cell[2][0] = -1 * x1 + cos(x3) + x4 + .5 * F_t + F_u - 3.74;
	y->cell[3][0] = -1 * x2 + x3 + sin(x4) + F_t + 0.5 * F_u - 0.79;
}

void jacobiF(const struct matrix *x, struct matrix *jacobi)
{
	assert(jacobi->nrow == 4 && jacobi->ncol == 4);
	assert(x->nrow == 4 && x->ncol == 1);

	jacobi->cell[0][0] = -1;
	jacobi->cell[0][1] = 0;
	jacobi->cell[0][2] = 1;
	jacobi->cell[0][3] = 1;
	jacobi->cell[1][0] = 0;
	jacobi->cell[1][1] = -1;
	jacobi->cell[1][2] = 1;
	jacobi->cell[1][3] = 1;
	jacobi->cell[2][0] = -1;
	jacobi->cell[2][1] = 0;
	jacobi->cell[2][2] = -1 * sin(x->cell[2][0]);
	jacobi->cell[2][3] = 1;
	jacobi->cell[3][0] = 0;
	jacobi->cell[3][1] = -1;
	jacobi->cell[3][2] = 1;
	jacobi->cell[3][3] = cos(x->cell[3][0]);
}

void newton_nl( void (*F)(const struct matrix*, struct matrix*),
	void (*jacobiF)(const struct matrix*, struct matrix*),
	const struct matrix *x0, long kmax, long double err, 
	struct matrix *result)
{
	struct matrix *x, *y, *dltx, *jac, *abar;
	long i, j, k;
	
	x = new_matrixm(x0);
	y = new_matrix(x->nrow, x->ncol);
	dltx = new_matrix(x->nrow, x->ncol);
	jac = new_matrix(x->nrow, x->nrow);
	abar = new_matrix(jac->nrow, jac->ncol + 1);
	
	for (k = 0; k < kmax; ++k) {
		/* y = F(x), jacobi = F'(x) */
		F(x, y);
		jacobiF(x, jac);
		/* F'(x) * dltx = -F(x) */
		for (i = 0; i < jac->nrow; ++i)
			for (j = 0; j < jac->ncol; ++j)
				abar->cell[i][j] = jac->cell[i][j];
		for (i = 0; i < y->nrow; ++i)
			abar->cell[i][jac->ncol] = -1 * y->cell[i][0];
		solve(dltx, abar);
		/* ||dltx||/||x|| <= err ? */
		if (norm_vec(dltx) / norm_vec(x) <= err)
			goto out;
		/* x += dltx */
		for (i = 0; i < x->nrow; ++i)
			x->cell[i][0] += dltx->cell[i][0];
	}
out:
	for (i = 0; i < x->nrow; ++i)
		result->cell[i][0] = x->cell[i][0];
	freemat(x);
	freemat(y);
	freemat(dltx);
	freemat(jac);
	freemat(abar);
}

void check_report( const struct matrix *vec, long double err){
	static long double rv[4] = {2.67, 1.07, 3.74, 0.79};

	int i;
	long double t, u, v, w, x, y, e[4];

	t = F_t;
	u = F_u;
	v = vec->cell[2][0];
	w = vec->cell[3][0];
	x = vec->cell[0][0];
	y = vec->cell[1][0];
	e[0] = 0.5 * cos(t) + u + v + w - x;
	e[1] = t + 0.5 * sin(u) + v + w - y;
	e[2] = 0.5 * t + u + cos(v) + w - x;
	e[3] = t + 0.5 * u + v + sin(w) - y;
	printf("check: t = %llf, u = %llf\n", t, u);
	for (i = 0; i < 4; ++i)
		printf("eq.#%d = %.12lle, err = %.12lle\n", i, e[i], 
				wabs(e[i] - rv[i]));
}

int main(void)
{
	static long double err = 1e-12;
	static unsigned long kmax = 100000;
	static long double x0[6] = {1, 1, 1, 1};
	static long double utab[6] = {0, 0.4, 0.8, 1.2, 1.6, 2};
	static long double ttab[6] = {0, 0.2, 0.4, 0.6, 0.8, 1};
	static long double ztab[6][6] = {{-.5, -.34, .14, .94, 2.06, 3.5},
			{-.42, -.5, -.26, .3, 1.18, 2.38},
			{-.18, -.5, -.5, -.18, .46, 1.42},
			{.22, -.34, -.58, -.5, -.1, .62},
			{.78, -.02, -.5, -.66, -.5, -.02},
			{1.5, .46, -.26, -.66, -.74, -.5}};

	struct matrix *x, *init_x;
	int i, j, k, ui, ti;
	long double xtab[6][6], ytab[6][6];

	x = new_matrix(4, 1);
	init_x = new_matrix(x->nrow, x->ncol);
	for ( ti = 0; ti < 6; ++ti) {
		F_t = ttab[ti];
		for ( ui = 0; ui < 6; ++ui) {
			F_u = utab[ui];
			for (i = 0; i < init_x->nrow; ++i)
				init_x->cell[i][0] = x0[i];
			newton_nl(F, jacobiF, init_x, kmax, err, x);
			xtab[ti][ui] = x->cell[0][0];
			ytab[ti][ui] = x->cell[1][0];
			printf("%6.3llf = f(%19.12lle, %19.12lle)\n", ztab[ti][ui], 
					xtab[ti][ui], ytab[ti][ui]);
		}
	}

	return 0;
}
