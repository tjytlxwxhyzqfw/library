#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "wmatrix.h"

#define ABS(x) ((x) < 0 ? -x : x)

/*
 * U is ignored!
 * This is right for chapter3.exercise7
 */
int jacobi(matrix_t *raw_A, long kmax, long double err, long double *lambda, 
		matrix_t *U)
{
	long k;
	matrix_t *A, *A1;
	int i, j, p, q;
	long double max, sin1, cos1, sinsin, coscos, cossin, sin2, cos2;
	double phi;
	
	A = new_matrixm(raw_A);

again:
	++k;
	printf("k = %d\n", k);
	max = 0.0;
	for (i = 0; i < A->nrow; ++i)
		for (j = 0; j < A->ncol; ++j)
			if ( i != j && max < ABS(A->cell[i][j])) {
				p = i;
				q = j;
				max = ABS(A->cell[i][j]);
			}

	printmat(A);
	printf("p = %d, q = %d, max = %.12lle\n\n", p, q, max);
	
	phi = atan((2 * (double)A->cell[p][q]) / 
			((double)A->cell[p][p] - (double)A->cell[q][q])) / 2;
	sin1 = (long double)sin(phi);
	cos1 = (long double)cos(phi);
	sin2 = sin(2 * phi);
	cos2 = cos(2 * phi);
	sinsin = sin1 * sin1;
	coscos = cos1 * cos1;
	cossin = cos1 * sin1;
	
	A1 = new_matrixm(A);
	A1->cell[p][p] = A->cell[p][p] * coscos + A->cell[q][q] * sinsin
			+ 2 * A->cell[p][q] * cossin;
	A1->cell[q][q] = A->cell[p][p] * sinsin + A->cell[q][q] * coscos
			- 2 * A->cell[p][q] * cossin;
	for (j=0; j < A->ncol; ++j) {
		if( j != p && j != q ) {
			A1->cell[p][j] = A->cell[p][j] * cos1
					+ A->cell[q][j] * sin1;
			A1->cell[j][p] = A1->cell[p][j];
			A1->cell[q][j] = A->cell[q][j] * cos1
					- A->cell[p][j] * sin1;
			A1->cell[j][q] = A1->cell[q][j];
		}
	}
	A1->cell[p][q] = (A->cell[q][q] - A->cell[p][p]) * sin2 / 2
			+ A->cell[p][q] * cos2;
	A1->cell[q][p] = A1->cell[p][q];
	/* A1->cell[i][j] = A1->cell[j][i] = A->cell[i][j] */

	printmat(A1);
	printf("\n");
	freemat(A);
	for (i = 0; i < A1->nrow; ++i)
		for (j = 0; j < A1->ncol; ++j)
			if (k < kmax && i != j && ABS(A1->cell[i][j]) > err) {
				A = A1;
				goto again;
			}
	
	for (i = 0; i < A1->nrow; ++i)
		lambda[i] = A1->cell[i][i];
	
	return 0;
}
	
int main(void)
{
	int i;
	matrix_t *A;
	long kmax = 1000000;
	long double err = 1e-5, *lambda;

	A = new_matrixp("mats/jacobi.mat");
	assert((lambda = malloc(sizeof(long double)*A->nrow)) != NULL);
	jacobi(A, kmax, err, lambda, NULL);
	for (i = 0; i < A->nrow; ++i)
		printf("%.12lle\n", lambda[i]);
	
	return 0;
}
