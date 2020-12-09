#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

/*******************************************************/
void sender();
void receivers();
void init_vector();

/*******************************************************/

int world_size, myrank;
int vector_size;
int *vector;

int main(int argc, char** argv){

    vector_size = atoi(argv[1]);
    vector = (int *) malloc(vector_size * sizeof(int));

    init_vector();

    MPI_Init(NULL, NULL);
    
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    if (myrank == 0)
    {
        sender();
    } 
    else
    {
        receivers();
    }    

    MPI_Finalize();

    free(vector);

    return 0;
}

/*******************************************************/

void init_vector()
{
    srand(time(NULL));

    for (int i = 0; i < vector_size; ++i)
    {
        vector[i] = rand() % 100;
    }
}

void sender(){
    int max_partial, max, min_partial, min, sum_partial, sum;
    float average;

    MPI_Bcast(vector, vector_size, MPI_INT, 0, MPI_COMM_WORLD);

    printf("\nMy rank: %d of %d process \t Vector Values: ", myrank, world_size);

    max_partial = 0;
    min_partial = 100;
    sum_partial = 0;

    for (int i = 0; i < vector_size/world_size; ++i)
    {
        printf("%d\t", vector[i]);
        if (max_partial < vector[i])
        {
            max_partial = vector[i];
        }
        if (min_partial > vector[i])
        {
            min_partial = vector[i];
        }
        sum_partial+=vector[i];
    }
    printf("\n");

    MPI_Reduce(&max_partial, &max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);    
    MPI_Reduce(&min_partial, &min, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD); 
    MPI_Reduce(&sum_partial, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD); 

    average = (float)sum/vector_size;

    printf("\nMax value: %d\n", max);
    printf("Min value: %d\n", min);
    printf("Average of values: %.2f\n", average);
}

void receivers(){
    int begin, end, max_partial, max, min_partial, min, sum_partial, sum;

    MPI_Bcast(vector, vector_size, MPI_INT, 0, MPI_COMM_WORLD);

    begin = (vector_size/world_size)*myrank;
    end = (vector_size/world_size)*(myrank+1);

    if (end > vector_size || (vector_size-end) < (vector_size/world_size)) end = vector_size;

    printf("\nMy rank: %d of %d process \t Vector Values: ", myrank, world_size);
    
    max_partial = 0;
    min_partial = 100;
    sum_partial = 0;

    for (int i = begin; i < end; ++i)
    {
        printf("%d\t", vector[i]);
        if (max_partial < vector[i])
        {
            max_partial = vector[i];
        }
        if (min_partial > vector[i])
        {
            min_partial = vector[i];
        }
        sum_partial+=vector[i];
    }
    printf("\n");

    MPI_Reduce(&max_partial, &max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD); 
    MPI_Reduce(&min_partial, &min, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Reduce(&sum_partial, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);       
}