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


int main(void)
{
	long double err = 1e-12;
	unsigned long kmax = 100000;
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

	int i, j, k, order;
	struct matrix *tmp;
	/* fitting */
	struct matrix *B, *C, *G, *u, *t, *z;
	/* equation */
	struct matrix *sol, *init_x;
	long double zval;

	setbuf(stdout, NULL);

	t = new_matrix(sizeof(ttab) / sizeof(long double), 1);	
	u = new_matrix(sizeof(utab) / sizeof(long double), 1);
	z = new_matrix(t->nrow, u->nrow);
	for (i = 0; i < z->nrow; ++i) {
		t->cell[i][0] = ttab[i];
		u->cell[i][0] = utab[i];
		for (j = 0; j < z->ncol; ++j)
			z->cell[i][j] = ztab[i][j];
	}

	/* fitting */
	order = 4;
	B = new_matrix(t->nrow, order + 1);
	fitting_coefmat_ploy(B, t, order);
	G = new_matrix(u->nrow, order + 1);
	fitting_coefmat_ploy(G, u, order);
	C = new_matrix(B->ncol, G->ncol);
	fitting2(B, G, z, C);
	printf("C = \n");
	printmat(C);

	/* equation group */
	init_x = new_matrix(sizeof(x0) / sizeof(long double), 1);
	for (k = 0; k < init_x->nrow; k++)
		init_x->cell[k][0] = x0[k];
	sol = new_matrix(4, 1);
	for (i = 0; i < 11; ++i) {
		F_x = .08 * i;
		for ( j = 0; j < 21; ++j) {
			F_y = .5 + .05 * j;
			newton_nl(F, jacobiF, init_x, kmax, err, sol);
			//check_report(sol, err);
			zval = calz(sol->cell[0][0], sol->cell[1][0], C);
			printf("%19.12lle = f(%19.12lle, %19.12lle)\n", zval, 
					F_x, F_y);
		}
	}
	for (i = 0; i < 6; ++i)
		for (j = 0; j < 6; ++j) {
			zval = calz(ttab[i], utab[j], C);
			printf("%19.12lle = table(%19.12lle, %19.12lle)\n", zval, 
					ttab[i], utab[j]);
		}
	
	return 0;
}












