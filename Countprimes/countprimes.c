/*
 
    Name: Jose Hichez
    PID: 6186740
    
    I affirm that I wrote this program myself
    without any help from any other people or
    sources from the internet.
    
    Project Description:
    This project is supposed to split up the calculation in
    finding the number of primes between 1 to 50000, with a
    user defined 'n' number of processes.
 
*/

#include <stdio.h>
#include <unistd.h>
#include <mpi.h>

#define NUMBER 50000

int countPrimes(int value) {
    int n, i, flag = 0;
    for (i = 2; i <= value / 2; i++) {
       if (value % i == 0) {
            flag = 1;
             break;
        }
    }
    if(value != 1 && value != 0 && flag == 0){
        return 1;
    }
    return 0;
}

int main(int argc, char *argv[]) {
  int count;           /* Solutions found by this proc */
  double elapsed_time; /* Time to find, count solutions */
  int global_count;    /* Total number of solutions */
  int i;
  int id; /* Process rank */
  int p;  /* Number of processes */
  char hostname[1024];

  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Comm_size(MPI_COMM_WORLD, &p);

  hostname[1023] = '\0';
  gethostname(hostname, 1023);
  // printf("MPI rank %d on host %s\n", id, hostname);

  /* Start timer */
  MPI_Barrier(MPI_COMM_WORLD);
  elapsed_time = -MPI_Wtime();

  count = 0;

  for (i = id; i < NUMBER; i += p)
    count += countPrimes(i);
  printf("Process ID: %d found %d\n", id, count);
  MPI_Reduce(&count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  /* Stop timer */
  elapsed_time += MPI_Wtime();

  if (!id)
  {
    printf("Total time %8.6f\n", elapsed_time);
    fflush(stdout);
  }
  MPI_Finalize();
  if (!id)
    printf("The total number of primes are %d \n",
           global_count);
  return 0;
}
