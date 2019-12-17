#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#define N 50

int main(int argc,char *argv[ ])
{
        int i,size,myrank,a[N],b[N],sum_temp=0,sum;
        MPI_Init(&argc,&argv);
        MPI_Comm_size(MPI_COMM_WORLD,&size);
        MPI_Comm_rank(MPI_COMM_WORLD,&myrank);

        if(myrank==0)
        {
                for(i=0;i<N;i++)
                {
                    a[i]=1;
                    b[i]=1;
                }
        }

        int ch = N/size;

        int *c = (int *) malloc(sizeof(int) * ch);
        int *d = (int *) malloc(sizeof(int) * ch);
 
        MPI_Scatter(a,ch,MPI_INT,c,ch,MPI_INT,0,MPI_COMM_WORLD);
        MPI_Scatter(b,ch,MPI_INT,d,ch,MPI_INT,0,MPI_COMM_WORLD);

        for(i=0;i<N/size;i++)
        {
            sum_temp += (c[i] * d[i]);
        }

        MPI_Reduce(&sum_temp,&sum,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);

        if(myrank==0)
        {
            printf("%d \n ",sum);
        }
        MPI_Finalize();

        return 0;
}
