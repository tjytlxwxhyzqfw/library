#include <stdio.h>
#include <time.h>

#include "wmatrix.h"

int scal;
int nb;

void
init_globals( void ){
	scal = 9;
	nb = 3;
}

void
print_matrix( matrix_t *mat ){
	int i, j;
	for( i=0; i<mat->nrow; i++ ){
		for( j=0; j<mat->ncol; j++ ){
			printf( "%f ", mat->cell[i][j] );
		}
		printf( "\n" );
	}
}

void
set_matrix( matrix_t *mat ){
	int i, j;
	for( i=0; i<mat->nrow; i++ ){
		for( j=0; j<mat->ncol; j++ ){
			mat->cell[i][j] = i+(double)rand()/RAND_MAX;
		}
	}
}

int
row_echelon( matrix_t *mat ){
	double elim_cof;
	int i, j;
	for( i=0; i<mat->nrow; i++ ){
		if( mat->cell[i][i]==0 ){
			return 1;
		}
		for( j=i+1; j<mat->nrow; j++ ){
			elim_cof = -1.0*mat->cell[j][i]/mat->cell[i][i];
			ma( j, i, elim_cof, mat );
		}
	}
	return 0;
}

int
solution( matrix_t *sol, matrix_t *mat ){
	double sum;
	int i, k, b;

	assert( sol->nrow>=mat->nrow && sol->ncol>=(mat->ncol-mat->nrow) );
	assert( row_echelon(mat)==0 );
	
	/*
	** I'm solving a specific equation set #b
	*/
	for( b=mat->nrow; b<mat->ncol; b++ ){
		/*
		** I begin solving this equation set from the lastest equation
		*/
		//printf( "b=%d\n", b );
		for( i=mat->nrow-1; i>=0; i-- ){
			assert( mat->cell[i][i]!=0 );
			//printf( "i=%d\n", i );
			/*
			** This equation's form is: mat->cell[i][i]*x+sum=mat->cell[i][b], 
			** I'll compute sum.
			*/
			sum = 0.0;
			for( k=mat->nrow-1; k>i; k-- ){
				//printf( "sum += m[%d][%d]*s[%d][%d]=%f*%f\n", 
						//i, k, k, b-mat->nrow, mat->cell[i][k], sol->cell[k][b-mat->nrow] );
				sum += mat->cell[i][k]*sol->cell[k][b-mat->nrow];
			}
			/*Then:
			** x = (mat->cell[i][b]-sum)/mat->cell[i][i];
			** x will be stored in sol->cell[i][b-mat->nrow]
			*/
			//printf( "s[%d][%d]=(m[%d][%d]-sum)/m[%d][%d]=(%f-%f)/%f\n",
					//i, b-mat->nrow, i, b, i, i, mat->cell[i][b], sum, mat->cell[i][i] );
			sol->cell[i][b-mat->nrow] = (mat->cell[i][b]-sum)/mat->cell[i][i];
			printf( "sol=%f\n", sol->cell[i][b-mat->nrow] );
		}
	}
	return 0;
}
			
	

int
main( void ){
	char cmd[10];
	int i, j, k;
	double a, b, c;
	matrix_t *m, *s;
	init_globals();
	srand( time(0) );
	setbuf( stdout, NULL );

	m = new_matrix( scal, scal+nb );
	set_matrix( m );
	print_matrix( m );

	s=new_matrix( scal, nb );
	
	#if 0
	while( 1 ){
		scanf( "%s", cmd );
		if( !strcmp(cmd,"ex") ){
			scanf( "%d%d", &i, &j );
			ex( i, j, m );
			print_matrix( m );
		}else if( !strcmp(cmd,"mn") ){
			scanf( "%d%lf", &i, &a );
			mn( i, a, m );
			print_matrix( m );
		}else if( !strcmp(cmd,"ma") ){
			scanf( "%d%d%lf", &i, &j, &a );
			ma( i, j, a, m );
			print_matrix( m );
		}else{
			perror( "unknown operation" );
			break;
		}
	}
	#endif
	printf( "row echelon form:\n" );
	if( !row_echelon(m) ){
		print_matrix( m );
	}else{
		printf( "cannot now\n" );
	}

	solution( s, m );
	print_matrix( s );
	
	return 0; 
}
