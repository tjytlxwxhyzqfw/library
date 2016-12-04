#include <math.h>
#include <limits.h>
#include "wmatrix.c"

#define PM_INVERSE 1
#define MU 39

long double g_errtol = 1e-12;
int g_kmax = 1000;
long double g_beta=1.0;


long double
powermethod( const matrix_t* const A, const matrix_t* const u0, long double errtol, int kmax, int inverse ){
	int i, j, k;
	matrix_t *u, *y, *ut, *utu, *yt, *ytu, *Abar;
	long double err, beta, betap, eta;

	static long double beta_guess = 1.0;

	u = new_matrixm( u0 );
	y = new_matrix( u->nrow, u->ncol );
	utu = new_matrix( 1, 1 );
	ytu = new_matrix( 1, 1 );

	beta = beta_guess;
	err = INT_MAX;
	for( k=0; k<kmax && err>errtol; k++ ){
		printf( "k=%3d, ", k );

		ut = new_matrixm( u );
		transpose( &ut );
		mul( utu, ut, u );
		eta = sqrt( utu->cell[0][0] );
		freemat( ut );
		printf( "eta=%13.12lle, ", eta );
	
		for( i=0; i<u->nrow; i++ ){
			y->cell[i][0] = u->cell[i][0]/eta;
		}

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
		printf( "beta=%13.12lle, ", beta );
		
		err=wabs(beta-betap)/wabs(beta);
		printf( "err=%13.12lle\n", err );
	}
	freemat( u );
	freemat( y );
	freemat( utu );
	freemat( ytu );
	return beta;
		
}

int
main( void ){
	int i, j, k;
	matrix_t *A, *Ap, *u0;
	long double lambda, lambda1, lambda501, lambdas, mu, lambdai[MU];

	setbuf( stdout, NULL );

	A = new_matrixp( "A.mat" );
	u0 = new_matrixp( "u0.mat" );

	lambda1 = powermethod( A, u0, g_errtol, g_kmax, !PM_INVERSE );

	Ap = new_matrixm( A );
	for( i=0; i<Ap->nrow; i++ ){
		Ap->cell[i][i] -= lambda1;
	}
	lambda501 = lambda1+powermethod( Ap, u0, g_errtol, g_kmax, !PM_INVERSE );
	freemat( Ap );
	if( lambda1>lambda501 ){
		lambda = lambda1;
		lambda1 = lambda501;
		lambda501 = lambda;
	}

	lambdas = powermethod( A, u0, g_errtol, g_kmax, PM_INVERSE );

	for( k=0; k<MU; k++ ){
		mu = lambda1+(k+1)*(lambda501-lambda1)/40;
		Ap = new_matrixm( A );
		for( i=0; i<Ap->nrow; i++ ){
			Ap->cell[i][i] -= mu;
		}
		lambdai[k] = mu+powermethod( Ap, u0, g_errtol, g_kmax, PM_INVERSE );
		printf( "lambdai[%2d]=%13.12lle\n", k+1, lambdai[k] );
		freemat( Ap );
	}
	
	printf( "lambda1=%13.12lle\nlambda501=%13.12lle\nlambdas=%13.12lle\n", lambda1, lambda501, lambdas );
	for( k=0; k<MU; k++ ){
		printf( "lambdai[%d]=%13.12lle\n", k+1, lambdai[k] );
	}
	printf( "cond(A)=%13.12lle\n", (wabs(lambda1)>wabs(lambda501)?wabs(lambda1):wabs(lambda501))/wabs(lambdas) );
	printf( "det(A)=%13.12lle\n", det(A) );

	freemat( A );
	freemat( u0 );

	return 0;
}












		
