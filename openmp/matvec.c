#include<stdio.h>
#include<stdlib.h>
#include "omp.h"
#include<math.h>
#include<sys/time.h>

#define ERROR 1.0e-9

void check(double *a, double *b, int len)
{
  int i;
  for(i=0;i<len;i++)
    if(fabs(a[i]-b[i])>ERROR)
    {
      printf("Error : Serial and OpenMP result do not match index=%d\tser=%f\tpar=%f\n",i,a[i],b[i]);
      exit(-1);
    }
}

int main(int argc, char *argv[])
{
  double *a, *b, *c_ser, *c_par;
  double time_ser, time_par, speedup; 
  double time_start, time_end;
  struct timeval tv;
  struct timezone tz;
  int i, j;

  if(argc !=4)
  {
    printf("Usage: ./matvec <rows> <cols> <threads>\n");
    exit(-1);
  }

  int numRows = atoi(argv[1]);
  int numCols = atoi(argv[2]);
  int numThreads = atoi(argv[3]);

  if(numThreads<1)
  {
    printf("There should be atleast 1 thread.\n");
    exit(-1);
  }
  omp_set_num_threads(numThreads);

  a = (double *) malloc(numRows*numCols*sizeof(double));
  b = (double *) malloc(numCols*sizeof(double));
  c_ser = (double *) malloc(numRows*sizeof(double));
  c_par = (double *) malloc(numRows*sizeof(double));

  //Random filling of Matrix
  #pragma omp parallel for schedule(static)
  for(i=0; i < numRows*numCols; i++)
    a[i] = drand48();

  #pragma omp parallel for schedule(static)
  for(i=0; i < numCols; i++)
    b[i] = drand48();

  //Parallel
  gettimeofday(&tv, &tz);
  time_start = (double)tv.tv_sec + (double)tv.tv_usec / 1000000.0;
  #pragma omp parallel for private(i,j) schedule(static)
  for(i=0; i<numRows; i++)
  {
    c_par[i] = 0;
    for(j=0; j<numCols; j++)
      c_par[i] = c_par[i] + a[i*numCols+j]*b[j];
  }
  gettimeofday(&tv, &tz);
  time_end = (double)tv.tv_sec + (double)tv.tv_usec / 1000000.0;
  time_par = time_end - time_start;

  //Serial
  gettimeofday(&tv, &tz);
  time_start = (double)tv.tv_sec + (double)tv.tv_usec / 1000000.0;
  for(i=0; i<numRows; i++)
  {
    c_ser[i] = 0;
    for(j=0; j<numCols; j++)
      c_ser[i] = c_ser[i] + a[i*numCols+j]*b[j];
  }
  gettimeofday(&tv, &tz);
  time_end = (double)tv.tv_sec + (double)tv.tv_usec / 1000000.0;
  time_ser = time_end - time_start;

  check(c_ser,c_par,numRows);
  speedup = time_ser/time_par; 
  
  printf("n=%d\tt=%d\tSpeedup=%f\n",numCols,numThreads,speedup);

  return 0;
}
