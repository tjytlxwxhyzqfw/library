#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "wmatrix.h"

#define ABS(x) ((x) < 0 ? (-x) : x)
#define SGN(x) ((x) < 0 ? (-1) : 1)

int qr(matrix_t *raw_A, matrix_t *raw_Q, matrix_t *raw_R)
{
	int i, j, r;
	matrix_t *A, *Q, *u, *w, *p, *tmpmat;
	long double d, c, h, tmp;

	A = new_matrixm(raw_A);
	Q = new_matrix(A->nrow, A->ncol);
	for (i = 0; i < Q->nrow; ++i)
		for (j = 0; j < Q->ncol; ++j)
			Q->cell[i][j] = ((i == j) ? 1 : 0);
	u = new_matrix(A->nrow, 1);
	w = new_matrix(Q->nrow, u->ncol);
	p = new_matrix(A->ncol, u->ncol);

	for (r = 0; r < A->nrow; ++r) {
		printf("\nr = %d\n", r);
		//printf("A:\n");
		//printmat(A);
		//printf("Q:\n");
		//printmat(Q);
		for (i = r + 1; i < A->nrow; ++i)
			if (A->cell[i][r] != 0)
				goto compute;
		continue;
	compute:
		printf("computing...\n");
		/* d[r] = cigma(a[i][r]^2) */
		for (d = 0, i = r; i < A->nrow; ++i)
			d += A->cell[i][r] * A->cell[i][r];
		d = sqrt(d);
		printf("d = %.12lle\n", d);
		/* cr = -sgn(a[r][r])*d[r] */
		c = d;
		if (A->cell[r][r] != 0)
			c *= -1 * SGN(A->cell[r][r]);
		printf("c = %.12lle\n", c);
		/* h[r] = c[r]^2 - c[r]*a[r][r](r) */
		h = c * c - c * A->cell[r][r];
		/* 
		 * u[r] = 
		 * (0, ..., 0, a[r][r] - c[r], a[r+1][r], ..., a[n][r]).T
		 */
		for (i = 0; i < r; ++i)
			u->cell[i][0] = 0;
		u->cell[r][0] = A->cell[r][r] - c;
		printf("u ");
		for (++i; i < A->nrow; ++i)
			u->cell[i][0] = A->cell[i][r];
		/* w[r] = Q[r]*u[r] */
		mul(w, Q, u);
		printf("w ");
		/* Q[r+1] = Q[r] - w[r]*u[r].T/h[r] */
		transpose(&u);
		tmpmat = new_matrix(w->nrow, u->ncol);
		mul(tmpmat, w, u);
		for (i = 0; i < Q->nrow; ++i)
			for(j = 0; j < Q->ncol; ++j)
				Q->cell[i][j] -= tmpmat->cell[i][j] / h;
		transpose(&u);
		freemat(tmpmat);
		printf("Q ");
		/* p[r] = A[r].T*u[r] / h[r] */
		transpose(&A);
		mul(p, A, u);
		for (i = 0; i < p->nrow; i++)
			p->cell[i][0] /= h;
		transpose(&A);
		printf("p ");
		/* A[r+1] = A[r] - u[r] * p[r].T */
		transpose(&p);
		tmpmat = new_matrix(u->nrow, p->ncol);
		mul(tmpmat, u, p);
		for (i = 0; i < A->nrow; ++i)
			for (j = 0; j < A->ncol; ++j)
				A->cell[i][j] -= tmpmat->cell[i][j];
		transpose(&p);
		freemat(tmpmat);
		printf("A\n");
	}
	printf("out\n");
	for (i = 0; i < A->nrow; ++i)
		for ( j = 0; j < A->ncol; ++j){
			raw_Q->cell[i][j] = Q->cell[i][j];
			raw_R->cell[i][j] = A->cell[i][j];
		}

	freemat(A);
	freemat(Q);
	freemat(u);
	freemat(w);
	freemat(p);
	return 0;
}

int main(void)
{
	matrix_t *A, *Q, *R;
	
	setbuf(stdout, NULL);

	A = new_matrixp("mats/A.mat");
	Q = new_matrix(A->nrow, A->ncol);
	R = new_matrix(A->nrow, A->ncol);
	qr(A, Q, R);
	printf("check:\n");
	printf("Q*R\n");
	mul(A, Q, R);
	printmat(A);
	printf("Q*Q.T\n");
	freemat(R);
	R = new_matrixm(Q);
	transpose(&R);
	mul(A, Q, R);
	printmat(A);

	freemat(A);
	freemat(Q);
	freemat(R);
	return 0;
}
