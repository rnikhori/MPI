#include<stdio.h>
#include<time.h>
#include<unistd.h>
double f(double a)
{
	double result = 1/(1+(a*a));
	return(result);
}
int main()
{
	int n=10000,i;
	double a,b,h,approx,y;
	printf("enter the endpoints of the curve");
	scanf("%lf %lf",&a,&b);
	h = (b-a)/n; 
	clock_t start, end;
    double cpu_time_used;
	start = clock();

					
					approx = (f(a) + f(b))/2.0; 
					for (i = 1; i <= n-1; i++) 
						{ 
							y = a + i*h; 
							approx += f(y); 
						} 
						approx = h*approx;

						printf("The area under the given curve is%f\n",approx );









						end = clock();
     					cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
						printf("Time elasped is seconds%f\n",cpu_time_used);

}
