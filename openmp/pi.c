#include <stdio.h> 
#include <stdlib.h> 
#include "omp.h"
#include <math.h>
#include <limits.h>

main(int argc, char *argv[]) 
{ 
  int i, count; /* points inside the unit quarter circle */ 
  unsigned short xi[3]; /* random number seed */ 
  int samples; /* samples Number of points to generate */ 
  double x,y; /* Coordinates of points */ 
  double pi; /* Estimate of pi */
  double error; /* Percentage error of the calculated value  */

  if(argc != 3)
  {
    printf("Usage: %s <num_samples> <num_threads>\n",argv[0]);
    exit(-1);
  }

  samples = atoi(argv[1]); 
  omp_set_num_threads (atoi(argv[2]));

  count = 0;
  #pragma omp parallel private(xi,x,y) 
  { 
    xi[0] = 1; /* These statements set up the random seed */ 
    xi[1] = 1; 
    xi[2] = omp_get_thread_num();

    #pragma omp for reduction(+:count) 
    for (i = 0; i < samples; i++) 
    { 
      x = erand48(xi); 
      y = erand48(xi); 
      if (x*x + y*y <= 1.0) count++; 
    } 
  } 
  pi = 4.0 * (double)count / (double)samples; 
  error = fabs(pi-M_PI)*100/M_PI;
  printf("Count = %d, Samples = %d, Error: %7.5f%%\n", count, samples, error); 
} 

