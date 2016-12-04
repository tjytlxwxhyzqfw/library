#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "wmatrix.c"

long double F_x, F_y;
void F(const struct matrix *x, struct matrix *y)
{
	long double x1, x2, x3, x4;

	assert(y->nrow == 4 && y->ncol == 1);
	assert(x->nrow == 4 && x->ncol == 1);

	x1 = x->cell[0][0];
	x2 = x->cell[1][0];
	x3 = x->cell[2][0];
	x4 = x->cell[3][0];
	
	y->cell[0][0] = 0.5 * cos(x1) + x2 + x3 + x4 - F_x - 2.67;
	y->cell[1][0] = x1 + 0.5 * sin(x2) + x3 + x4 - F_y - 1.07;
	y->cell[2][0] = 0.5 * x1 + x2 + cos(x3) + x4 - F_x - 3.74;
	y->cell[3][0] = x1 + 0.5 * x2 + x3 + sin(x4) - F_y - 0.79;
}

void jacobiF(const struct matrix *x, struct matrix *jacobi)
{

	assert(jacobi->nrow == 4 && jacobi->ncol == 4);
	assert(x->nrow == 4 && x->ncol == 1);

	jacobi->cell[0][0] = .5 * -1 * sin(x->cell[0][0]);
	jacobi->cell[0][1] = 1;
	jacobi->cell[0][2] = 1;
	jacobi->cell[0][3] = 1;
	jacobi->cell[1][0] = 1;
	jacobi->cell[1][1] = .5 * cos(x->cell[1][0]);
	jacobi->cell[1][2] = 1;
	jacobi->cell[1][3] = 1;
	jacobi->cell[2][0] = .5;
	jacobi->cell[2][1] = 1;
	jacobi->cell[2][2] = -1 * sin(x->cell[2][0]);
	jacobi->cell[2][3] = 1;
	jacobi->cell[3][0] = 1;
	jacobi->cell[3][1] = .5;
	jacobi->cell[3][2] = 1;
	jacobi->cell[3][3] = cos(x->cell[3][0]);
}

void check_report( const struct matrix *vec, long double err){
	static long double rv[4] = {2.67, 1.07, 3.74, 0.79};

	int i;
	long double t, u, v, w, x, y, e[4];

	t = vec->cell[0][0];
	u = vec->cell[1][0];
	v = vec->cell[2][0];
	w = vec->cell[3][0];
	x = F_x;
	y = F_y;

	e[0] = 0.5 * cos(t) + u + v + w - x;
	e[1] = t + 0.5 * sin(u) + v + w - y;
	e[2] = 0.5 * t + u + cos(v) + w - x;
	e[3] = t + 0.5 * u + v + sin(w) - y;
	printf("check: t = %19.12lle, u = %19.12lle\n", t, u);
	printf("sol = \n");
	printmat(vec);
	for (i = 0; i < 4; ++i)
		printf("eq.#%d = %.12lle, err = %.12lle\n", i, e[i], 
				wabs(e[i] - rv[i]));
}

long double calz(long double t, long double u, const struct matrix *c)
{
	int i, j;
	long double z;
	z = 0;
	for (i = 0; i < c->nrow; ++i)
		for (j = 0; j < c->ncol; ++j)
			z += (long double)pow(t, i) * (long double)pow(u, j) 
					* c->cell[i][j];
	return z;
}

long double lgg(long double s, int k, 
		const struct matrix *x, int xb, int xe)
{
	long double xk, xi, bot, top;
	int i;

	assert(xe < x->nrow);

	bot = 1;
	top = 1;
	xk = x->cell[k][0];
	for (i = xb; i <= xe; ++i) {
		xi = x->cell[i][0];
		if (i != k) {
			bot *= xk - xi;
			top *= s - xi;
		}
	}

	return top / bot;
}

long double intf2(long double s, long double t, 
		const struct matrix *x, int xb, int xe, 
		const struct matrix *y, int yb, int ye, 
		const struct matrix *z)
{
	long double ret, lx, ly;
	int i, j;

	ret = 0;
	for (i = xb; i <= xe; ++i)
		for (j = yb; j <= ye; ++j) {
			lx = lgg(s, i, x, xb, xe);
			ly = lgg(t, j, y, yb, ye);
			ret += lx * ly * z->cell[i][j];
		}
	
	return ret;
}

int int_where(long double val, struct matrix *mat, long double h)
{
	int i, ret;
	long double center;

	assert(mat->nrow > 2);

	h /= 2;
	for (i = 1; i < mat->nrow; ++i) {
		center = mat->cell[i][0];
		if ((i == 1 && val <= center + h) 
				|| (i == mat->nrow - 2 && val > center - h)
				|| (val > center - h && val <= center + h)) {
			return i;
		}
	}
	/* error */
	assert(0);
	return -1;
}
int main(void)
{
	long double err = 1e-12;
	unsigned long kmax = 1000000;
	/* equation group */
	long double x0[4] = {1, 1, 1, 1};
	/* fitting */
	long double ttab[6] = {0, 0.2, 0.4, 0.6, 0.8, 1};
	long double utab[6] = {0, 0.4, 0.8, 1.2, 1.6, 2};
	long double ztab[6][6] = {{-.5, -.34, .14, .94, 2.06, 3.5},
			{-.42, -.5, -.26, .3, 1.18, 2.38},
			{-.18, -.5, -.5, -.18, .46, 1.42},
			{.22, -.34, -.58, -.5, -.1, .62},
			{.78, -.02, -.5, -.66, -.5, -.02},
			{1.5, .46, -.26, -.66, -.74, -.5}};

	int i, j, k, order, ti, ui;
	struct matrix *B, *C, *G, *f;
	struct matrix *u, *t, *z, *x, *y;
	struct matrix *sol, *init_x;
	long double zval, tval, uval, xval, yval, fval, pval;
	long double cigma;

	setbuf(stdout, NULL);

	/* 初始化矩阵 */
	t = new_matrix(sizeof(ttab) / sizeof(long double), 1);	
	u = new_matrix(sizeof(utab) / sizeof(long double), 1);
	z = new_matrix(t->nrow, u->nrow);
	for (i = 0; i < z->nrow; ++i) {
		t->cell[i][0] = ttab[i];
		u->cell[i][0] = utab[i];
		for (j = 0; j < z->ncol; ++j)
			z->cell[i][j] = ztab[i][j];
	}
	init_x = new_matrix(sizeof(x0) / sizeof(long double), 1);
	sol = new_matrix(init_x->nrow, init_x->ncol);
	for (i = 0; i < init_x->nrow; ++i)
		init_x->cell[i][0] = x0[i];

	/* 双二次分片插值， 计算f(x,y), for (x,y) in D */
	x = new_matrix(11, 1);
	for (i = 0; i < x->nrow; ++i)
		x->cell[i][0] = .08 * i;
	y = new_matrix(21, 1);
	for (j = 0; j < y->nrow; ++j)
		y->cell[j][0] = .5 + .05 * j;
	f = new_matrix(x->nrow, y->nrow);
	for (i = 0; i < 11; ++i) {
		F_x = .08 * i;
		for( j = 0; j < 21; ++j) {
			F_y = .5 + .05 * j;
			newton_nl(F, jacobiF, init_x, kmax, err, sol);
			tval = sol->cell[0][0];
			uval = sol->cell[1][0];
			ti = int_where(tval, t, .2);
			ui = int_where(uval, u, .4);
			f->cell[i][j] = intf2(tval, uval, t, ti - 1, ti + 1,
					u, ui - 1, ui + 1, z);
		}
	}

	/* output: (2) */
	for (i = 0; i < 11; ++i) {
		xval = .08 * i;
		for( j = 0; j < 21; ++j) {
			yval = .5 + .05 * j;
			fval = f->cell[i][j];
			printf("%2.1llf %2.1llf %19.12lle\n", xval, yval, fval);
		}
	}
	
	/* 最小二乘拟合, 求p */
	order = 0;
next_order:
	B = new_matrix(x->nrow, order + 1);
	fitting_coefmat_ploy(B, x, order);
	G = new_matrix(y->nrow, order + 1);
	fitting_coefmat_ploy(G, y, order);
	C = new_matrix(B->ncol, G->ncol);
	fitting2(B, G, f, C);
	for (cigma = 0, i = 0; i < 11; ++i) {
		xval = .08 * i;
		for( j = 0; j < 21; ++j) {
			yval = .5 + .05 * j;
			pval = calz(xval, yval, C);
			fval = f->cell[i][j];
			cigma += pow(fval - pval, 2);
		}
	}
	
	/* output: (3) */
	printf("order = %2d cigma = %19.12lle\n", order, cigma);
	if (cigma > 1e-7) {
		++order;
		goto next_order;
	}

	/* output: (4) */
	printf("Coef.Mat.\n");
	printmat(C);
	
	/* output: (5) */
	for (i = 1; i <= 8; ++i) {
		F_x = .1 * i;
		for (j = 1; j <= 5; ++j) {
			F_y = .5 + .2 * j;
			newton_nl(F, jacobiF, init_x, kmax, err, sol);
			tval = sol->cell[0][0];
			uval = sol->cell[1][0];
			ti = int_where(tval, t, .2);
			ui = int_where(uval, u, .4);
			fval = intf2(tval, uval, t, ti - 1, ti + 1,
					u, ui - 1, ui + 1, z);
			pval = calz(F_x, F_y, C);
			printf("%2.1llf %2.1llf %19.12lle %19.12lle\n", F_x, 
					F_y, fval, pval);
		}
	}
	
	return 0;
}












