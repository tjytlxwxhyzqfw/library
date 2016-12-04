#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "wmatrix.h"

#define SGN(x) ((x) > 0 ? (1) : (-1))

int hessenberg(matrix_t *raw_A, matrix_t *H)
{
	matrix_t *A, *u, *p, *q, *w, *tmpmat, *tmpmat2;
	long double t, d, c, h;
	int i, j, r;

	A = new_matrixm(raw_A);
	u = new_matrix(A->nrow, 1);
	p = new_matrix(A->nrow, u->ncol);
	q = new_matrix(A->nrow, u->ncol);
	w = new_matrix(u->nrow, u->ncol);
	for (r = 0; r < A->nrow - 2; ++r) {
		printf("\nr = %d\n", r);
		for (i = r + 2; i < A->nrow; ++i)
			if (A->cell[i][r] != 0)
				goto compute;
		continue;
	compute:
		/* d */
		for (d = 0, i = r + 1; i < A->nrow; ++i)
			d += A->cell[i][r] * A->cell[i][r];
		d = sqrt(d);
		/* c */
		c = d;
		if (A->cell[r+1][r] != 0)
			c *= -1 * SGN(A->cell[r+1][r]);
		/* h */
		h = c * c - c * A->cell[r+1][r];
		/* u */
		for (i = 0; i <= r; ++i)
			u->cell[i][0] = 0.0;
		u->cell[r+1][0] = A->cell[r+1][r] - c;
		for (++i; i < A->nrow; ++i)
			u->cell[i][0] = A->cell[i][r];
		/* p */
		transpose(&A);
		mul(p, A, u);
		for (i = 0; i < p->nrow; ++i)
			p->cell[i][0] /= h;
		transpose(&A);
		/* q */
		mul(q, A, u);
		for (i = 0; i < q->nrow; ++i)
			q->cell[i][0] /= h;
		/* t */
		transpose(&p);
		tmpmat = new_matrix(p->nrow, u->ncol);
		mul(tmpmat, p, u);
		t = tmpmat->cell[0][0] / h;
		freemat(tmpmat);
		transpose(&p);
		/* w */
		for (i = 0; i < w->nrow; ++i)
			w->cell[i][0] = q->cell[i][0] - t * u->cell[i][0];
		/* A */
		transpose(&u);
		tmpmat = new_matrix(w->nrow, u->ncol);
		mul(tmpmat, w, u);
		transpose(&u);
		transpose(&p);
		tmpmat2 = new_matrix(u->nrow, p->ncol);
		mul(tmpmat2, u, p);
		transpose(&p);
		for (i = 0; i < A->nrow; ++i)
			for (j = 0; j < A->ncol; ++j)
				A->cell[i][j] -= tmpmat->cell[i][j] + tmpmat2->cell[i][j];
		freemat(tmpmat2);
		freemat(tmpmat);
	}
	printf( "out\n");
	assert(H->nrow == A->nrow && H->ncol == A->ncol);
	for (i = 0; i < A->nrow; ++i)
		for (j = 0; j < A->ncol; ++j)
			H->cell[i][j] = A->cell[i][j];

	freemat(A);
	return 0;
}

int main(void)
{
	matrix_t *A, *H;
	
	setbuf(stdout, NULL);

	A = new_matrixp("mats/hessenberg.mat");
	H = new_matrix(A->nrow, A->ncol);
	hessenberg(A, H);
	printmat(H);

	return 0;
}
