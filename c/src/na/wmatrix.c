#include "wmatrix.h"

matrix_t *new_matrix(int n, int m)
{
	int i;
	matrix_t *mat;
	assert((mat = malloc(sizeof(matrix_t))) != NULL);
	mat->nrow = n;
	mat->ncol = m;
	assert((mat->cell = malloc(sizeof(long double*) * n)) != NULL);
	for (i = 0; i < n; ++i) {
		assert((mat->cell[i] = malloc(sizeof(long double) * m)) != NULL);
	}
	return mat;
}

matrix_t *new_matrixp(char *matrix_file_path)
{
	int i, j;
	matrix_t *ret;
	FILE *mf;
	assert((mf = fopen(matrix_file_path, "r")) != NULL);
	fscanf(mf, "%d%d", &i, &j);
	ret = new_matrix(i, j);
	for (i = 0; i < ret->nrow; ++i)
		for (j = 0; j < ret->ncol; ++j) 
			fscanf(mf, "%llf", &ret->cell[i][j]);
	fclose(mf);
	return ret;
}

matrix_t *new_matrixm(const matrix_t* const mat)
{
	int i, j;
	matrix_t *ret;
	ret = new_matrix(mat->nrow, mat->ncol);
	for(i = 0; i < mat->nrow; ++i)
		for(j = 0; j < mat->ncol; ++j)
			ret->cell[i][j] = mat->cell[i][j];
	return ret;
}

void freemat(matrix_t *mat)
{
	int i, j;
	for(i = 0; i < mat->nrow; ++i)
		free(mat->cell[i]);
	free(mat->cell);
	free(mat);
}

void
ex( int i, int j, matrix_t *m ){
	long double *t;
	t = m->cell[i];
	m->cell[i] = m->cell[j];
	m->cell[j] = t;
}

void
mn( int i, long double n, matrix_t *mat ){
	int j;
	for( j=0; j<mat->ncol; j++ ){
		mat->cell[i][j] *= n;
	}
}

void
ma( int i, int j, long double n, matrix_t *mat ){
	int k;
	for( k=0; k<mat->ncol; k++ ){
		mat->cell[i][k] += n*mat->cell[j][k];
	}
}

/*
** 将矩阵mat化为行阶梯型矩阵
** 使用列主元素法(慢!!)
*/
int
rechelon( matrix_t *mat ){
	double elim_cof;
	int i, j, rmax, r;
	//printf( "I'm rechelon\n" );
	for( i=0; i<mat->nrow; i++ ){
		rmax = i;
		for( r=i+1; r<mat->nrow; r++ ){
			if( wabs(mat->cell[r][i]) > wabs(mat->cell[rmax][i]) ){
				rmax = r;
			}
		}
		//printf( "rmax=%d, mat[rmax][i]=%.12lle\n", rmax, mat->cell[rmax][i] );
		ex( i,rmax, mat );
		for( j=i+1; j<mat->nrow; j++ ){
			elim_cof = -1.0*mat->cell[j][i]/mat->cell[i][i];
			ma( j, i, elim_cof, mat );
		}
	}
	return 0;
}

long double
det( const matrix_t* const matrix ){
	matrix_t *mat;
	long double elim_cof, det=1.0;
	int i, j, rmax, r;
	//printf( "I'm rechelon\n" );
	mat = new_matrixm( matrix );
	for( i=0; i<mat->nrow; i++ ){
		rmax = i;
		for( r=i+1; r<mat->nrow; r++ ){
			if( wabs(mat->cell[r][i]) > wabs(mat->cell[rmax][i]) ){
				rmax = r;
			}
		}
		//printf( "rmax=%d, mat[rmax][i]=%.12lle\n", rmax, mat->cell[rmax][i] );
		ex( i,rmax, mat );
		if( i!=rmax ){
			det *= -1.0;
		}
		for( j=i+1; j<mat->nrow; j++ ){
			elim_cof = -1.0*mat->cell[j][i]/mat->cell[i][i];
			ma( j, i, elim_cof, mat );
		}
	}
	for( i=0; i<mat->nrow; i++ ){
		det *= mat->cell[i][i];
	}
	freemat( mat );
	return det;
}
			
/*
** 就地转置
*/
int
transpose( matrix_t **mat ){
	int i, j;
	matrix_t *ret;
	ret = new_matrix( (*mat)->ncol, (*mat)->nrow );
	for( i=0; i<ret->nrow; ++i ){
		for( j=0; j<ret->ncol; ++j ){
			ret->cell[i][j] = (*mat)->cell[j][i];
		}
	}
	freemat(*mat);
	*mat = ret;
	return 0;
}

/*
** 矩阵乘法
** res = x * y
*/
int
mul( matrix_t *res, const matrix_t* const x, const matrix_t* const y ){
	int i, j, k;

	assert( x->ncol == y->nrow );
	assert( x->nrow==res->nrow && y->ncol==res->ncol );

	for( i=0; i<res->nrow; i++ ){
		for( j=0; j<res->ncol; j++ ){
			res->cell[i][j] = 0;
			for( k=0; k<x->ncol; k++ ){
				res->cell[i][j] += x->cell[i][k]*y->cell[k][j];
			}
		}
	}
	return 0;
}

/*
 * 解方程
 * sol = solution(abar)
 */
int
solve( matrix_t *sol, const matrix_t* const abar ){
	double sum;
	int i, k, b;
	matrix_t *abarp;

	abarp = new_matrixm( abar );

	assert( sol->nrow>=abarp->nrow && sol->ncol==(abarp->ncol-abarp->nrow) );
	assert( rechelon(abarp)==0 );
	
	for( b=abarp->nrow; b<abarp->ncol; b++ ){
		for( i=abarp->nrow-1; i>=0; i-- ){
			assert( abarp->cell[i][i]!=0 );
			sum = 0.0;
			for( k=abarp->nrow-1; k>i; k-- ){
				sum += abarp->cell[i][k]*sol->cell[k][b-abarp->nrow];
			}
			sol->cell[i][b-abarp->nrow] = (abarp->cell[i][b]-sum)/abarp->cell[i][i];
		}
	}
	freemat( abarp );
	return 0;
}

/*
 * 幂法和反幂法
 */
long double powermethod(const matrix_t* const A, const matrix_t* const u0,
				long double errtol, int kmax, int inverse,
				long double *lambda, struct matrix *p)
{
	int i, j, k;
	matrix_t *u, *y, *ut, *utu, *yt, *ytu, *Abar;
	long double err, beta, betap, eta;

	static long double beta_guess = 1.0;

	u = new_matrixm(u0);
	y = new_matrix(u->nrow, u->ncol);
	utu = new_matrix(1, 1);
	ytu = new_matrix(1, 1);

	beta = beta_guess;
	err = INT_MAX;
	for (k = 0; k < kmax && err > errtol; ++k) {
		//printf("k=%3d, ", k);

		ut = new_matrixm(u);
		transpose(&ut);
		mul(utu, ut, u);
		eta = sqrt(utu->cell[0][0]);
		freemat(ut);
		//printf("eta=%13.12lle, ", eta);
	
		for (i = 0; i < u->nrow; ++i)
			y->cell[i][0] = u->cell[i][0] / eta;

		if( inverse ){
			Abar = new_matrix( A->nrow, A->ncol+1 );
			for( i=0; i<A->nrow; i++ ){
				for( j=0; j<A->ncol; j++ ){
					Abar->cell[i][j] = A->cell[i][j];
				}
			}
			for( i=0; i<A->ncol; i++ ){
				Abar->cell[i][A->ncol] = y->cell[i][0];
			}
			solve( u, Abar );
			freemat( Abar );
		}else{
			mul( u, A, y );
		}

		yt = new_matrixm( y );
		transpose( &yt );
		mul( ytu, yt, u );
		betap = beta;
		beta = ytu->cell[0][0];
		if( inverse ){
			beta = 1.0/beta;
		}
		freemat( yt );
		//printf( "beta=%13.12lle, ", beta );
		
		err=wabs(beta-betap)/wabs(beta);
		//printf( "err=%13.12lle\n", err );
	}
	*lambda = beta;
	for (i = 0; i < u->nrow; ++i)
		p->cell[i][0] = y->cell[i][0];
	freemat( u );
	freemat( y );
	freemat( utu );
	freemat( ytu );
	return beta;	
}

/*
 * 雅克比方法
 * wcc 2015-11-17
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
			if ( i != j && max < wabs(A->cell[i][j])) {
				p = i;
				q = j;
				max = wabs(A->cell[i][j]);
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
			if (k < kmax && i != j && wabs(A1->cell[i][j]) > err) {
				A = A1;
				goto again;
			}
	
	for (i = 0; i < A1->nrow; ++i)
		lambda[i] = A1->cell[i][i];
	
	return 0;
}

/*
 * qr分解
 * wcc 2015.11.17
 * last modified 2015.11.19
 */
int qr(const matrix_t *raw_A, long double err, matrix_t *res_Q, matrix_t *res_R)
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
		//printf("\nr = %d\n", r);

		for (i = r + 1; i < A->nrow; ++i)
			/*
			 * cause bug:
			 * if (A->cell[i][r] != 0)
			 */
			if (wabs(A->cell[i][r]) > err)
				goto compute;
		continue;
	compute:
		/* d */
		for (d = 0, i = r; i < A->nrow; ++i)
			d += A->cell[i][r] * A->cell[i][r];
		d = sqrt(d);
		/* c */
		c = d;
		/*
		 * won't cause bug:
		 * if (A->cell[r][r] != 0)
		 */
		if (wabs(A->cell[r][r]) > err)
			c *= -1 * sgn(A->cell[r][r]);
		/* h */
		h = c * c - c * A->cell[r][r];
		/* u */
		for (i = 0; i < r; ++i)
			u->cell[i][0] = 0;
		u->cell[r][0] = A->cell[r][r] - c;
		for (++i; i < A->nrow; ++i)
			u->cell[i][0] = A->cell[i][r];
		/* w */
		mul(w, Q, u);
		/* Q */
		transpose(&u);
		tmpmat = new_matrix(w->nrow, u->ncol);
		mul(tmpmat, w, u);
		for (i = 0; i < Q->nrow; ++i)
			for(j = 0; j < Q->ncol; ++j)
				Q->cell[i][j] -= tmpmat->cell[i][j] / h;
		transpose(&u);
		freemat(tmpmat);
		/* p */
		transpose(&A);
		mul(p, A, u);
		for (i = 0; i < p->nrow; i++)
			p->cell[i][0] /= h;
		transpose(&A);
		/* A[r+1] */
		transpose(&p);
		tmpmat = new_matrix(u->nrow, p->ncol);
		mul(tmpmat, u, p);
		for (i = 0; i < A->nrow; ++i)
			for (j = 0; j < A->ncol; ++j)
				A->cell[i][j] -= tmpmat->cell[i][j];
		transpose(&p);
		freemat(tmpmat);
	}
	for (i = 0; i < A->nrow; ++i)
		for ( j = 0; j < A->ncol; ++j){
			res_Q->cell[i][j] = Q->cell[i][j];
			res_R->cell[i][j] = A->cell[i][j];
		}
#ifdef DEBUG_CHECK_QR
	/* Does qr() work ? */
	mul(A, res_Q, res_R);
	tmpmat = new_matrixm(res_Q);
	transpose(&tmpmat);
	mul(Q, tmpmat, res_Q);
	freemat(tmpmat);
	for (i = 0; i < A->nrow; ++i)
		for (j = 0; j < A->ncol; ++j) {
			assert(wabs(A->cell[i][j] - raw_A->cell[i][j]) < err);
			if (i > j)
				assert(wabs(res_R->cell[i][j]) <= err);
			if (i != j)
				assert(wabs(Q->cell[i][j]) <= err);
			else
				assert((wabs(Q->cell[i][j]) - 1) <= err);
		}		
#endif

	freemat(A);
	freemat(Q);
	freemat(u);
	freemat(w);
	freemat(p);
	return 0;
}

/*
 * 拟上三角化
 * wcc 2015-11-18
 */
int hessenberg(matrix_t *raw_A, long double err, matrix_t *H)
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
		for (i = r + 2; i < A->nrow; ++i)
			if (wabs(A->cell[i][r]) > err)
				goto compute;
		continue;
	compute:
		/* d */
		for (d = 0, i = r + 1; i < A->nrow; ++i)
			d += A->cell[i][r] * A->cell[i][r];
		d = sqrt(d);
		/* c */
		c = d;
		if (wabs(A->cell[r+1][r]) > err)
			c *= -1 * sgn(A->cell[r+1][r]);
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
	assert(H->nrow == A->nrow && H->ncol == A->ncol);
	for (i = 0; i < A->nrow; ++i)
		for (j = 0; j < A->ncol; ++j)
			H->cell[i][j] = A->cell[i][j];

	freemat(A);
	return 0;
}

void 
printmat( const struct matrix *const m ){
	int i, j;
	printf( "nrow=%d, ncol=%d\n", m->nrow, m->ncol );
	for( i=0; i<m->nrow; ++i ){
		for( j=0; j<m->ncol; ++j ){
			printf( "%.12lle ", m->cell[i][j] );
		}
		printf( "\n" );
	}
}

void comlr(struct matrix *abar, const struct matrix *a, const struct matrix *b)
{
	int i, j;

	assert(abar->nrow == a->nrow && abar->nrow == b->nrow);
	assert(abar->ncol == a->ncol + b->ncol);
	for (i = 0; i < a->nrow; ++i )
		for (j = 0; j < a->ncol; ++j)
			abar->cell[i][j] = a->cell[i][j];
	for (i = 0; i < b->nrow; ++i)
		for (j = a->ncol; j < abar->ncol; ++j)
			abar->cell[i][j] = b->cell[i][j - a->ncol];
}

void reverse(struct matrix *res, const struct matrix *A)
{
	int i, j;
	struct matrix *E, *abar;

	assert(A->nrow == A->ncol);
	E = new_matrix(A->nrow, A->ncol);
	for (i = 0; i < E->nrow; ++i)
		for (j = 0; j < E->ncol; ++j)
			E->cell[i][j] = (i == j ? 1 : 0);
	abar = new_matrix(A->nrow, A->ncol + E->ncol);
	comlr(abar, A, E);
	solve(res, abar);
	freemat(E);
	freemat(abar);
}

long double norm_vec(const struct matrix *v)
{
	long double max;
	int i;

	max = wabs(v->cell[0][0]);
	for (i = 1; i < v->nrow; ++i)
		if (wabs(max) < wabs(v->cell[i][0]))
			max = wabs(v->cell[i][0]);
	return max;
#if 0
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
#endif
}

void fitting_coefmat_ploy(struct matrix *A, const struct matrix *x, int order)
{
	int i, j;

	assert(A->nrow == x->nrow);
	assert(A->ncol == order + 1);
	for (i = 0; i < A->nrow; ++i)
		for (j = 0; j < A->ncol; ++j)
			A->cell[i][j] = (long double)pow(x->cell[i][0], j);
}

void fitting(struct matrix *c, const struct matrix *A, const struct matrix *y)
{
	struct matrix *At, *a, *b, *abar;

	At = new_matrixm(A);
	a = new_matrix(A->ncol, A->ncol);
	b = new_matrix(A->ncol, y->ncol);
	abar = new_matrix(a->nrow, a->ncol + b->ncol);

	transpose(&At);
	mul(a, At, A);
	mul(b, At, y);
	comlr(abar, a, b);
	solve(c, abar);

	freemat(At);
	freemat(a);
	freemat(b);
	freemat(abar);
}

void fitting2(const struct matrix *B, const struct matrix *G, 
		const struct matrix *U, struct matrix *C)
{
	struct matrix *Bt, *BtB, *BtBr;
	struct matrix *tmp1, *tmp2, *tmp3;
	struct matrix *Gt, *GtG, *GtGr;
	struct matrix *check;
	
	/* Bt = B^T */
	Bt = new_matrixm(B);
	transpose(&Bt);
	/* BtB = Bt * B */
	BtB = new_matrix(Bt->nrow, B->ncol);
	mul(BtB, Bt, B);
	/* BrBr = (BtB)^(-1) */
	BtBr = new_matrix(BtB->nrow, BtB->ncol);
	reverse(BtBr, BtB);
	check = new_matrix(BtBr->nrow, BtBr->ncol);
	mul(check, BtB, BtBr);
	//printf("BtB * BtBr = \n");
	//printmat(check);
	freemat(check);
	/* tmp1 = BtBr * Bt */
	tmp1 = new_matrix(BtBr->nrow, Bt->ncol);
	mul(tmp1, BtBr, Bt);
	/* tmp2 = BtBr * Bt * U */
	tmp2 = new_matrix(tmp1->nrow, U->ncol);
	mul(tmp2, tmp1, U);
	/* tmp3 = BrBr * Bt * U * G*/
	tmp3 = new_matrix(tmp2->nrow, G->ncol);
	mul(tmp3, tmp2, G);
	/* Gt = G^T */
	Gt = new_matrixm(G);
	transpose(&Gt);
	/* GtG = Gt * G*/
	GtG = new_matrix(Gt->nrow, G->ncol);
	mul(GtG, Gt, G);
	/* GtGr = GrG ^ -1 */
	GtGr = new_matrix(GtG->nrow, GtG->ncol);
	reverse(GtGr, GtG);
	check = new_matrix(GtG->nrow, GtG->ncol);
	mul(check, GtG, GtGr);
	//printf("GtG * GtGr = \n");
	//printmat(check);
	freemat(check);
	
	/* C = BtBr * Bt * U * G * GtGr */
	mul(C, tmp3, GtGr);

	freemat(GtGr);
	freemat(GtG);
	freemat(Gt);
	freemat(tmp3);
	freemat(tmp2);
	freemat(tmp1);
	freemat(BtBr);
	freemat(BtB);
	freemat(Bt);
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

