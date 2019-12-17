

#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#define N 50

int main(int argc,char *argv[ ])
{
        int i,size,myrank,a[N],b[N],sum_temp=0,sum,disp1[10],disp2[10],z1[10],z2[10];
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
                disp1[0] = 0;
                disp2[0] = 0;
                for(i = 0; i < size; i++) 
                {
                    z1[i] = i + 2;
                    disp1[i + 1] = disp1[i] + z1[i];
                    z2[i] = i + 2;
                    disp2[i + 1] = disp2[i] + z2[i];
                }
        }

        int ch = N/size;

        int *c = (int *) malloc(sizeof(int) * 2 * ch);
        int *d = (int *) malloc(sizeof(int) * 2 * ch);
 
        MPI_Scatterv(a,z1,disp1,MPI_INT,c,(myrank + 2),MPI_INT,0,MPI_COMM_WORLD);        
        MPI_Scatterv(b,z2,disp2,MPI_INT,d,(myrank + 2),MPI_INT,0,MPI_COMM_WORLD);

        for(i=0;i<(myrank + 2);i++)
        {
            sum_temp += (c[i]*d[i]);
        }

        MPI_Reduce(&sum_temp,&sum,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);

        if(myrank==0)
        {
            printf("%d \n ",sum);
        }
        MPI_Finalize();

        return 0;
}
