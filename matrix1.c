
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

	int n , pid, p ;

	int *A, *B, *C;
	
	int tagn = 0 ;
	int tagA = 1 ;
	int tagB = 2 ;
	int tagC = 3 ;

	char processor_name[MPI_MAX_PROCESSOR_NAME];

	MPI_Init(&argc,&argv);

	MPI_Comm_size(MPI_COMM_WORLD, &p); 
	MPI_Comm_rank(MPI_COMM_WORLD, &pid); 
        
	MPI_Status status;

	if (pid ==0 ) {

	

		FILE *fp = fopen("data.txt","r"); 
		
		fscanf ( fp , "%d", & n );
		A = (int*) malloc( n *n * sizeof( int* ));
		B = (int*) malloc( n *n *sizeof( int* ));
		C = (int*) malloc( n *n* sizeof( int* ));
		for ( int r = 0 ; r < n ; r++)
			for ( int c = 0 ; c < n ; c++ )
				fscanf ( fp , "%d", & A[r*n + c] );	


		for ( int r = 0 ; r < n ; r++)
			for ( int c = 0 ; c < n ; c++ )
				fscanf ( fp , "%d", & B[r*n + c] );	
	

		fclose (fp);

	
		int offset = n / p * n ; 

		for ( int dest = 1 ; dest < p ; dest ++ ) 
		{
			

			MPI_Send(& n , 1 , MPI_INT, dest , tagn, MPI_COMM_WORLD);
			MPI_Send(& A[offset], n / p * n , MPI_INT, dest , tagA, MPI_COMM_WORLD);
			MPI_Send(& B[0] , n * n , MPI_INT, dest , tagB, MPI_COMM_WORLD);
			printf("Sent %d elements to task %d offset= %d\n",n*n/p,dest,offset);
			offset += n / p * n ;		
		}


		
		for ( int i=0 ; i< (n/p) ; i++ )
			for ( int j = 0 ; j<n ; j++ )
			{			
				C[i*n+j] = 0 ;	
				for ( int k = 0 ; k<n ; k++ )
					C[i*n+j] += A[i*n+k]*B[k*n+j];
				
			}


		offset = n / p * n ;
		
		for ( int dest = 1 ; dest < p ; dest ++ ) 
		{

			MPI_Recv(&C[offset] , n / p * n, MPI_INT, dest, tagC, MPI_COMM_WORLD, &status);
			printf("Receive %d elements from task %d offset= %d\n",n*n/p,dest,offset);
			offset += n / p * n ;		
		}

		printf ("The output is \n") ;

		for ( int i=0 ; i< n ; i++ ){
			for ( int j = 0 ; j<n ; j++ )	
				printf ( "%d  " , C[i*n+j] ) ;	
			printf ("\n");
		}

		FILE *out = fopen("matrix_result.txt","w");
		if (out == NULL)
		{
    			printf("Error opening file!\n");
    			exit(1);
		}

		for ( int i=0 ; i< n ; i++ ){
			for ( int j = 0 ; j<n ; j++ )	
				fprintf ( out , "%d  " , C[i*n+j] ) ;	
			fprintf (out , "\n");
		}

		
		fprintf ( out , "Thank you ") ;
		fclose ( out) ;

	}

	if ( pid != 0 )
	{

		MPI_Recv(&n, 1, MPI_INT, 0, tagn , MPI_COMM_WORLD, &status);
		A = (int*) malloc( n/p * n * sizeof( int* ));
		B = (int*) malloc( n * n* sizeof( int* ));
		C = (int*) malloc( n /p *n * sizeof( int* ));


		MPI_Recv(&A[0] , n / p * n, MPI_INT, 0, tagA, MPI_COMM_WORLD, &status);
		MPI_Recv(&B[0] , n * n, MPI_INT, 0, tagB, MPI_COMM_WORLD, &status);

		for ( int i=0 ; i< (n/p) ; i++ )
			for ( int j = 0 ; j<n ; j++ )
			{			
				C[i*n+j] = 0 ;	
				for ( int k = 0 ; k<n ; k++ )
					C[i*n+j] += A[i*n+k]*B[k*n+j];
				

			}

		MPI_Send(& C[0] , n * n /p , MPI_INT, 0 , tagC, MPI_COMM_WORLD);


	}

	MPI_Finalize();

	return 0;
}

