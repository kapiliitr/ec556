#include<stdio.h>
#include<stdlib.h>
#include "omp.h"
#include<math.h>
#include<sys/time.h>

void check(int *a, int *b, int n)
{
  int i;
  for(i=2;i<=n;i++)
    if(a[i]!=b[i])
    {
      printf("Error : Serial and OpenMP result do not match index=%d\tser=%d\tpar=%d\n",i,a[i],b[i]);
      exit(-1);
    }
}

int * sieve_parallel(int n)
{
  int i,k,sqrtN;
  int *list;

  list = (int *) malloc((n+1)*sizeof(int));
  sqrtN = (int) sqrt(n);

#pragma omp parallel for
  for(i=2; i<=n; i++) 
  { 
    list[i] = 0; 
  } 

  for(i=2; i<=sqrtN; i++)
  {
    if(!list[i])
    {
#pragma omp parallel for
      for(k=i+1; k<=n; k++)
      {
        if(!(k%i))
          list[k]=1;
      }
    }
  }
  
  return list;
}

int * sieve_serial(int n)
{
  int i,k,sqrtN;
  int *list;

  list = (int *) malloc((n+1)*sizeof(int));
  sqrtN = (int) sqrt(n);

  for(i=2; i<=n; i++) 
  { 
    list[i] = 0; 
  } 

  for(i=2; i<=sqrtN; i++)
  {
    if(!list[i])
    {
      for(k=i+1; k<=n; k++)
      {
        if(!(k%i))
          list[k]=1;
      }
    }
  }
  
  return list;
}


int main(int argc, char *argv[])
{
  int n, numThreads, sqrtN;
  int i,k;
  int *list_ser, *list_par;
  double time_ser, time_par, speedup; 
  double time_start, time_end;
  struct timeval tv;
  struct timezone tz;

  if(argc != 3)
  {
    printf("Usage: %s <max num> <num threads>\n",argv[0]);
    exit(-1);
  }

  n = atoi(argv[1]);
  numThreads = atoi(argv[2]);
 
  gettimeofday(&tv, &tz);
  time_start = (double)tv.tv_sec + (double)tv.tv_usec / 1000000.0;
  list_par = sieve_parallel(n);
  gettimeofday(&tv, &tz);
  time_end = (double)tv.tv_sec + (double)tv.tv_usec / 1000000.0;
  time_par = time_end - time_start;

  gettimeofday(&tv, &tz);
  time_start = (double)tv.tv_sec + (double)tv.tv_usec / 1000000.0;
  list_ser = sieve_serial(n);
  gettimeofday(&tv, &tz);
  time_end = (double)tv.tv_sec + (double)tv.tv_usec / 1000000.0;
  time_ser = time_end - time_start;

  check(list_ser,list_par,n);
  speedup = time_ser/time_par; 
  
  printf("n=%d\tt=%d\tSpeedup=%f\n",n,numThreads,speedup);

  return 0;
}
