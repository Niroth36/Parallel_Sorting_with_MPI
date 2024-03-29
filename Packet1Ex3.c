#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

// Comparison function for qsort
int cmpfunc(const void* a, const void* b) {
    return (*(int*)b - *(int*)a);
}


int main(int argc, char* argv[]) {
    int N;  // size of large array A
    
    
    int my_rank, comm_size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

    // read in the value of n
   if (my_rank == 0) {
        printf("Enter a value: ");
        fflush(stdout);  // flush the output buffer
        scanf("%d", &N);
   }

   int A[N];  // large array A
   int my_A[N/comm_size];  // local array for each process

    // broadcast the value of n to all processes
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Seed the random number generator (only done by process 0)
    if (my_rank == 0) {
        srand(time(NULL));
    }

    // Generate comm_size random datasets in A (only done by process 0)
    if (my_rank == 0) {
        for (int i = 0; i < comm_size; i++) {
            for (int j = 0; j < N/comm_size; j++) {
                A[i*N/comm_size + j] = rand() % 100 + 1;  // random number from 1 to 100
            }
        }
    }

    // Divide the data in A among the processes using MPI_Scatter
    MPI_Scatter(A, N/comm_size, MPI_INT, my_A, N/comm_size, MPI_INT, 0, MPI_COMM_WORLD);

    // Each process sorts its local array using qsort
    qsort(my_A, N/comm_size, sizeof(int), cmpfunc);

    // Print the sorted numbers to the terminal
    for (int i = 0; i < N/comm_size; i++) {
        printf("Process %d: %d\n", my_rank, my_A[i]);
    }

    MPI_Finalize();
    return 0;
}

