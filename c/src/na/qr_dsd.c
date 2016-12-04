#include "wmatrix.h"

int qtaq(const struct matrix *raw_B, const struct matrix *raw_A, 
				long double err, struct matrix *res_A)
{
	int i, j, r;
	struct matrix *B, *C, *u, *v, *p, *q, *w, *tmp, *tmp2;
	long double d, c, h, t;
	
	B = new_matrixm(raw_B);
	C = new_matrixm(raw_A);
	u = new_matrix(B->nrow, 1);
	v = new_matrix(u->nrow, u->ncol);
	p = new_matrix(u->nrow, u->ncol);
	q = new_matrix(u->nrow, u->ncol);
	w = new_matrix(u->nrow, u->ncol);
	for (r = 0; r < B->nrow - 1; ++r) {
		for (i = r + 1; i < B->nrow; ++i)
			if (wabs(B->cell[i][r]) > err)
				goto compute;
		continue;
	compute:
		/* d */
		for (d = 0, i = r; i < B->nrow; ++i)
			d += B->cell[i][r] * B->cell[i][r];
		d = sqrt(d);
		/* c */
		c = d;
		if (wabs(B->cell[r][r]) > err)
			c *= -1 * sgn(B->cell[r][r]);
		/* h */
		h = c * c - c * B->cell[r][r];
		/* u */
		for (i = 0; i < r; ++i)
			u->cell[i][0] = 0;
		u->cell[r][0] = B->cell[r][r] - c;
		for (++i; i < B->nrow; ++i)
			u->cell[i][0] = B->cell[i][r];
		/* v */
		transpose(&B);
		mul(v, B, u);
		for (i = 0; i < v->nrow; ++i)
			v->cell[i][0] /= h;
		transpose(&B);
		/* B */
		transpose(&v);
		tmp = new_matrix(u->nrow, v->ncol);
		mul(tmp, u, v);
		for (i = 0; i < B->nrow; ++i)
			for (j = 0; j < B->ncol; ++j)
				B->cell[i][j] -= tmp->cell[i][j];
		freemat(tmp);
		transpose(&v);
		/* p */
		transpose(&C);
		mul(p, C, u);
		for (i = 0; i < p->nrow; ++i)
			p->cell[i][0] /= h;
		transpose(&C);
		/* q */
		mul(q, C, u);
		for (i = 0; i < q->nrow; ++i)
			q->cell[i][0] /= h;
		/* t */
		transpose(&p);
		tmp = new_matrix(p->nrow, u->ncol);
		mul(tmp, p, u);
		t = tmp->cell[0][0] / h;
		freemat(tmp);
		transpose(&p);
		/* w */
		for (i = 0; i < w->nrow; ++i)
			w->cell[i][0] = q->cell[i][0] - t * u->cell[i][0];
		/* C */
		transpose(&u);
		tmp = new_matrix(w->nrow, u->ncol);
		mul(tmp, w, u);
		transpose(&u);
		transpose(&p);
		tmp2 = new_matrix(u->nrow, p->ncol);
		mul(tmp2, u, p);
		for (i = 0; i < C->nrow; ++i)
			for (j = 0; j < C->ncol; ++j)
				C->cell[i][j] -= tmp->cell[i][j] 
						+ tmp2->cell[i][j];
		transpose(&p);
		freemat(tmp);
		freemat(tmp2);
	}
	
	/* res_A */
	for (i = 0; i < res_A->nrow; ++i)
		for (j = 0; j < res_A->ncol; ++j)
			res_A->cell[i][j] = C->cell[i][j];
	
	freemat(B);
	freemat(C);
	freemat(u);
	freemat(v);
	freemat(p);
	freemat(q);
	freemat(w);
	return 0;
}

int qr_dsd(struct matrix *raw_A, long double err, long kmax, 
				long double *lambda)
{
	struct matrix *A, *M, *Q, *R, *tmp;
	long double b, c, s, t, s1, s2, sim, delta;
	int i, j, m, k;
	/* counter of lambda */
	int l = 0;

	A = new_matrix(raw_A->nrow, raw_A->ncol);
	M = new_matrix(raw_A->nrow, raw_A->ncol);
	Q = new_matrix(raw_A->nrow, raw_A->ncol);
	R = new_matrix(raw_A->nrow, raw_A->ncol);

	/* step 1 */
	hessenberg(raw_A, err, A);
	printf("hessenberg:\n");
	printmat(A);
	/* step 2 */
	k = 1;
	m = A->nrow-1;
step3:
	assert(m > 0);
	if (wabs(A->cell[m][m - 1]) <= err) {
		lambda[l++] = A->cell[m][m];
		lambda[l++] = 0.0;
		--m;
	} else {
		goto step5;
	}
step4:
	switch (m) {
	case 0:
		lambda[l++] = A->cell[0][0];
		lambda[l++] = 0.0;
	case -1:
		goto step11;
	default:
		goto step3;
	}
step5:
	/* lambda^2 - b * lambda + c = 0 */
	assert(m > 0);
	b = A->cell[m-1][m-1] + A->cell[m][m];
	c = A->cell[m-1][m-1] * A->cell[m][m] 
				- A->cell[m-1][m] * A->cell[m][m-1];
	delta = b * b - 4 * c;
	if (delta < 0){
		s1 = b / 2;
		s2 = b / 2;
		sim = sqrt(wabs(delta)) / 2;
	} else {
		s1 = (b + sqrt(delta)) / 2;
		s2 = (b - sqrt(delta)) / 2;
		sim = 0.0;
	}
	/* step 6 */
	if (m == 1) {
		lambda[l++] = s1;
		lambda[l++] = sim;
		lambda[l++] = s2;
		lambda[l++] = -1 * sim;
		goto step11;
	}
	/*step 7*/
	assert(m > 1);
	if (wabs(A->cell[m-1][m-2]) <= err) {
		lambda[l++] = s1;
		lambda[l++] = sim;
		lambda[l++] = s2;
		lambda[l++] = -1 * sim;
		m -= 2;
		goto step4;
	}
	/* step 8 */
	if (k > kmax)
		/* 没有得到A的全部特征值 */
		goto step11;
	/* step 9 */
	/* s, t */
	assert(m > 0);
	s = A->cell[m - 1][m - 1] + A->cell[m][m];
	t = A->cell[m - 1][m - 1] * A->cell[m][m] 
				- A->cell[m][m - 1] * A->cell[m - 1][m];
	/* M */
	mul(M, A, A);
	for (i = 0; i < A->nrow; ++i) {
		for (j = 0; j < A->ncol; ++j)
			M->cell[i][j] -= s * A->cell[i][j];
		M->cell[i][i] += t;
	}
	/* M = Q * R */
	//qr(M, err, Q, R);
	/* A[k+1] */
	//transpose(&Q);
	//mul(M, Q, A);
	//transpose(&Q);
	//mul(A, M, Q);
	tmp = new_matrixm(A);
	qtaq(M, tmp, err, A);
	freemat(tmp);
	/* step 10 */
	++k;
	goto step3;
step11:
	printf("Final A:\n");
	printmat(A);
	return 0;
}

int main(void)
{
	int i, x, y;
	struct matrix *A, *tmp, *p, *u0;
	long double *lambda;
	
	setbuf(stdout, NULL);

	A = new_matrixp("mats/A2.mat");
	u0 = new_matrixp("mats/A2_u0.mat");
	p = new_matrix(u0->nrow, u0->ncol);
	assert((lambda = malloc(sizeof(long double) * 2 * A->nrow)) != NULL);
	qr_dsd(A, 1e-12, 1000000, lambda);
	for (i = 0; i < 2 * A->nrow; i += 2) {
		printf("%3d: %.12lle, %.12lle\n", i / 2, lambda[i], lambda[i + 1]);
		if (wabs(lambda[i + 1]) > 1e-12)
			continue;
		tmp = new_matrixm(A);
		for (x = 0; x < A->nrow; ++x)
			tmp->cell[x][x] -= lambda[i] + 1e-5;
		powermethod(tmp, u0, 1e-12, 100000, 1, lambda + i + 1, p);
		printmat(p);
	}
	

	return 0;
}
