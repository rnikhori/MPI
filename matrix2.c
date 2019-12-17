#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

        int n , pid, p ;

        int *sendA, *sendB, *finalC , *recA , *recC ;

        int tagn = 0 ;
        int tagA = 1 ;
        int tagB = 2 ;
        int tagC = 3 ;

        char processor_name[MPI_MAX_PROCESSOR_NAME];

        MPI_Init(&argc,&argv);

        MPI_Comm_size(MPI_COMM_WORLD, &p); 
        MPI_Comm_rank(MPI_COMM_WORLD, &pid); 

        MPI_Status status;

        MPI_Comm comm1 , comm2;
        if (pid ==0 ) {

                FILE *fp = fopen("data.txt","r");

                fscanf ( fp , "%d", & n );
                printf ("rows is %d \n", n );

                sendA = (int*) malloc( n *n * sizeof( int* ));
                sendB = (int*) malloc( n *n *sizeof( int* ));
                finalC = (int*) malloc( n *n* sizeof( int* ));


                for ( int r = 0 ; r < n ; r++)
                        for ( int c = 0 ; c < n ; c++ )
                        {
                                fscanf ( fp , "%d", & sendA[r*n + c] );
            
                        }
                for ( int r = 0 ; r < n ; r++)
                        for ( int c = 0 ; c < n ; c++ )
                                fscanf ( fp , "%d", & sendB[r*n + c] );


                fclose (fp);
                printf (" process 0 send create buffer \n " ) ;

        }


	

	MPI_Barrier(MPI_COMM_WORLD); 
	double start = MPI_Wtime(); 
        MPI_Bcast ( &n, 1 , MPI_INT , 0 , MPI_COMM_WORLD ) ;
        printf ("  n / p is %d \n " , n/p  ) ;
        recA = (int*) malloc( n/p * n * sizeof( int* ));
	if (sendB == NULL ) 
         {
          printf ("NULL\n");
          sendB = (int*) malloc( n * n* sizeof( int* ));
         }
          recC = (int*) malloc( n /p *n * sizeof( int* ));
        printf (" process %d create rec buffer \n " , pid  ) ;

        MPI_Scatter( sendA , n * n/p , MPI_INT , recA , n * n/p , MPI_INT , 0, MPI_COMM_WORLD ) ;

        for (int i=0; i < (n/p) ; i++)
            printf (" process %d A [%d] = % d \n " , pid , i , recA[i] ) ;
        
        MPI_Bcast ( sendB, n*n , MPI_INT , 0 , MPI_COMM_WORLD ) ;

        for (int i=0; i < (n*n) ; i++)
		printf (" process %d sendB [%d] = % d \n " , pid , i , sendB[i] ) ;		
        for ( int i=0 ; i< (n/p) ; i++ )
                for ( int j = 0 ; j<n ; j++ )
                {
                        recC[i*n+j] = 0 ;
                        for ( int k = 0 ; k<n ; k++ )
                                recC[i*n+j] += recA[i*n+k]*sendB[k*n+j];
						printf (" process %d recC [%d] = % d \n" , pid , i*n+j , recC[i*n+j] ) ;
                        
				}
		
	
        MPI_Gather( recC , n*n/p , MPI_INT , finalC , n*n/p , MPI_INT , 0, MPI_COMM_WORLD ) ;


			
	MPI_Barrier(MPI_COMM_WORLD); 
	double end = MPI_Wtime(); 
	
	double time = end - start ;
	printf (" ********** process %d : time = % f \n" , pid , time ) ;
		
	
		
        if (pid == 0) {
		
			FILE *out = fopen("matrix_result.txt","w");
			if (out == NULL)
			{
					printf("Error opening file!\n");
					exit(1);
			}

			for (int i=0; i < n ; i++)
			{
				for (int j=0; j < n ; j++)
					fprintf ( out ," %d " , finalC[i*n+j] ) ;
				fprintf ( out , "\n") ;	
			}
			fclose ( out) ;
		}

        MPI_Finalize();

	return 0;
}
