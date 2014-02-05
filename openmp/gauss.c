#include<stdio.h>
#include<stdlib.h>
#include "omp.h"
#include<math.h>
#include<sys/time.h>

#define ERROR 1.0e-6

int main(int argc, char *argv[])
{
  double  *a, *x, *c; 
  int     *loc;
  double  time; 
  double  time_start, time_end;
  struct  timeval tv;
  struct  timezone tz;
  int     i, j, k, picked;
  double  tmp, mag;

  if(argc != 3)
  {
    printf("Usage: %s <num vars> <num threads>\n",argv[0]);
    exit(-1);
  }

  int numVars = atoi(argv[1]);
  int numThreads = atoi(argv[2]);

  int numRows = numVars;
  int numCols = numVars + 1;

  if(numThreads<1)
  {
    printf("There should be atleast 1 thread.\n");
    exit(-1);
  }
  omp_set_num_threads(numThreads);

  a = (double *) malloc(numRows*numCols*sizeof(double));
  c = (double *) malloc(numRows*numCols*sizeof(double));
  loc = (int *) malloc(numRows*sizeof(int));
  x = (double *) malloc(numRows*sizeof(double));

  //Random filling of Matrix
#pragma omp parallel for
  for(i=0; i < numRows*numCols; i++)
  {
    a[i] = 10*drand48(); 
    c[i] = a[i];
  }

  //Begin Gaussian method
  gettimeofday(&tv, &tz);
  time_start = (double)tv.tv_sec + (double)tv.tv_usec / 1000000.0;

  //Assigning initial pivots
#pragma omp parallel for
  for(i=0; i<numRows; i++)
    loc[i] = i;

  for(i=0; i<numRows; i++)
  {
    //Find pivot row picked
    mag=0;
    int local_max,local_pick;
#pragma omp parallel private(local_max,local_pick)
    {
      local_max = 0;
#pragma omp for nowait
      for(j=i; j<numRows; j++)
      {
        if(fabs(a[loc[j]*numCols+i]) > mag)
        {
          local_max = fabs(a[loc[j]*numCols+i]);
          local_pick = j;
        }
      }
      if(local_max > mag)
      {
#pragma omp critical
        if(local_max > mag)
        {
          mag = local_max;
          picked = local_pick;
        }
      }
    }
    int t = loc[i];
    loc[i] = loc[picked];
    loc[picked] = t;

    //Drive to 0 column i elements in unmarked rows
#pragma omp parallel private(j,k,tmp)
    for(j=i+1; j<numRows; j++)
    {
      tmp = a[loc[j]*numCols+i]/a[loc[i]*numCols+i];
#pragma omp for nowait
      for(k=i+1; k<=numRows; k++)
      {
        a[loc[j]*numCols+k] -= (a[loc[i]*numCols+k]*tmp);
      }
    }
  }

  //Back substitution
#pragma omp parallel private(i,j)
  for(i=numRows-1; i>=0; i--)
  {
    x[i] = a[loc[i]*numCols+numRows]/a[loc[i]*numCols+i];
#pragma omp for
    for(j=0; j<i; j++)
      a[loc[j]*numCols+numRows] -= (x[i]*a[loc[j]*numCols+i]);
  }

  //End Gaussian method
  gettimeofday(&tv, &tz);
  time_end = (double)tv.tv_sec + (double)tv.tv_usec / 1000000.0;
  time = time_end - time_start;

  //Check correctness
  for(i=0; i<numRows; i++)
  {
    tmp = 0;
    for(j=0; j<numRows; j++)
    {
      tmp += c[i*numCols+j]*x[j];
    }
    if(fabs(c[i*numCols+numRows]-tmp)>ERROR)
    {
      printf("Error : Target and output result do not match index=%d\ttar=%f\tout=%f\n",i,c[i*numCols+numRows],tmp);
      exit(-1);
    }
  }

  printf("n=%d\tt=%d\tTime=%f s\n",numVars,numThreads,time);

  return 0;
}
